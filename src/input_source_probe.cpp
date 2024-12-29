/******************************************************************************
 Copyright (c) 2020-2023 b1aqW0lf
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
    const char input_index[] = "Input\\s*#\\s*[\\d]*[,]?\\s*([\\w\\d]*\\s*[,]?\\s*[\\w\\d]*)";
    const char video_data[] = "Stream #([0-9]+):([0-9]+)[^.]*: Video:\\s*([\\w\\d]*)\\s?[(]?([\\w\\s\\d:]*)?[)]?[,]?[^.]*\\s+([[0-9]*x[0-9]*)\\s?([^.]*SAR\\s*([0-9]*:[0-9]*)\\s*DAR\\s*([0-9]*:[0-9]*)[]]*)?(,\\s*([\\d]*)\\s*kb\\s)?(,[^.]*)?,\\s*([\\d]*[.]*[\\d]*)?\\s*fps";
    const char audio_data[] = "Stream #(([0-9]+).([0-9]+)).*: Audio:\\s*([\\w\\d]*)[^,]*,\\s*([0-9]+)\\s*Hz,\\s*([^,]*),\\s*([^,]*,\\s*([0-9]+)\\s*kb\\/s)?";
    //const char video_data[] = "Stream #([0-9]+).([0-9]+).*: Video:\\s*(([\\w\\d]*)\\s*[(\\w\\d)]*)\\s*[^)]*[)]*,[^)]*[)],\\s*([[0-9]*x[0-9]*)([^.]*SAR\\s*([0-9]*:[0-9]*)\\s*DAR\\s*([0-9]*:[0-9]*)[]]*)?(,\\s*([\\d]*)\\s*kb\\/s)?(,[^.]*)?,\\s*([\\d]*[.]*[\\d]*)?\\s*fps";
    //const char audio_data[] = "Stream #(([0-9]+).([0-9]+)).*: Audio:\\s*([\\w\\d]*)[^,]*,\\s*([0-9]+)\\s*Hz,\\s*([^,]*),\\s*([^,]*,\\s*([0-9]+)\\s*kb\\/s)?(\\s*[^]*\\s*Metadata:[^.]*[^]*[^.]*\\s*DURATION\\s*:\\s*([\\d]*[:]?[\\d]*[:]?[\\d]*[.]?[\\d]*))?";
    const char meta_data[] = "Duration:\\s*(([\\d]*):([\\d]*):([\\d]*[.]?[\\d]*)),\\s*start:\\s*([\\d]*[.]?[\\d]*),\\s*bitrate:\\s*([\\d]*)\\s*kb\\/s";
    const char profile_data[] = "profile\\s*=\\s*([\\d\\w]*)";
    const char sar_data[] = "sample_aspect_ratio\\s*=\\s*([0-9a-zA-Z]*[:]?[\\/]?[0-9a-zA-z]*)";
    const char dar_data[] = "display_aspect_ratio\\s*=\\s*([0-9a-zA-Z]*[:]?[\\/]?[0-9a-zA-z]*)";
    const char samplerate_data[] = "^sample_rate\\s*=\\s*([\\d]*)$";
    const char duration_data[] = "codec_type\\s*=\\s*audio[^.]*[.]*[^.]*\\s*duration\\s*=\\s*([\\d]*[.]?[\\d]*)";
    const char colorspace_data[] = "^color_space=\\s*([\\w\\d]*)$";
    const char pixelformat_data[] = "^pix_fmt=\\s*([\\w\\d]*)$";
    const char codectype_audio[] = "\\s*Stream #[\\d]*:[\\d]*: Audio";
    //const char bitrate_data[] = "^bit_rate\\s*=\\s*([\\w\\d]*[\\/]?[\\w\\d]*)?$(?!.*\1)";
    const char video_codec_type[] = "codec_type=([video]+)";
    const char audio_codec_type[] = "codec_type=([audio]+)";
    const char coded_width_data[] = "^coded_width\\s*=\\s*([\\d]*)$";
    const char coded_height_data[] = "^coded_height\\s*=\\s*([\\d]*)$";
}

InputSourceProbe::InputSourceProbe(QObject *parent) : QObject(parent)
{
    //connection with SelectSourceUI is made in MainWindow-->works!
    //ffprobe_started_message is implemented in MainWindow

    ffprobe = new QProcess;

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
    //run ffprobe on input file
    this->input_file_flag = input_flag;
    input_file_title_check(file);

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
    Q_EMIT show_vid_data(videostream.sample_aspect_ratio, timeout);

    QRegularExpression bitrate_regx(Analyze::meta_data);
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
    Q_EMIT show_vid_data(videostream.codec_type, timeout);

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
    QRegularExpression regx_aud(QRegularExpression::anchoredPattern(QLatin1String(Analyze::audio_data)));
    QRegularExpressionMatch match = regx_aud.match(data);
    //int index{regx_aud.indexIn(data)};
    //if(index != -1)
    if(match.hasMatch())
    {
        //reading the ffprobe output for desired data
        this->audiostream.audio_str = match.captured(1);
        this->audiostream.stream_index1 = match.captured(2);//AudioStream::index1
        this->audiostream.stream_index2 = match.captured(3);//AudioStream::index2
        this->audiostream.codec_name = match.captured(4);//AudioStream::codec_name
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
        QRegularExpression regx_check(QRegularExpression::anchoredPattern(QLatin1String(Analyze::samplerate_data)));
        //int index_sr{regx_check.indexIn(data)};
        //if(index_sr != -1)
        match = regx_check.match(data);
        if(match.hasMatch())
        {
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

    /*//additional duration check
    if(this->audiostream.duration.isEmpty() == true)
    {
        QRegExp regx_duration(Analyze::duration_data);
        int index_dur{regx_duration.indexIn(data)};
        if(index_dur != -1)
        {
            this->audiostream.duration = regx_duration.cap(1);
        }
    }*/

    if(this->input_file_flag == "input2")
    {
        QRegularExpression codectype_regx(QRegularExpression::anchoredPattern(QLatin1String(Analyze::audio_codec_type)));
        //int ctype_index{codectype_regx.indexIn(data)};
        //if(ctype_index != -1)
        match = codectype_regx.match(data);
        if(match.hasMatch())
        {
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
