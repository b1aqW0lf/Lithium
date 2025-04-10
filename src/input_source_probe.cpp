/******************************************************************************
 Copyright (c) 2020-2025 b1aqW0lf
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/


#include "input_source_probe.h"

#include <QCoreApplication>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>


namespace Analyze
{
    const char video_data[] = "Stream #([0-9]+):([0-9]+)[^.]*: Video:\\s*([\\w\\d]*)\\s?[(]?([\\w\\s\\d:]*)?[)]?[,]?[^.]*\\s+([[0-9]*x[0-9]*)\\s?([^.]*SAR\\s*([0-9]*:[0-9]*)\\s*DAR\\s*([0-9]*:[0-9]*)[]]*)?(,\\s*([\\d]*)\\s*kb\\/s)?(,[^.]*)?,\\s*([\\d]*[.]*[\\d]*)?\\s*fps";
    const char audio_data[] = "Stream #([0-9]+):([0-9]+).*: Audio:\\s*([\\w\\d]*)\\s?[(]?([\\w\\s\\d:]*)?[)]?[,]?[^,]*,\\s*([0-9]+)\\s*Hz,\\s*([^,]*),\\s*([^,]*,\\s*([0-9]+)\\s*kb\\/s)?";
    const char duration_data[] = "Duration:\\s*((-?[\\d]*):([\\d]*):([\\d]*[.]?[\\d]*)),\\s*start:\\s*([\\d]*[.]?[\\d]*),\\s*bitrate:\\s*([\\d]*)\\s*kb\\/s";
    const char profile_data[] = "profile\\s*=\\s*([\\d\\w]*)";
    const char sar_data[] = "sample_aspect_ratio\\s*=\\s*([0-9a-zA-Z]*[:]?[\\/]?[0-9a-zA-z]*)";
    const char dar_data[] = "display_aspect_ratio\\s*=\\s*([0-9a-zA-Z]*[:]?[\\/]?[0-9a-zA-z]*)";
    const char samplerate_data[] = "^sample_rate\\s*=\\s*([\\d]*)$";
    const char audio_duration[] = "codec_type\\s*=\\s*audio[^.]*[.]*[^.]*\\s*duration\\s*=\\s*([\\d]*[.]?[\\d]*)";
    const char colorspace_data[] = "^color_space=\\s*([\\w\\d]*)$";
    const char pixelformat_data[] = "^pix_fmt=\\s*([\\w\\d]*)$";
    const char codectype_audio[] = "\\s*Stream #[\\d]*:[\\d]*: Audio";
    //const char bitrate_data[] = "^bit_rate\\s*=\\s*([\\w\\d]*[\\/]?[\\w\\d]*)?$(?!.*\1)";
    const char video_codec_type[] = "codec_type=([video]+)";
    const char audio_codec_type[] = "codec_type=([audio]+)";
    const char coded_width_data[] = "^coded_width\\s*=\\s*([\\d]*)$";
    const char coded_height_data[] = "^coded_height\\s*=\\s*([\\d]*)$";
    const char av1_profile_data[] = "Stream #[0-9]+:[0-9]+[^.]*: Video: av1\\s*[^]*[\\d\\w]?[^(]*[(]?([\\d\\w]*)?[)]?";
}

InputSourceProbe::InputSourceProbe(QObject *parent) : QObject(parent)
{
    this->ffprobe = new QProcess;
    this->ffprobe_cmd0 = new QProcess;

    connect(this->ffprobe_cmd0, &QProcess::readyReadStandardOutput, this, &InputSourceProbe::ffprobe_standard_output);
    connect(this->ffprobe, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &InputSourceProbe::read_ffprobe_output);

    connect(this->ffprobe, &QProcess::started,
            this, &InputSourceProbe::ffprobe_started);
}

//destructor
InputSourceProbe::~InputSourceProbe(){}

void InputSourceProbe::ffprobe_path_check()
{
#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffprobe.exe") &&
        QFile::exists(application_dir+"/ffmpeg/ffprobe.exe"))
    {
        //If ffprobe is found in both root directory and subdirectory
        //use ffprobe found in user-created "ffmpeg" subdirectory
        this->ffprobe_path = application_dir+"/ffmpeg/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir+"/ffmpeg");
    }
    if(QFile::exists(application_dir+"/ffprobe.exe"))
    {
        this->ffprobe_path = application_dir+"/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir);
    }
    if(QFile::exists(application_dir+"/ffmpeg/ffprobe.exe"))
    {
        this->ffprobe_path = application_dir+"/ffmpeg/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir+"/ffmpeg");
    }
#elif defined Q_OS_LINUX
    this->ffprobe_path = "ffprobe";
#endif
}

//experimental
void InputSourceProbe::start_probe_process(const QString &file, const QString &input_flag)
{
    this->count_frames_flag = false;
    this->input_file_flag = input_flag;
    /*if(input_file_flag == "input1")
    {
        this->count_frames_flag = true;
        this->ffprobe_process_cmd0(file);
    }*/

    this->ffprobe_process_cmd1(file);
    this->input_file_title_check(file);
}

