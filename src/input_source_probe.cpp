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
#include <QRegExp>


namespace Analyze
{
    const char input_index[] = "Input\\s*#\\s*[\\d]*[,]?\\s*([\\w\\d]*\\s*[,]?\\s*[\\w\\d]*)";
    const char video_data[] = "Stream #([0-9]+).([0-9]+).*: Video:\\s*(([\\w\\d]*)\\s*[(\\w\\d)]*)\\s*[^)]*[)]*,[^)]*[)],\\s*([[0-9]*x[0-9]*)([^.]*SAR\\s*([0-9]*:[0-9]*)\\s*DAR\\s*([0-9]*:[0-9]*)[]]*)?(,\\s*([\\d]*)\\s*kb\\/s)?(,[^.]*)?,\\s*([\\d]*[.]*[\\d]*)?\\s*fps";
    const char audio_data[] = "Stream #(([0-9]+).([0-9]+)).*: Audio:\\s*([\\w\\d]*)[^,]*,\\s*([0-9]+)\\s*Hz,\\s*([^,]*),\\s*([^,]*,\\s*([0-9]+)\\s*kb\\/s)?";
    //const char audio_data[] = "Stream #(([0-9]+).([0-9]+)).*: Audio:\\s*([\\w\\d]*)[^,]*,\\s*([0-9]+)\\s*Hz,\\s*([^,]*),\\s*([^,]*,\\s*([0-9]+)\\s*kb\\/s)?(\\s*[^]*\\s*Metadata:[^.]*[^]*[^.]*\\s*DURATION\\s*:\\s*([\\d]*[:]?[\\d]*[:]?[\\d]*[.]?[\\d]*))?";
    const char audio_data2[] = "Stream #(([0-9]+).([0-9]+)).*: Audio:\\s*([\\w\\d]*)[^,]*,\\s*([0-9]+)\\s*Hz,\\s*([^,]*),\\s*([^,]*),\\s*([0-9]+)\\s*kb\\/s\\s*[^ ]*\\s*Metadata:[^.]*[^ ]*[^.]*\\s*DURATION\\s*:\\s*([\\d]*:[\\d]*:[\\d]*[.]?[\\d]*)";
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
    QRegExp regx_vid(Analyze::video_data);
    int index{regx_vid.indexIn(data)};
    if(index != -1)
    {
        //reading the ffprobe output for desired data
        this->videostream.video_str  = regx_vid.cap(0);
        this->videostream.stream_index1 = regx_vid.cap(1);//VideoStream::stream_index1
        this->videostream.stream_index2 = regx_vid.cap(2);//VideoStream::stream_index2
        this->videostream.codec_name = regx_vid.cap(3);//VideoStream::codec_name
        this->videostream.resolution = regx_vid.cap(5);//VideoStream::resolution
        this->videostream.sample_aspect_ratio  = regx_vid.cap(7);//VideoStream::sample_aspect_ratio
        this->videostream.display_aspect_ratio = regx_vid.cap(8);//VideoStream::display_aspect_ratio
        //this->vidstream.bit_rate   = regx_vid.cap(10);//VideoStream::bit_rate
        this->videostream.frame_rate = regx_vid.cap(12);//VideoStream::frame_rate
    }
    Q_EMIT source_vid_codec_name(videostream.codec_name);
    Q_EMIT source_vid_resolution(videostream.resolution);
    Q_EMIT source_vid_frame_rate(videostream.frame_rate);
    //used for testing only
    //Q_EMIT show_vid_data(vidstream.sample_aspect_ratio, timeout);

