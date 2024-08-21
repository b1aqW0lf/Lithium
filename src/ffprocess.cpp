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


#include <QCoreApplication>
#include <QDir>

#include "ffprocess.h"


DetectFFmpeg::DetectFFmpeg(QProcess *parent)
    : QProcess{parent}
{
    //create the processto be used
    this->ffmpeg = new QProcess{this};

    //connect signals and slots
    connect(this->ffmpeg, &QProcess::readyReadStandardOutput,
            this, &DetectFFmpeg::ffmpegReadStandardOutput);

    connect(this->ffmpeg, &QProcess::started,
            this, &DetectFFmpeg::ffmpeg_process_started);

    //location of ffmpeg, ffplay, and ffprobe
    ffmpeg_location_setup();
    ffprobe_location_setup();
    ffplay_location_setup();

    //loads ffmpeg on startup
    generate_ffmpeg_version_prompt();
}

DetectFFmpeg::~DetectFFmpeg()
{
    delete ffmpeg;
}

void DetectFFmpeg::ffmpegReadStandardOutput()
{
    QString ffmpeg_output{};
    //ffmpeg process' readyReadStandardOutput implementation
    ffmpeg_output = this->ffmpeg->readAllStandardOutput();//single line stats
    Q_EMIT ffmpeg_read_output(ffmpeg_output);
    send_ffmpeg_status();
}

void DetectFFmpeg::ffmpeg_location_setup()
{

#ifdef Q_OS_WIN
    ffmpeg_location_check("ffmpeg");
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif

}

void DetectFFmpeg::ffprobe_location_setup()
{

#ifdef Q_OS_WIN
    ffmpeg_location_check("ffprobe");
#elif defined Q_OS_LINUX
    this->ffprobe_path = "ffprobe";
#endif

}

void DetectFFmpeg::ffplay_location_setup()
{

#ifdef Q_OS_WIN
    ffmpeg_location_check("ffplay");
#elif defined Q_OS_LINUX
    this->ffplay_path = "ffplay";
#endif

}

//check the location of ffmpeg, ffprobe and ffplay
void DetectFFmpeg::ffmpeg_location_check(const QString &app)
{
    int timeout{0};
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir + "/" + app + ".exe") &&
        !QFile::exists(application_dir + "/ffmpeg/" + app + ".exe"))
    {
        //Lithium root directory + ffmpeg executable
        this->ffmpeg_path = application_dir + "/" + app + ".exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
        set_ffmpeg_ready_status(app);
    }
    else if(QFile::exists(application_dir + "/ffmpeg/" + app + ".exe") &&
            !QFile::exists(application_dir + "/" + app + ".exe"))
    {
        //Lithium root directory + "ffmpeg" sub directory + ffmpeg executable
        this->ffmpeg_path = application_dir + "/ffmpeg/" + app + ".exe";
        this->ffmpeg->setWorkingDirectory(application_dir + "/ffmpeg");
        set_ffmpeg_ready_status(app);
    }
    else if(QFile::exists(application_dir + "/" + app + ".exe") &&
             QFile::exists(application_dir + "/ffmpeg/" + app + ".exe"))
    {
        //If ffmpeg is found in both root directory and subdirectory
        //use ffmpeg found in user-created "ffmpeg" subdirectory
        this->ffmpeg_path = application_dir + "/ffmpeg/" + app + ".exe";
        this->ffmpeg->setWorkingDirectory(application_dir + "/ffmpeg");
        Q_EMIT ffmpeg_status_message("Using ffmpeg found in " + (application_dir + "/ffmpeg").toUtf8(),
                                      timeout);
        set_ffmpeg_ready_status(app);
    }
    else
    {
        /*ui->ffProcWindow->setText(tr("FFmpeg executables not detected"));
        ui->statusbar->showMessage(tr("FFmpeg executables not detected"));*/
        Q_EMIT ffmpeg_status_message("FFmpeg executables not detected", timeout);
    }
}

void DetectFFmpeg::set_ffmpeg_ready_status(const QString &app)
{
    if(app == "ffmpeg")
    {
        this->ffmpeg_ready = true;
    }
    else if(app == "ffprobe")
    {
        this->ffprobe_ready = true;
    }
    else if(app == "ffplay")
    {
        this->ffplay_ready = true;
    }
    else
    {
        return;
    }
}

void DetectFFmpeg::ffmpeg_process_started()
{
    ffmpeg->waitForStarted();
    //check if ffmpeg process has started
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        //does nothing
        return;
    }
}

void DetectFFmpeg::send_ffmpeg_status()
{
    int message_timeout{0};//status bar message timeout value
#ifdef Q_OS_WIN
    if((this->ffmpeg_ready == true) &&
        (this->ffprobe_ready == true) &&
        (this->ffplay_ready == true))
    {
        Q_EMIT ffmpeg_status_message("Ready",message_timeout);
    }
#elif defined Q_OS_LINUX
    Q_EMIT ffmpeg_status_message("Ready",message_timeout);
#endif
}

void DetectFFmpeg::generate_ffmpeg_version_prompt()
{
    ffmpeg->waitForStarted();
    ffmpeg->start(ffmpeg_path, QStringList() << "-version");
}