void InputSourceProbe::ffprobe_process_cmd0(const QString &file)
{
    int timeout{0};
    ffprobe_path_check();
    if(ffprobe->QProcess::state() == QProcess::NotRunning)
    {
        if(count_frames_flag == true)
        {
            this->ffprobe_cmd0->setProcessChannelMode(QProcess::MergedChannels);
            this->ffprobe_cmd0->start(ffprobe_path, QStringList() <<  "-v" << "error"
                                                             << "-select_streams" << "v:0" << "-count_packets"
                                                             << "-show_entries" << "stream=nb_read_packets" << "-of"
                                                             << "csv=p=0" << file);
        }
    }
    else
    {
        Q_EMIT ffprobe_started_message("Process ffprobe_cmd0 still running and has not stopped", timeout);
    }
}

void InputSourceProbe::ffprobe_process_cmd1(const QString &file)
{
    int timeout{0};
    ffprobe_path_check();
    if(ffprobe->QProcess::state() == QProcess::NotRunning)
    {
        this->ffprobe->setProcessChannelMode(QProcess::MergedChannels);
        this->ffprobe->start(ffprobe_path, QStringList() << "-i" << file << "-show_streams");
    }
    else
    {
        Q_EMIT ffprobe_started_message("Process ffprobe still running and has not stopped", timeout);
    }
}

void InputSourceProbe::ffprobe_started()
{
    const int timeout{0};
    ffprobe->waitForStarted();

    if(ffprobe->QProcess::state() == QProcess::Running)
    {
        Q_EMIT ffprobe_started_message("ffprobe has started", timeout);
        //return;
    }
}
void InputSourceProbe::ffprobe_standard_output()
{
    if(this->count_frames_flag == true)
    {
        //read the frame count data
        videostream.nb_frames = this->ffprobe_cmd0->readAllStandardOutput().trimmed();
        Q_EMIT source_vid_frame_count(videostream.nb_frames);

        //<---used for testing only---------------------------------->//
        //Q_EMIT show_video_data("nb_frames = " + videostream.nb_frames, 0);
    }
}

void InputSourceProbe::read_ffprobe_output()
{
    QString ffprobe_output{};
    //ffprobe_output = this->ffprobe->readAllStandardOutput().trimmed();
    //verify ffprobe has stopped before reading data begins
    if(ffprobe->QProcess::exitStatus() == QProcess::NormalExit)
    {
        if(this->input_file_flag == "input1")
        {
            //after ffprobe has exited
            while(this->ffprobe->canReadLine())
            {
                //begin reading data
                ffprobe_output = QString(this->ffprobe->readLine().trimmed());
                parse_video_output(ffprobe_output);
                parse_audio_output(ffprobe_output);
            }
        }
        if(this->input_file_flag == "input2")
        {
            //after ffprobe has exited
            while(this->ffprobe->canReadLine())
            {
                //begin reading data
                ffprobe_output = QString(this->ffprobe->readLine().trimmed());
                parse_audio_output(ffprobe_output);
            }
        }
    }
}

