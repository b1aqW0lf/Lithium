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


#include "encoder_process.h"

#include <QCoreApplication>
#include <QDir>


EncoderProcess::EncoderProcess(QObject *parent)
    : QObject{parent}
{
    //initialize the ffmpeg and ffprobe processes
    ffmpeg = new QProcess(this);
    ffprobe = new QProcess(this);

    connect(this->ffprobe, &QProcess::readyReadStandardOutput, this, &EncoderProcess::ffprobe_standard_output);
    connect(this->ffmpeg, &QProcess::readyReadStandardOutput, this, &EncoderProcess::ffmpeg_standard_output);
    connect(&timer, &AbstractTimer::show_duration_timer, this, &EncoderProcess::encoder_process_message);
    connect(this->ffmpeg, &QProcess::started, this, &EncoderProcess::encoder_process_started);
    connect(this->ffmpeg, &QProcess::finished, this, &EncoderProcess::encoder_process_finished);
    connect(this->ffprobe, &QProcess::finished, this, &EncoderProcess::probing_process_finished);
}

EncoderProcess::~EncoderProcess()
{
    delete ffmpeg;
    delete ffprobe;
}

//check the location of ffmpeg, ffprobe and ffplay
void EncoderProcess::set_encoder_path(const QString &encoder)
{
#ifdef Q_OS_WIN
    QString encoder_path{};
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir + "/" + encoder + ".exe") &&
        !QFile::exists(application_dir + "/ffmpeg/" + encoder + ".exe"))
    {
        //root directory + ffmpeg executable
        encoder_path = application_dir + "/" + encoder + ".exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
    }
    if(QFile::exists(application_dir + "/ffmpeg/" + encoder + ".exe") &&
        !QFile::exists(application_dir + "/" + encoder + ".exe"))
    {
        //root directory + "ffmpeg" sub directory + ffmpeg executable
        encoder_path = application_dir + "/ffmpeg/" + encoder + ".exe";
        this->ffmpeg->setWorkingDirectory(application_dir + "/ffmpeg");
    }
    if(QFile::exists(application_dir + "/" + encoder + ".exe") &&
        QFile::exists(application_dir + "/ffmpeg/" + encoder + ".exe"))
    {
        //If ffmpeg is found in both root directory and subdirectory
        //use ffmpeg found in user-created "ffmpeg" subdirectory
        encoder_path = application_dir + "/ffmpeg/" + encoder + ".exe";
        this->ffmpeg->setWorkingDirectory(application_dir + "/ffmpeg");
    }

    if(encoder == "ffmpeg"){ this->ffmpeg_path = encoder_path; }
    if(encoder == "ffprobe"){ this->ffprobe_path = encoder_path; }
#elif defined Q_OS_LINUX
    if(encoder == "ffmpeg")
    {
        this->ffmpeg_path = encoder;
    }
    if(encoder == "ffprobe")
    {
        this->ffprobe_path = encoder;
    }
#endif
}

void EncoderProcess::start_ffprobe(const QStringList &arguments)
{
    this->set_encoder_path("ffprobe");
    if(ffprobe->QProcess::state() == QProcess::NotRunning)
    {
        this->ffprobe->setProcessChannelMode(QProcess::MergedChannels);
        this->ffprobe->start(ffprobe_path, arguments);
    }
}

void EncoderProcess::start_ffmpeg(const QStringList &arguments)
{
    this->set_encoder_path("ffmpeg");
    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, arguments);
}

void EncoderProcess::stop_ffmpeg()
{
    this->stop_encoder_timer();
    //cancel the transcoding process
    this->ffmpeg->kill();
    this->ffmpeg->close();
    this->ffmpeg->closeWriteChannel();
}

void EncoderProcess::stop_encoder_timer()
{
    timer.stop_timer();
    //reset the duration time to 00:00:00
    timer.reset_duration_time();
}

void EncoderProcess::ffprobe_standard_output()
{
    QString ffprobe_output{};
    ffprobe_output = this->ffprobe->readAllStandardOutput().trimmed();
    Q_EMIT this->send_ffprobe_output(ffprobe_output);
}

void EncoderProcess::ffmpeg_standard_output()
{
    QString ffmpeg_output{};
    ffmpeg_output.append(this->ffmpeg->readAllStandardOutput().trimmed());
    Q_EMIT this->send_ffmpeg_output(ffmpeg_output);
}

void EncoderProcess::encoder_process_started()
{
    const int timeout{0};
    timer.start_timer();
    //leave message blank to send general message
    this->encoder_process_message("", timeout);
}

void EncoderProcess::encoder_process_finished(int exit_code)
{
    this->stop_encoder_timer();
    Q_EMIT this->send_encoder_process_exit_code(exit_code);
}

void EncoderProcess::probing_process_finished(int exit_code)
{
    QString ffprobe_output{};
    if(exit_code == QProcess::NormalExit)
    {
        while(this->ffprobe->canReadLine())
        {
            //begin reading data
            ffprobe_output = QString(this->ffprobe->readLine().trimmed());
            Q_EMIT this->send_ffprobe_output(ffprobe_output);
        }
    }
    else
    {
        //do nothing
        return;
    }
}

void EncoderProcess::encoder_process_message(const QString &message, const int &timeout)
{
    //send transcode message
    Q_EMIT send_encoder_process_message(message, timeout);
}
