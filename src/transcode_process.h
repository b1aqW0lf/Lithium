#ifndef TRANSCODE_PROCESS_H
#define TRANSCODE_PROCESS_H


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

#include <QProcess>
#include <QWidget>


class TranscodeProcess : public QWidget
{
    Q_OBJECT

public:
    explicit TranscodeProcess(QWidget *parent = nullptr);
    ~TranscodeProcess();

Q_SIGNALS:
    void send_ffprobe_output(const QString &nb_frames);
    void send_ffmpeg_output(QString &output);
    void send_transcode_process_message(const QString &message, const int &timeout);

public Q_SLOTS:
    void ffprobe_standard_output(const QString &output);//new
    void ffmpeg_standard_output(QString &output);//new
    void receive_source_file(const QString &file);
    void start_transcoding_process();
    void cancel_transcoding_process();
    void transcode_process_started(const QString &message, const int &timeout);//new
    void transcode_process_finished(int &exit_code);

private:
    EncoderProcess encoder;

    //functions
    void set_encoder_path(const QString &encoder);
    void start_ffprobe_process();
    void start_ffmpeg_process();

    //variables
    QString source_file{};
};

#endif // TRANSCODE_PROCESS_H