//experimental
void InputSourceProbe::parse_video_output(const QString &data)
{
    int timeout{0};
    //QRegExp regx_vid(Analyze::video_data);
    QRegularExpression regx_vid(Analyze::video_data);
    QRegularExpressionMatchIterator itr = regx_vid.globalMatch(data);

    while (itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();

        //reading the ffprobe output for desired data
        this->videostream.stream_index1 = match.captured(1);//VideoStream::stream_index1
        this->videostream.stream_index2 = match.captured(2);//VideoStream::stream_index2
        this->videostream.codec_name = match.captured(3);//VideoStream::codec_name
        this->videostream.codec_profile = match.captured(4);//VideoStream::codec_profile
        this->videostream.resolution = match.captured(5);//VideoStream::resolution
        this->videostream.sample_aspect_ratio  = match.captured(7);//VideoStream::sample_aspect_ratio
        this->videostream.display_aspect_ratio = match.captured(8);//VideoStream::display_aspect_ratio
        //this->vidstream.bit_rate   = regx_vid.cap(10);//VideoStream::bit_rate
        this->videostream.frame_rate = match.captured(12);//VideoStream::frame_rate
    }
    Q_EMIT source_vid_codec_name(videostream.codec_name);
    Q_EMIT source_vid_resolution(videostream.resolution);
    Q_EMIT source_vid_frame_rate(videostream.frame_rate);
    //used for testing only
    //extra check for video codec profile - make sure a profile has been captured
    if(videostream.codec_profile.contains("libdav1d"))
    {
        QRegularExpression profile_regx(Analyze::av1_profile_data);
        itr = profile_regx.globalMatch(data);
        while (itr.hasNext())
        {
            QRegularExpressionMatch match = itr.next();
            this->videostream.codec_profile = match.captured(1);
        }
    }
    Q_EMIT source_vid_codec_profile(videostream.codec_profile);
    Q_EMIT show_video_data(videostream.sample_aspect_ratio, timeout);

    QRegularExpression bitrate_regx(Analyze::duration_data);
    itr = bitrate_regx.globalMatch(data);
    while (itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        //check other meta data information
        this->videostream.duration = match.captured(1);
        this->videostream.dur_hours = match.captured(2).toInt();
        this->videostream.dur_mins = match.captured(3).toInt();
        this->videostream.dur_secs = match.captured(4).toDouble();
        this->videostream.start_time = match.captured(5).toDouble();
        this->videostream.bit_rate = match.captured(6);
    }
    Q_EMIT source_vid_bit_rate(videostream.bit_rate);
    Q_EMIT source_vid_duration(videostream.duration);
    Q_EMIT source_vid_dur_hours(videostream.dur_hours);
    Q_EMIT source_vid_dur_mins(videostream.dur_mins);
    Q_EMIT source_vid_dur_secs(videostream.dur_secs);
    Q_EMIT source_vid_start_time(videostream.start_time);

    //used for testing only--------------------------------//
    //Q_EMIT show_vid_data(vidstream.duration, timeout);
    //----experimental------------------------------------//

    //verifying sample aspect ratio value
    if(videostream.sample_aspect_ratio.isEmpty() == true)
    {
        QRegularExpression regx_sar(Analyze::sar_data);
        itr = regx_sar.globalMatch(data);
        while(itr.hasNext())
        {
            QRegularExpressionMatch match = itr.next();
            this->videostream.sample_aspect_ratio = match.captured(1);
        }
    }
    Q_EMIT source_vid_sample_aspect_ratio(videostream.sample_aspect_ratio);

    //verifying the display aspect ratio value
    if(videostream.display_aspect_ratio.isEmpty() == true)
    {
        QRegularExpression regx_dar(Analyze::dar_data);
        itr = regx_dar.globalMatch(data);
        while(itr.hasNext())
        {
            QRegularExpressionMatch match = itr.next();
            this->videostream.display_aspect_ratio = match.captured(1);
        }
    }
    Q_EMIT source_vid_display_aspect_ratio(videostream.display_aspect_ratio);

    //verifying color space
    QRegularExpression colorspace_regx(Analyze::colorspace_data);
    itr = colorspace_regx.globalMatch(data);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->videostream.color_space = match.captured(1);
    }
    Q_EMIT source_vid_color_space(videostream.color_space);

    //verifying pixel format
    QRegularExpression pixformat_regx(Analyze::pixelformat_data);
    itr = pixformat_regx.globalMatch(data);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->videostream.pixel_format = match.captured(1);
    }
    Q_EMIT source_vid_pixel_format(videostream.pixel_format);
    //Q_EMIT show_vid_data(vidstream.pixel_format, timeout);

    QRegularExpression codectype_regx(Analyze::video_codec_type);
    itr = codectype_regx.globalMatch(data);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->videostream.codec_type = match.captured(1);
    }
    Q_EMIT show_video_data(videostream.codec_type, timeout);

    QRegularExpression coded_width_regex(Analyze::coded_width_data);
    itr = coded_width_regex.globalMatch(data);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->videostream.coded_width = match.captured(1);
    }

    QRegularExpression coded_height_regex(Analyze::coded_height_data);
    itr = coded_height_regex.globalMatch(data);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->videostream.coded_height = match.captured(1);
    }
    Q_EMIT source_vid_coded_resolution(videostream.coded_width, videostream.coded_height);
}

