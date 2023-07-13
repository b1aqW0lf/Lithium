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

Transcode::~Transcode(){}

void Transcode::receive_source_video_file(const QString &source_video)
{
    this->source_vid_file = source_video;
}

void Transcode::source_input_file_check()
{
    //conversion preparation
    //ffmpeg argument
    QStringList args;
    int timeout{0};

    //check for existence of the input file
    if(this->source_vid_file.isEmpty())
    {
        Q_EMIT source_vid_file_status(tr("No Input"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Input file not specified"));

        //ui->actionEncode->setChecked(false);
        Q_EMIT encode_button_set_checked(false);
        return; //nothing is returned
    }
}

void Transcode::receive_output_file_path(const QString &output_path)
{
    this->output_vid_file = output_path;
}

void Transcode::output_video_path_check()
{
    int timeout{0};

    //check for the existence of a specified output file
    if(this->output_vid_file.isEmpty())
    {
        Q_EMIT output_vid_file_status(tr("No Output"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Output file not specified"));

        Q_EMIT encode_button_set_checked(false);
        return; //nothing is returned
    }

    //check if specified output file already exists
    //use output_vid_file in this test
    Q_EMIT output_vid_file_status(tr("Output file check... ").append(this->output_vid_file), timeout);
    if(QFile::exists(this->output_vid_file))
    {
        if(QMessageBox::question(this, tr("Lithium"),
                                  tr("There already exists a file called %1 in "
                                     "the currect directory. Overwrite file?").arg(output_vid_file),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            return;
        QFile::remove(this->output_vid_file);

        while(QFile::exists(this->output_vid_file))
        {
            Q_EMIT output_vid_file_status(tr("Output file path is set"), timeout);
        }
    }
}

void Transcode::two_pass_encode_enabled(const bool &status)
{
    this->two_pass_enabled = status;
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
        return;
    }
    else if(this->extract_mode == true)
    {
        return;
    }
}

//use with actionEncode
void Transcode::start_normal_mode_transcode()
{
    //normal_mode_transcode();
}

void Transcode::normal_mode_transcode()
{
    int timeout{0};
    source_input_file_check();
    output_video_path_check();

    if(this->two_pass_enabled == true)
    {

        return;
    }
    if(this->two_pass_enabled == false)
    {
        //normal transcode
        args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
             << "-i" << source_vid_file << "-c:v" << video_codec << "-crf"
             << crf_value << "-speed" << vid_encoder_preset << "-c:a" << audio_codec
             << "-b:a" << audio_bitrate << output_vid_file;

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
        if((this->ffmpeg->QProcess::state() == QProcess::Running))
        {
            //this logic works!
            Q_EMIT send_encoder_status(tr("Encoding Started "), timeout);
        }
        args.clear();/**/
    }
}

void Transcode::cancel_encode_process()
{
    int timeout{0};

    this->ffmpeg->kill();
    this->ffmpeg->close();
    this->ffmpeg->closeWriteChannel();
    Q_EMIT send_encoder_status(tr("Encoding Cancelled "), timeout);
}

void Transcode::encoding_process_finished()
{
    int timeout{0};

    //Set the encoding status by checking output file's existence
    this->ffmpeg->waitForFinished();

    if(QFile::exists(output_vid_file) &&
        (this->ffmpeg->QProcess::exitStatus() == QProcess::NormalExit))
    {
        Q_EMIT send_encoder_status(tr("Encoding Status: Successful "), timeout);
        //playOutput->setEnabled(true);//<===pay attention to this!!, no ui
    }
    Q_EMIT enable_encode_button();
    this->ffmpeg->closeWriteChannel();//close write channel after encoding finishes
}
