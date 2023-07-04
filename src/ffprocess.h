#ifndef FFPROCESS_H
#define FFPROCESS_H


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


#include <QProcess>
#include <QWidget>


class FFprocess : public QProcess
{
    Q_OBJECT

    friend class Transcode;
    friend class InputSourceProbe;

public:
    explicit FFprocess(QProcess *parent = nullptr);
    ~FFprocess();

public:
    //ffmpeg executables as processes
    QProcess *ffmpeg; //transcodes, encodes, records, and streams files
    QProcess *ffprobe;//examines input and output files
    QProcess *ffplay; //play input and output files

Q_SIGNALS:
    void ffmpeg_read_output(const QString &output);
    void ffprobe_read_output(const QString &output);
    void ffmpeg_found(const QString &status);
    void ffprobe_found(const QString &status);
    void ffplay_found(const QString &status);
    void ffmpeg_ready_status(const QString &status, const int &stat);
    void ffmpeg_started();
    void ffprobe_started();
    void ffmpeg_finished();
    void ffprobe_finished();

    //not impelemented yet
    /*void ffmpeg_cancelled();
    void ffprobe_cancelled();*/

private Q_SLOTS:
    void ffmpegReadStandardOutput();//ffmpeg QProcess function
    void ffprobeReadStandardOutput();//ffprobe QProcess function
    void ffmpeg_process_started();
    void ffmpeg_process_finished();
    void ffprobe_process_started();
    void ffprobe_process_finished();

    //not impelemented yet
    /*void ffmpeg_process_cancelled();
    void ffprobe_process_cancelled();*/

private:
    //ffmpeg status functions
    void ffmpeg_location_setup();
    void ffprobe_location_setup();
    void ffplay_location_setup();

    //function
    void send_ffmpeg_status();

    //ffplay output strings
    QString ffplay_output{};

    //ffmpeg argument
    QStringList args{};

    //ffmpeg file path -> path to ffmpeg
    QString ffmpeg_path{};
    QString ffprobe_path{};
    QString ffplay_path{};
};

#endif // FFPROCESS_H
