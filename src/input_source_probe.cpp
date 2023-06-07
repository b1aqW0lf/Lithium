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

#include <QRegExp>


namespace Analyze
{
    const char input_index[] = "Input\\s*#\\s*[\\d]*[,]?\\s*([\\w\\d]*\\s*[,]?\\s*[\\w\\d]*)";
    const char video_data[] = "Stream #([0-9]+).([0-9]+).*: Video:\\s*(([\\w\\d]*)\\s*[(\\w\\d)]*)\\s*[^)]*[)]*,[^)]*[)],\\s*([[0-9]*x[0-9]*)([^.]*SAR\\s*([0-9]*:[0-9]*)\\s*DAR\\s*([0-9]*:[0-9]*)[]]*)?(,\\s*([\\d]*)\\s*kb\\/s)?(,[^.]*)?,\\s*([\\d]*[.]*[\\d]*)?\\s*fps";
    //const char audio_data[] = "Stream #([0-9]+.[0-9]+).*: Audio:\\s*([\\w\\d]*),\\s*([0-9]+)\\s*Hz,\\s*([^,]*),\\s*([^,]*),\\s*([0-9]+)\\s*kb\\/s";
    //const char audio_data[] = "Stream #([0-9]+.[0-9]+).*: Audio:\\s*([\\w\\d]*)\\s*[^)]*[)]*,\\s*([\\w\\d]*)\\s*Hz,\\s([\\d]*[.]*[\\d]*)";
    const char audio_data[] = "Stream #([0-9]+.[0-9]+).*: Audio:\\s*([\\w\\d]*)\\s*[^)]*[)]*\\s*[^)]*[)]*,\\s*([\\w\\d]*)\\s*Hz,\\s([\\w\\d]*[.]*[\\d]*)([^,]*,\\s*[^,]*,\\s*([\\w\\d]*)\\s*kb\\/s)?";
    const char meta_data[] = "Duration:\\s*(([\\d]*):([\\d]*):([\\d]*[.]?[\\d]*)),\\s*start:\\s*([\\d]*[.]?[\\d]*),\\s*bitrate:\\s*([\\d]*)\\s*kb\\/s";
    const char profile_data[] = "profile\\s*=\\s*([\\d\\w]*)";
    const char sar_data[] = "sample_aspect_ratio\\s*=\\s*([0-9a-zA-Z]*[:]?[\\/]?[0-9a-zA-z]*)";
    const char dar_data[] = "display_aspect_ratio\\s*=\\s*([0-9a-zA-Z]*[:]?[\\/]?[0-9a-zA-z]*)";
    const char samplerate_data[] = "^sample_rate\\s*=\\s*([\\d]*)$";
    //const char bitrate_data[] = "^bit_rate\\s*=\\s*([\\w\\d]*[\\/]?[\\w\\d]*)?$(?!.*\1)";
}

InputSourceProbe::InputSourceProbe(QObject *parent) : QObject(parent)
{
    //connection with SelectSourceUI is made in MainWindow-->works!
    //ffprobe_started_message is implemented in MainWindow

    ffprobe = new QProcess;

    connect(this->ffprobe, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &InputSourceProbe::read_ffprobe_output);

    connect(this->ffprobe, &QProcess::started,
            this, &InputSourceProbe::ffprobe_started);/**/
}

//destructor
InputSourceProbe::~InputSourceProbe(){}

void InputSourceProbe::receive_vid_file_path(const QString &file)
{
    /*this->input_vid = file;

    //temp code - used to test statusbar connection only
    int message_timeout{0};
    Q_EMIT file_path(file,message_timeout);
    //---temp code------------------------------------//*/
}

void InputSourceProbe::receive_audio_file_path(const QString &file)
{
    /*this->input_aud = file;

    //temp code - used to test statusbar connection only
    int message_timeout{0};
    Q_EMIT file_path(file,message_timeout);
    //---temp code------------------------------------//*/
}

//experimental
void InputSourceProbe::start_probe_process(const QString &file)
{
    //run ffprobe on input file
    int timeout{0};
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
        //after ffprobe has exited
        while(this->ffprobe->canReadLine())
        {
            //begin reading data
            ffprobe_output = QString(this->ffprobe->readLine().trimmed());
            parse_output(ffprobe_output);
        }
    }
}

