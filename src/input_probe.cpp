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


#include "input_probe.h"

#include <QCoreApplication>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QRegularExpressionMatch>


namespace Analyze
{
const char video_data[] = "Stream #([0-9]+):([0-9]+)[^.]*: Video:\\s*([\\w\\d]*)\\s?[(]?([\\w\\s\\d:]*)?[)]?[,]?[^.]*\\s+([[0-9]*x[0-9]*)\\s?([^.]*SAR\\s*([0-9]*:[0-9]*)\\s*DAR\\s*([0-9]*:[0-9]*)[]]*)?(,\\s*([\\d]*)\\s*kb\\/s)?(,[^.]*)?,\\s*([\\d]*[.]*[\\d]*)?\\s*fps";
const char vid_data_alt[] = "Stream #([0-9]+):([0-9]+)[^.]*: Video:\\s*([\\w\\d]*)\\s?[(]?([\\w\\s\\d:]*)?[)]?[^,]*[,]+\\s*([\\w\\d]*)[^.]*\\s+([[0-9]*x[0-9]*)\\s?([^.]*SAR\\s*([0-9]*:[0-9]*)\\s*DAR\\s*([0-9]*:[0-9]*)[]]*)?(,\\s*([\\d]*)\\s*kb\\/s)?(,[^.]*)?,\\s*([\\d]*[.]*[\\d]*)?\\s*fps";
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
const char video_codec_type[] = "codec_type=([video]+)";
const char audio_codec_type[] = "codec_type=([audio]+)";
const char coded_width_data[] = "^coded_width\\s*=\\s*([\\d]*)$";
const char coded_height_data[] = "^coded_height\\s*=\\s*([\\d]*)$";
const char av1_profile_data[] = "Stream #[0-9]+:[0-9]+[^.]*: Video: av1\\s*[^]*[\\d\\w]?[^(]*[(]?([\\d\\w]*)?[)]?";
}

InputProbe::InputProbe(QObject *parent)
    : QObject{parent}
{
    ffprobe = new QProcess(this);

    /*connect(this->ffprobe, &QProcess::readyReadStandardOutput, this, &InputProbe::ffprobe_standard_output);*/
    /*connect(this->ffprobe, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &InputProbe::read_ffprobe_output);*/
    connect(&encoder, &EncoderProcess::send_ffprobe_output, this, &InputProbe::read_ffprobe_output);
}

InputProbe::~InputProbe()
{
    delete ffprobe;
}

void InputProbe::receive_source_file(const QString &source_file)
{
    this->start_ffprobe_process(source_file);
}

void InputProbe::start_ffprobe_process(const QString &source_file)
{
    /*ffprobe_path_check();
    if(ffprobe->QProcess::state() == QProcess::NotRunning)
    {
        this->ffprobe->setProcessChannelMode(QProcess::MergedChannels);
        this->ffprobe->start(ffprobe_path, QStringList() << "-i" << source_file << "-show_streams");
    }*/
    encoder.start_ffprobe(QStringList() << "-i" << source_file << "-show_streams");
}

void InputProbe::ffprobe_path_check()
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

void InputProbe::ffprobe_standard_output()
{
    QString ffprobe_output{};
    //ffprobe_output = this->ffprobe->readAllStandardOutput().trimmed();
    //this->parse_video_output(ffprobe_output);
    while(this->ffprobe->canReadLine())
    {
        //begin reading data
        ffprobe_output = QString(this->ffprobe->readLine().trimmed());
        this->parse_video_output(ffprobe_output);
        this->parse_audio_output(ffprobe_output);
    }
}

/*void InputProbe::read_ffprobe_output(const int &exit_code)
{
    static const int normal_exit{0};
    QString ffprobe_output{};
    if(exit_code == normal_exit)
    {
        while(this->ffprobe->canReadLine())
        {
            //begin reading data
            ffprobe_output = QString(this->ffprobe->readLine().trimmed());
            this->parse_video_output(ffprobe_output);
            this->parse_audio_output(ffprobe_output);
        }
    }
    else
    {
        //do nothing
        return;
    }
}*/

void InputProbe::read_ffprobe_output(QString &ffprobe_output)
{
    this->parse_video_output(ffprobe_output);
    this->parse_audio_output(ffprobe_output);
}/**/

