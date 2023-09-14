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


#include "transcode.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>

Transcode::Transcode(QWidget *parent)
    : QWidget{parent}
{
    ffmpeg = new QProcess{this};

    //local connection
    connect(this->ffmpeg, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Transcode::encoding_process_finished);
}

Transcode::~Transcode()
{
    //stop ffmpeg if application closes before it exits normally
    delete ffmpeg;
}

void Transcode::receive_source_video_file(const QString &source_video)
{
    this->source_video_file = source_video;
}

void Transcode::receive_source_audio_file(const QString &source_audio)
{
    this->source_audio_file = source_audio;
}

void Transcode::source_video_file_check()
{
    //conversion preparation
    int timeout{0};

    //check for existence of the input file
    if(this->source_video_file.isEmpty())
    {
        Q_EMIT source_file_status(tr("No Input"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Input file not specified"));

        //ui->actionEncode->setChecked(false);
        Q_EMIT enable_encode_button();
        return; //nothing is returned
    }
}

void Transcode::source_audio_file_check()
{
    //conversion preparation
    int timeout{0};

    //check for existence of the input file
    if(this->source_audio_file.isEmpty())
    {
        Q_EMIT source_file_status(tr("No Input"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Input file not specified"));

        //ui->actionEncode->setChecked(false);
        Q_EMIT enable_encode_button();
        return; //nothing is returned
    }
}

void Transcode::receive_output_file_path(const QString &output_path)
{
    this->output_file = output_path;
}

void Transcode::output_file_path_check()
{
    int timeout{0};

    //check for the existence of a specified output file
    if(this->output_file.isEmpty())
    {
        Q_EMIT output_file_status(tr("No Output"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Output file not specified"));

        Q_EMIT enable_encode_button();
        return; //nothing is returned
    }

    //check if specified output file already exists
    if(QFile::exists(this->output_file))
    {
        //use output_vid_file in this test
        Q_EMIT output_file_status(tr("Output file check... ").append(this->output_file), timeout);
        if(QMessageBox::question(this, tr("Lithium"),
                                  tr("There already exists a file called %1 in "
                                     "the currect directory. Overwrite file?").arg(output_file),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            return;
        QFile::remove(this->output_file);

        while(QFile::exists(this->output_file))
        {
            Q_EMIT output_file_status(tr("Output file path is set"), timeout);
        }
    }
}

void Transcode::two_pass_encode_enabled(const bool &status)
{
    this->two_pass_enabled = status;
}

void Transcode::average_bitrate_encode_enabled(const bool &status)
{
    this->average_bitrate_enabled = status;
}

void Transcode::transcode_processing_mode(const bool &normal, const bool &merge, const bool &extract)
{
    this->normal_mode = normal;
    this->merge_mode = merge;
    this->extract_mode = extract;
}

//receive current audio options
void Transcode::receive_current_audio_options(const QString &codec, const QString &channels,
                                              const QString &samplerate, const QString &bitrate)
{
    this->audio_codec = codec;
    this->audio_channels = channels;
    this->audio_samplerate = samplerate;
    this->audio_bitrate = bitrate;
}

//receive current video options
void Transcode::receive_current_video_options(const QString &codec, const QString &video_bitrate,
                                              const QString &crf_value, const QString &qscale_value,
                                              const QString &video_res_value, const QString &video_dar_value,
                                              const QString &video_fps_val, const QString &encoder_preset_val)
{
    this->video_codec = codec;
    this->vid_avg_bitrate = video_bitrate;
    this->crf_value = crf_value;
    this->qscale_value = qscale_value;
    this->video_res = video_res_value;
    this->video_dar = video_dar_value;
    this->video_fps = video_fps_val;
    this->vid_encoder_preset = encoder_preset_val;

    //start encode
    start_encode_mode_check();/**/
}


void Transcode::start_encode_mode_check()
{
    if(this->normal_mode == true)
    {
        normal_mode_transcode();
    }
    else if(this->merge_mode == true)
    {
        merge_mode_transcode();
    }
    else if(this->extract_mode == true)
    {
        extract_mode_transcode();
    }
}

void Transcode::normal_mode_transcode()
{
    int timeout{0};
    source_video_file_check();
    output_file_path_check();


    if(this->average_bitrate_enabled == false)
    {
        //normal transcode
        args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
             << "-i" << source_video_file << "-c:v" << video_codec
             << "-crf" << crf_value << "-preset" << vid_encoder_preset
             << "-color_primaries" << "1" << "-color_trc" << "1"
             << "-colorspace" << "1" << "-c:a" << audio_codec
             << "-map_metadata" << "0" << output_file;

#ifdef Q_OS_WIN
        QString application_path{QCoreApplication::applicationDirPath()};
        QString application_dir{QDir(application_path).absolutePath()};

        if(QFile::exists(application_dir+"/ffmpeg.exe"))
        {
            this->ffmpeg_path = application_dir+"/ffmpeg.exe";
            this->ffmpeg->setWorkingDirectory(application_dir);
        }
        if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
        {
            this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
            this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
        }
#elif defined Q_OS_LINUX
        this->ffmpeg_path = "ffmpeg";
#endif
        this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
        this->ffmpeg->start(this->ffmpeg_path, args);
        this->ffmpeg->waitForStarted();
        if(this->ffmpeg->QProcess::state() == QProcess::Running)
        {
            //this logic works!
            Q_EMIT send_encoder_status(tr("Encoding Started "), timeout);
        }
        args.clear();
    }
    if(this->average_bitrate_enabled == true)/**/
    {
        if(this->two_pass_enabled == false)
        {
            average_bitrate_encode();
        }
        if(this->two_pass_enabled == true)
        {
            two_pass_encode_1st_pass();
            two_pass_encode_2nd_pass();
        }
    }
}

void Transcode::average_bitrate_encode()
{
    int timeout{0};

    //normal transcode + average bitrate
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-c:v" << video_codec
         << "-b:v" << vid_avg_bitrate << "-preset" << vid_encoder_preset
         << "-color_primaries" << "1" << "-color_trc" << "1"
         << "-colorspace" << "1" << "-c:a" << audio_codec
         << "-map_metadata" << "0" << output_file;

#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
    }
    if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif
    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        //this logic works!
        Q_EMIT send_encoder_status(tr("Encoding Started "), timeout);
    }
    args.clear();
}

void Transcode::two_pass_encode_1st_pass()
{
    int timeout{0};
    int msecs{-1};

    //normal transcode + 2-pass encoding
    //1st pass
    QDir file = QFileInfo(source_video_file).absoluteDir();
    pass_log_location = file.absolutePath()+"/passlogfile";

    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-sn" << "-c:v" << video_codec
         << "-pix_fmt" << "yuv420p" << "-b:v" << vid_avg_bitrate << "-preset"
         << vid_encoder_preset << "-map_metadata" << "0" << "-y"
         << "-passlogfile" << pass_log_location << "-pass" << "1" << "-an"
         << "-f" << "mp4" << "/dev/null";

#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
    }
    if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif
    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        //this logic works!
        Q_EMIT send_encoder_status(tr("2 Pass Encode Started - 1st Pass "), timeout);
    }
    this->ffmpeg->waitForFinished(msecs);
    args.clear();
}

void Transcode::two_pass_encode_2nd_pass()
{
    int timeout{0};
    int msecs{-1};

    //normal transcode + 2-pass encoding
    //2nd Pass
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-sn" << "-c:v" << video_codec
         << "-pix_fmt" << "yuv420p" << "-b:v" << vid_avg_bitrate << "-preset"
         << vid_encoder_preset << "-map_metadata" << "0" << "-y" << "-passlogfile"
         << pass_log_location << "-pass" << "2" << "-c:a" << audio_codec
         << "-f" << "mp4" << output_file;

#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
    }
    if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif
    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        //this logic works!
        Q_EMIT send_encoder_status(tr("2 Pass Encode Started - 2nd Pass "), timeout);
    }
    this->ffmpeg->waitForFinished(msecs);
    args.clear();
}

void Transcode::merge_mode_transcode()
{
    int timeout{0};
    source_video_file_check();
    source_audio_file_check();
    output_file_path_check();

    //merge sources transcode
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-i" << source_audio_file
         << "-c:v" << video_codec << "-crf" << crf_value << "-preset"
         << vid_encoder_preset << "-c:a" << audio_codec << "-map_metadata"
         << "0" << output_file;

#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
    }
    if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif
    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        //this logic works!
        Q_EMIT send_encoder_status(tr("Encoding Started -- Merging "), timeout);
    }
    args.clear();
}

