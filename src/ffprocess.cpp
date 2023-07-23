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


#include "ffprocess.h"

#include <QCoreApplication>
#include <QDir>

FFprocess::FFprocess(QProcess *parent)
    : QProcess{parent}
{
    //create the processes to be used
    this->ffmpeg = new QProcess;
    this->ffplay = new QProcess;
    this->ffprobe = new QProcess;


    //connect signals and slots
    //connect(ffmpeg, SIGNAL(started()), this, SLOT(ffprocess_running()));
    connect(this->ffmpeg, &QProcess::readyReadStandardOutput,
            this, &FFprocess::ffmpegReadStandardOutput);

    //connect(ffprobe, SIGNAL(finished(int)), this, SLOT(ffprobe_finished()));
    connect(this->ffprobe, &QProcess::readyReadStandardOutput,
            this, &FFprocess::ffprobeReadStandardOutput);//ffprobe data

    connect(this->ffmpeg, &QProcess::started,
            this, &FFprocess::ffmpeg_process_started);

    connect(this->ffmpeg, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &FFprocess::ffmpeg_process_finished);

    connect(this->ffprobe, &QProcess::started,
            this, &FFprocess::ffprobe_process_started);

    connect(this->ffprobe, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &FFprocess::ffprobe_process_finished);

    //set process channel
    ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    ffplay->setProcessChannelMode(QProcess::MergedChannels);
    ffprobe->setProcessChannelMode(QProcess::MergedChannels);

    //location of ffmpeg, ffplay, and ffprobe
    ffmpeg_location_setup();
    ffprobe_location_setup();
    ffplay_location_setup();
    send_ffmpeg_status();

    //loads ffmpeg on startup
    args << "-version";
    ffmpeg->waitForStarted();
    ffmpeg->start(ffmpeg_path, args);
    args.clear();

}

FFprocess::~FFprocess()
{
    delete ffmpeg;
    delete ffprobe;
    delete ffplay;
}

void FFprocess::ffmpegReadStandardOutput()
{
    QString ffmpeg_output{};
    //ffmpeg process' readyReadStandardOutput implementation
    ffmpeg_output = this->ffmpeg->readAllStandardOutput();//single line stats
    Q_EMIT ffmpeg_read_output(ffmpeg_output);
}

void FFprocess::ffprobeReadStandardOutput()
{
    QString ffprobe_output{};
    //ffprobe process' readyReadStandardOutput implementation
    ffprobe_output = this->ffprobe->readAllStandardOutput();//single line stats
    Q_EMIT ffprobe_read_output(ffprobe_output);
}

void FFprocess::ffmpeg_location_setup()
{

#ifdef Q_OS_WIN
    int timeout{0};
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe") &&
            !QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        //Lithium root directory + ffmpeg executable
        this->ffmpeg_path = application_dir+"/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
        this->ffmpeg_ready = true;
    }
    else if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe") &&
            !QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        //Lithium root directory + "ffmpeg" sub directory + ffmpeg executable
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
        this->ffmpeg_ready = true;
    }
    else if(QFile::exists(application_dir+"/ffmpeg.exe") &&
             QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        //If ffmpeg is found in both root directory and subdirectory
        //use ffmpeg found in user-created "ffmpeg" subdirectory
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
        //ui->statusbar->showMessage(tr("Using ffmpeg found in "+(application_dir+"/ffmpeg").toUtf8()));
        Q_EMIT ffmpeg_detected_status("Using ffmpeg found in "+(application_dir+"/ffmpeg").toUtf8(),
                                      timeout);
        this->ffmpeg_ready = true;
    }
    else
    {
        /*ui->ffProcWindow->setText(tr("FFmpeg executables not detected"));
        ui->statusbar->showMessage(tr("FFmpeg executables not detected"));*/
        Q_EMIT ffmpeg_detected_status("FFmpeg executables not detected", timeout);
        this->ffmpeg_ready = false;
    }
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif

}