void InputProbe::parse_video_output(QString &output)
{
    //QRegularExpression regx_vid(Analyze::video_data);
    QRegularExpression regx_vid(Analyze::vid_data_alt);
    QRegularExpressionMatchIterator itr = regx_vid.globalMatch(output);

    while (itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();

        //reading the ffprobe output for desired data
        this->videostream.stream_index1 = match.captured(1);//VideoStream::stream_index1
        this->videostream.stream_index2 = match.captured(2);//VideoStream::stream_index2
        this->videostream.codec_name = match.captured(3);//VideoStream::codec_name
        this->videostream.codec_profile = match.captured(4);//VideoStream::codec_profile
        //--------------------------------------------------------------------------------------//
        this->videostream.pixel_format = match.captured(5);//VideoStream::pixel_format
        this->videostream.resolution = match.captured(6);//VideoStream::resolution
        this->videostream.sample_aspect_ratio  = match.captured(8);//VideoStream::sample_aspect_ratio
        this->videostream.display_aspect_ratio = match.captured(9);//VideoStream::display_aspect_ratio
        this->videostream.bitrate             = match.captured(11);//VideoStream::bitrate
        this->videostream.framerate = match.captured(13);//VideoStream::framerate
        //--------------------------------------------------------------------------------------//
        /*this->videostream.resolution = match.captured(5);//VideoStream::resolution
        this->videostream.sample_aspect_ratio  = match.captured(7);//VideoStream::sample_aspect_ratio
        this->videostream.display_aspect_ratio = match.captured(8);//VideoStream::display_aspect_ratio
        this->videostream.framerate = match.captured(12);//VideoStream::framerate*/
    }
    Q_EMIT this->send_source_video_bitrate(this->videostream.bitrate);

    //verifying the display aspect ratio value
    if(videostream.display_aspect_ratio.isEmpty() == true)
    {
        QRegularExpression regx_dar(Analyze::dar_data);
        itr = regx_dar.globalMatch(output);
        while(itr.hasNext())
        {
            QRegularExpressionMatch match = itr.next();
            this->videostream.display_aspect_ratio = match.captured(1);
        }
    }

    //additional check for the display aspect ratio
    if(this->videostream.display_aspect_ratio.contains("N/A", Qt::CaseInsensitive))
    {
        const int val0{0};
        const int val1{1};
        const int index = this->videostream.resolution.indexOf("x", Qt::CaseInsensitive);
        this->videostream.width = this->videostream.resolution.sliced(val0, index);
        this->videostream.height = this->videostream.resolution.sliced(index + val1);

        //force ffmpeg to calculate the display aspect ratio
        this->videostream.display_aspect_ratio = "setdar="+this->videostream.width +
                                                 "/" +this->videostream.height;
    }/**/

    //verifying pixel format
    if(this->videostream.pixel_format.isEmpty())
    {
        QRegularExpression pixformat_regx(Analyze::pixelformat_data);
        itr = pixformat_regx.globalMatch(output);
        while(itr.hasNext())
        {
            QRegularExpressionMatch match = itr.next();
            this->videostream.pixel_format = match.captured(1);
        }

        //additional check for pixel format -- if pixel_format is still empty
        if(this->videostream.pixel_format.isEmpty())
        {
            const char pixelformat_data_alt[] = "Stream #[0-9]+:[0-9]+[^.]*: Video:\\s*[\\w\\d]*\\s?[(]?[\\w\\s\\d:]*[)]?[^,]*[,]+\\s*([\\w\\d]*)";
            QRegularExpression pixformat_regx_alt(pixelformat_data_alt);
            itr = pixformat_regx_alt.globalMatch(output);
            while(itr.hasNext())
            {
                QRegularExpressionMatch match = itr.next();
                this->videostream.pixel_format = match.captured(1);
            }
        }
    }

    QRegularExpression codectype_regx(Analyze::video_codec_type);
    itr = codectype_regx.globalMatch(output);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->videostream.codec_type = match.captured(1);
    }

    Q_EMIT this->send_source_file_video_data(this->videostream.codec_name, this->videostream.resolution,
                                             this->videostream.framerate, this->videostream.display_aspect_ratio);
}

void InputProbe::parse_audio_output(QString &output)
{
    QRegularExpression regx_aud(Analyze::audio_data);
    QRegularExpressionMatchIterator itr = regx_aud.globalMatch(output);

    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();

        //reading the ffprobe output for desired data
        this->audiostream.stream_index1 = match.captured(1);//AudioStream::index1
        this->audiostream.stream_index2 = match.captured(2);//AudioStream::index2
        this->audiostream.codec_name = match.captured(3);//AudioStream::codec_name
        this->audiostream.codec_profile = match.captured(4);//AudioStream::codec_profile
        this->audiostream.samplerate = match.captured(5);//AudioStream::samplerate
        this->audiostream.channels = match.captured(6);//AudioStream::channels
        this->audiostream.bitrate = match.captured(8);//AudioStream::bitrate
        this->audiostream.duration = match.captured(9);//AudioStream::duration
    }

    QRegularExpression codectype_regx(Analyze::audio_codec_type);
    itr = codectype_regx.globalMatch(output);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->audiostream.codec_type = match.captured(1);
    }

    this->send_output_data();
    Q_EMIT this->send_source_file_audio_data(this->audiostream.codec_name, this->audiostream.bitrate,
                                             this->audiostream.samplerate, this->audiostream.channels);
}

void InputProbe::get_display_aspect_ratio(QString &output)
{
    //using the coded width and coded height to force ffmpeg to calcuate the
    //display aspect ratio
    QRegularExpression regx_coded_width(Analyze::coded_width_data);
    QRegularExpressionMatchIterator itr = regx_coded_width.globalMatch(output);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->videostream.coded_width = match.captured(1);
    }

    QRegularExpression regx_coded_height(Analyze::coded_height_data);
    itr = regx_coded_height.globalMatch(output);
    while(itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->videostream.coded_height = match.captured(1);
    }

    //force ffmpeg to calculate the display aspect ratio
    this->videostream.display_aspect_ratio = "setdar="+this->videostream.coded_width +
                                             "/" +this->videostream.coded_height;
}

void InputProbe::send_output_data()
{
    Q_EMIT send_input_probe_data(this->videostream.codec_name,
                                 this->videostream.resolution,
                                 this->videostream.framerate,
                                 this->videostream.display_aspect_ratio,
                                 this->videostream.pixel_format,
                                 this->videostream.codec_type,
                                 this->audiostream.codec_name,
                                 this->audiostream.channels,
                                 this->audiostream.codec_type);
}