void InputSourceProbe::parse_audio_output(const QString &data)
{
    int timeout{0};
    QRegularExpression regx_aud(Analyze::audio_data);
    QRegularExpressionMatchIterator itr = regx_aud.globalMatch(data);

    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();

        //reading the ffprobe output for desired data
        this->audiostream.stream_index1 = match.captured(1);//AudioStream::index1
        this->audiostream.stream_index2 = match.captured(2);//AudioStream::index2
        this->audiostream.codec_name = match.captured(3);//AudioStream::codec_name
        this->audiostream.codec_profile = match.captured(4);//AudioStream::codec_profile
        this->audiostream.sample_rate = match.captured(5);//AudioStream::sample_rate
        this->audiostream.channels = match.captured(6);//AudioStream::channels
        this->audiostream.bit_rate = match.captured(8);//AudioStream::bit_rate
        this->audiostream.duration = match.captured(9);//AudioStream::duration
    }
    Q_EMIT source_audio_codec_name(audiostream.codec_name);
    Q_EMIT source_audio_channels(audiostream.channels);

    //check to verify if sample_rate string is empty
    if(this->audiostream.sample_rate.isEmpty() == true)
    {
        QRegularExpression regx_check(Analyze::samplerate_data);
        itr = regx_check.globalMatch(data);
        while(itr.hasNext())
        {
            QRegularExpressionMatch match = itr.next();
            this->audiostream.sample_rate = match.captured(1);
        }
    }
    Q_EMIT source_audio_samplerate(audiostream.sample_rate);

    //additional bitrate check
    if(this->audiostream.bit_rate.isEmpty() == true)
    {
        this->audiostream.bit_rate = "N/A";
    }
    Q_EMIT source_audio_bitrate(audiostream.bit_rate);

    if(this->input_file_flag == "input2")
    {
        QRegularExpression codectype_regx(Analyze::audio_codec_type);
        itr = codectype_regx.globalMatch(data);
        while(itr.hasNext())
        {
            QRegularExpressionMatch match = itr.next();
            this->audiostream.codec_type = match.captured(1);
        }
        //used for testing only
        Q_EMIT show_audio_data(audiostream.codec_type, timeout);
    }
    //---------^--------------------------------------------//
}

void InputSourceProbe::input_file_title_check(const QString &file)
{
    QString file_name = QFileInfo(file).fileName();
    QString input_title = file_name.left(file_name.lastIndexOf("."));
    Q_EMIT source_file_title(input_title);
}