void Transcode::extract_mode_transcode()
{
    int timeout{0};
    source_video_file_check();
    output_file_path_check();

    //extract audio transcode
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-vn" << "-c:a" << audio_codec
         << "-map_metadata" << "0" << output_file;

#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
    }
    if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif
    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        //this logic works!
        Q_EMIT send_encoder_status(tr("Encoding Started -- Extracting "), timeout);
    }
    args.clear();
}

void Transcode::cancel_encode_process()
{
    int timeout{0};

    if(this->ffmpeg->QProcess::state() == QProcess::NotRunning)
    {
        //check for existence of the input file
        if(this->source_video_file.isEmpty())
        {
            Q_EMIT source_file_status(tr("No Input to cancel"), timeout);
            QMessageBox::information(this, tr("Lithium"),
                                     tr("Input file not specified"));
            return; //nothing is returned
        }

        //check for the existence of a specified output file
        if(this->output_file.isEmpty())
        {
            Q_EMIT output_file_status(tr("No Output is being processed"), timeout);
            QMessageBox::information(this, tr("Lithium"),
                                     tr("No Output file was specified"));
            return; //nothing is returned
        }

        //check if source_vid_file has data but ffmpeg is not running
        if(!this->source_video_file.isEmpty())
        {
            this->ffmpeg->closeWriteChannel();
            Q_EMIT send_encoder_status(tr("Encoding Cancelled "), timeout);
        }
    }
    else
    {
        this->ffmpeg->kill();
        this->ffmpeg->close();
        this->ffmpeg->closeWriteChannel();
        Q_EMIT send_encoder_status(tr("Encoding Cancelled "), timeout);
    }
}

void Transcode::encoding_process_finished()
{
    int timeout{0};

    //Set the encoding status by checking output file's existence
    this->ffmpeg->waitForFinished();

    if(QFile::exists(output_file) &&
        (this->ffmpeg->QProcess::exitStatus() == QProcess::NormalExit))
    {
        Q_EMIT send_encoder_status(tr("Encoding Status: Successful "), timeout);
        //playOutput->setEnabled(true);//<===pay attention to this!!, no ui
    }
    Q_EMIT enable_encode_button();
    this->ffmpeg->closeWriteChannel();//close write channel after encoding finishes
}