void FFprocess::ffprobe_location_setup()
{

#ifdef Q_OS_WIN
    int timeout{0};
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffprobe.exe") &&
            !QFile::exists(application_dir+"/ffmpeg/ffprobe.exe"))
    {
        //Lithium root directory + ffprobe executable
        this->ffprobe_path = application_dir+"/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir);
        this->ffprobe_ready = true;
    }
    else if(QFile::exists(application_dir+"/ffmpeg/ffprobe.exe") &&
            !QFile::exists(application_dir+"/ffprobe.exe"))
    {
        //Lithium root directory + "ffmpeg" sub directory + ffprobe executable
        this->ffprobe_path = application_dir+"/ffmpeg/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir+"/ffmpeg");
        this->ffprobe_ready = true;
    }
    else if(QFile::exists(application_dir+"/ffprobe.exe") &&
             QFile::exists(application_dir+"/ffmpeg/ffprobe.exe"))
    {
        //If ffprobe is found in both root directory and subdirectory
        //use ffprobe found in user-created "ffmpeg" subdirectory
        this->ffprobe_path = application_dir+"/ffmpeg/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir+"/ffmpeg");
        //ui->statusbar->showMessage(tr("Using ffprobe found in "+(application_dir+"/ffmpeg").toUtf8()));
        Q_EMIT ffprobe_detected_status("Using ffprobe found in "+(application_dir+"/ffmpeg").toUtf8(),
                                       timeout);
        this->ffprobe_ready = true;
    }
    else
    {
        /*ui->ffProcWindow->setText(tr("FFprobe executable not detected"));
        ui->statusbar->showMessage(tr("FFprobe executable not detected"));*/
        Q_EMIT ffprobe_detected_status("FFprobe executable not detected", timeout);
        this->ffprobe_ready = false;
    }
#elif defined Q_OS_LINUX
    this->ffprobe_path = "ffprobe";
#endif

}

void FFprocess::ffplay_location_setup()
{

#ifdef Q_OS_WIN
    int timeout{0};
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffplay.exe") &&
            !QFile::exists(application_dir+"/ffmpeg/ffplay.exe"))
    {
        //Lithium root directory + ffplay executable
        this->ffplay_path = application_dir+"/ffplay.exe";
        this->ffplay->setWorkingDirectory(application_dir);
        this->ffplay_ready = true;
    }
    else if(QFile::exists(application_dir+"/ffmpeg/ffplay.exe") &&
            !QFile::exists(application_dir+"/ffplay.exe"))
    {
        //Lithium root directory + "ffmpeg" sub directory + ffplay executable
        this->ffplay_path = application_dir+"/ffmpeg/ffplay.exe";
        this->ffplay->setWorkingDirectory(application_dir+"/ffmpeg");
        this->ffplay_ready = true;
    }
    else if(QFile::exists(application_dir+"/ffplay.exe") &&
             QFile::exists(application_dir+"/ffmpeg/ffplay.exe"))
    {
        //If ffplay is found in both root directory and subdirectory
        //use ffplay found in user-created "ffmpeg" subdirectory
        this->ffplay_path = application_dir+"/ffmpeg/ffplay.exe";
        this->ffplay->setWorkingDirectory(application_dir+"/ffmpeg");
        //ui->statusbar->showMessage(tr("Using ffplay found in "+(application_dir+"/ffmpeg").toUtf8()));
        Q_EMIT ffplay_detected_status("Using ffplay found in "+(application_dir+"/ffmpeg").toUtf8(),
                                      timeout);
        this->ffplay_ready = true;
    }
    else
    {
        /*ui->ffProcWindow->setText(tr("FFplay executable not detected"));
        ui->statusbar->showMessage(tr("FFplay executable not detected"));*/
        Q_EMIT ffplay_detected_status("FFplay executable not detected", timeout);
        this->ffplay_ready = false;
    }
#elif defined Q_OS_LINUX
    this->ffplay_path = "ffplay";
#endif

}

void FFprocess::ffmpeg_process_started()
{
    ffmpeg->waitForStarted();
    //check if ffmpeg process has started
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        Q_EMIT ffmpeg_started();
        send_ffmpeg_status();
    }
}

void FFprocess::ffmpeg_process_finished()
{
    ffmpeg->waitForFinished();
    if(this->ffmpeg->QProcess::state() == QProcess::NotRunning)
    {
        Q_EMIT ffmpeg_finished();

        //close write channel after process finishes
        ffmpeg->closeWriteChannel();
    }
}

void FFprocess::ffprobe_process_started()
{
    ffprobe->waitForStarted();
    //check if ffmpeg process has started
    if(this->ffprobe->QProcess::state() == QProcess::Running)
    {
        Q_EMIT ffprobe_started();
    }
}

void FFprocess::ffprobe_process_finished()
{
    ffprobe->waitForFinished();
    if(this->ffprobe->QProcess::state() == QProcess::NotRunning)
    {
        Q_EMIT ffprobe_finished();

        //close write channel after process finishes
        ffprobe->closeWriteChannel();
    }
}

void FFprocess::send_ffmpeg_status()
{
    int message_timeout{0};//status bar message timeout value
#ifdef Q_OS_WIN
    if((this->ffmpeg_ready == true) &&
        (this->ffprobe_ready == true) &&
        (this->ffplay_ready == true))
    {
        Q_EMIT ffmpeg_ready_status("Ready",message_timeout);
    }
#elif defined Q_OS_LINUX
    Q_EMIT ffmpeg_ready_status("Ready",message_timeout);
#endif
}