void InputSourceProbe::parse_output(const QString &data)
{
    parse_video_output(data);
    parse_audio_output(data);
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
        this->vidstream.video_str  = regx_vid.cap(0);
        this->vidstream.stream_index1 = regx_vid.cap(1);//VideoStream::stream_index1
        this->vidstream.stream_index2 = regx_vid.cap(2);//VideoStream::stream_index2
        this->vidstream.codec_name = regx_vid.cap(3);//VideoStream::codec_name
        this->vidstream.resolution = regx_vid.cap(5);//VideoStream::resolution
        this->vidstream.sample_aspect_ratio  = regx_vid.cap(7);//VideoStream::sample_aspect_ratio
        this->vidstream.display_aspect_ratio = regx_vid.cap(8);//VideoStream::display_aspect_ratio
        //this->vidstream.bit_rate   = regx_vid.cap(10);//VideoStream::bit_rate
        this->vidstream.frame_rate = regx_vid.cap(12);//VideoStream::frame_rate
    }
    Q_EMIT source_vid_codec_name(vidstream.codec_name);
    Q_EMIT source_vid_resolution(vidstream.resolution);
    Q_EMIT source_vid_frame_rate(vidstream.frame_rate);
    //used for testing only
    //Q_EMIT show_vid_data(vidstream.sample_aspect_ratio, timeout);

    QRegExp bitrate_regx(Analyze::meta_data);
    int bitrate_index{bitrate_regx.indexIn(data)};
    if(bitrate_index != -1)
    {
        this->vidstream.bit_rate = bitrate_regx.cap(6);
    }/**/
    Q_EMIT source_vid_bit_rate(vidstream.bit_rate);
    //check other meta data information
    if(bitrate_index != -1)
    {
        this->vidstream.duration = bitrate_regx.cap(1);
        this->vidstream.dur_hours = bitrate_regx.cap(2).toInt();
        this->vidstream.dur_mins = bitrate_regx.cap(3).toInt();
        this->vidstream.dur_secs = bitrate_regx.cap(4).toDouble();
        this->vidstream.start_time = bitrate_regx.cap(5).toDouble();
    }
    Q_EMIT source_vid_duration(vidstream.duration);
    Q_EMIT source_vid_dur_hours(vidstream.dur_hours);
    Q_EMIT source_vid_dur_mins(vidstream.dur_mins);
    Q_EMIT source_vid_dur_secs(vidstream.dur_secs);
    Q_EMIT source_vid_start_time(vidstream.start_time);

    //used for testing only--------------------------------//
    Q_EMIT show_vid_data(vidstream.duration, timeout);
    //----experimental------------------------------------//

    //verifying sample aspect ratio value
    QRegExp regx_sar(Analyze::sar_data);
    int index_sar{regx_sar.indexIn(data)};
    if(vidstream.sample_aspect_ratio.isEmpty() == true)
    {
        if(index_sar != -1)
        {
            this->vidstream.sample_aspect_ratio = regx_sar.cap(1);
        }
    }
    Q_EMIT source_vid_sample_aspect_ratio(vidstream.sample_aspect_ratio);

    //verifying the display aspect ratio value
    QRegExp regx_dar(Analyze::dar_data);
    int index_dar{regx_dar.indexIn(data)};
    if(vidstream.display_aspect_ratio.isEmpty() == true)
    {
        if(index_dar != -1)
        {
            this->vidstream.display_aspect_ratio = regx_dar.cap(1);
        }
    }
    Q_EMIT source_vid_display_aspect_ratio(vidstream.display_aspect_ratio);

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
}

void InputSourceProbe::parse_audio_output(const QString &data)
{
    int timeout{0};
    QRegExp regx_aud(Analyze::audio_data);
    int index{regx_aud.indexIn(data)};
    if(index != -1)
    {
        //reading the ffprobe output for desired data
        this->audiostream.stream_index1 = regx_aud.cap(1);
        this->audiostream.codec_name = regx_aud.cap(2);//AudioStream::codec_name
        this->audiostream.sample_rate = regx_aud.cap(3);
    }
    Q_EMIT source_audio_codec_name(audiostream.codec_name);
    //used for testing only
    //Q_EMIT show_audio_data(audstream.sample_rate, timeout);

    //check to verify if sample_rate string is empty
    if(audiostream.sample_rate.isEmpty() == true)
    {
        QRegExp regx_check(Analyze::samplerate_data);
        int index_sr{regx_check.indexIn(data)};
        if(index_sr != -1)
        {
            this->audiostream.sample_rate = regx_check.cap(1);
        }
    }
    Q_EMIT source_audio_samplerate(audiostream.sample_rate);
    //used for testing only
    Q_EMIT show_audio_data(audiostream.codec_name, timeout);
}