    QRegExp bitrate_regx(Analyze::meta_data);
    int bitrate_index{bitrate_regx.indexIn(data)};
    if(bitrate_index != -1)
    {
        this->videostream.bit_rate = bitrate_regx.cap(6);
    }/**/
    Q_EMIT source_vid_bit_rate(videostream.bit_rate);
    //check other meta data information
    if(bitrate_index != -1)
    {
        this->videostream.duration = bitrate_regx.cap(1);
        this->videostream.dur_hours = bitrate_regx.cap(2).toInt();
        this->videostream.dur_mins = bitrate_regx.cap(3).toInt();
        this->videostream.dur_secs = bitrate_regx.cap(4).toDouble();
        this->videostream.start_time = bitrate_regx.cap(5).toDouble();
    }
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
        QRegExp regx_sar(Analyze::sar_data);
        int index_sar{regx_sar.indexIn(data)};
        if(index_sar != -1)
        {
            this->videostream.sample_aspect_ratio = regx_sar.cap(1);
        }
    }
    Q_EMIT source_vid_sample_aspect_ratio(videostream.sample_aspect_ratio);

    //verifying the display aspect ratio value
    if(videostream.display_aspect_ratio.isEmpty() == true)
    {
        QRegExp regx_dar(Analyze::dar_data);
        int index_dar{regx_dar.indexIn(data)};
        if(index_dar != -1)
        {
            this->videostream.display_aspect_ratio = regx_dar.cap(1);
        }
    }
    Q_EMIT source_vid_display_aspect_ratio(videostream.display_aspect_ratio);

    //verifying color space
    QRegExp colorspace_regx(Analyze::colorspace_data);
    int cspace_index{colorspace_regx.indexIn(data)};
    if(cspace_index != -1)
    {
        this->videostream.color_space = colorspace_regx.cap(1);
    }
    Q_EMIT source_vid_color_space(videostream.color_space);

    //verifying pixel format
    QRegExp pixformat_regx(Analyze::pixelformat_data);
    int pformat_index{pixformat_regx.indexIn(data)};
    if(pformat_index != -1)
    {
        this->videostream.pixel_format = pixformat_regx.cap(1);
    }
    Q_EMIT source_vid_pixel_format(videostream.pixel_format);

    //Q_EMIT show_vid_data(vidstream.pixel_format, timeout);

    //verifying the bitrate value
    /*QRegExp regx_brate(Analyze::bitrate_data);//this was on
    int index_brate{regx_brate.indexIn(data)};
    if(vidstream.bit_rate.isEmpty() == true)
    {
        if(index_brate != -1)
        {
            this->vidstream.bit_rate = regx_brate.cap(1);
        }
    }*/
    //Q_EMIT show_vid_data(vidstream.display_aspect_ratio, timeout);

    QRegExp codectype_regx(Analyze::video_codec_type);
    int ctype_index{codectype_regx.indexIn(data)};
    if(ctype_index != -1)
    {
        this->videostream.codec_type = codectype_regx.cap(1);
    }
    Q_EMIT show_vid_data(videostream.codec_type, timeout);
}

void InputSourceProbe::parse_audio_output(const QString &data)
{
    int timeout{0};
    QRegExp regx_aud(Analyze::audio_data);
    int index{regx_aud.indexIn(data)};
    if(index != -1)
    {
        //reading the ffprobe output for desired data
        this->audiostream.audio_str = regx_aud.cap(1);
        this->audiostream.stream_index1 = regx_aud.cap(2);//AudioStream::index1
        this->audiostream.stream_index2 = regx_aud.cap(3);//AudioStream::index2
        this->audiostream.codec_name = regx_aud.cap(4);//AudioStream::codec_name
        this->audiostream.sample_rate = regx_aud.cap(5);//AudioStream::sample_rate
        this->audiostream.channels = regx_aud.cap(6);//AudioStream::channels
        this->audiostream.bit_rate = regx_aud.cap(8);//AudioStream::bit_rate
        this->audiostream.duration = regx_aud.cap(9);//AudioStream::duration
    }
    Q_EMIT source_audio_codec_name(audiostream.codec_name);
    Q_EMIT source_audio_channels(audiostream.channels);

    //check to verify if sample_rate string is empty
    if(this->audiostream.sample_rate.isEmpty() == true)
    {
        QRegExp regx_check(Analyze::samplerate_data);
        int index_sr{regx_check.indexIn(data)};
        if(index_sr != -1)
        {
            this->audiostream.sample_rate = regx_check.cap(1);
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
        QRegExp codectype_regx(Analyze::audio_codec_type);
        int ctype_index{codectype_regx.indexIn(data)};
        if(ctype_index != -1)
        {
            this->audiostream.codec_type = codectype_regx.cap(1);
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
}
