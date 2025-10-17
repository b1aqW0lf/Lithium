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


#include "transcode_process.h"

#include <QDir>
#include <QMessageBox>

#ifndef TIMEOUT
#define TIMEOUT 0
#endif


TranscodeProcess::TranscodeProcess(QWidget *parent)
    : QWidget{parent}
{
    connect(&encoder, &EncoderProcess::send_ffprobe_output, this, &TranscodeProcess::ffprobe_standard_output);
    connect(&encoder, &EncoderProcess::send_ffmpeg_output, this, &TranscodeProcess::ffmpeg_standard_output);
    connect(&encoder, &EncoderProcess::send_encoder_process_message, this, &TranscodeProcess::transcode_process_started);
    connect(&encoder, &EncoderProcess::send_encoder_process_exit_code, this, &TranscodeProcess::transcode_process_finished);
}

TranscodeProcess::~TranscodeProcess()
{
    //empty destructor
}

void TranscodeProcess::receive_source_file(const QString &file)
{
    //receive the source file
    this->source_file = file;
}

void TranscodeProcess::start_transcoding_process(const QStringList &input_parameters)
{
    this->start_ffprobe_process();
    this->start_ffmpeg_process(input_parameters);
}

void TranscodeProcess::start_ffprobe_process()
{
    encoder.start_encoder("ffprobe", QStringList() << "-v" << "error"
                                        << "-select_streams" << "v:0"
                                        << "-count_packets" << "-show_entries"
                                        << "stream=nb_read_packets" << "-of" << "csv=p=0"
                                        << this->source_file);
}

void TranscodeProcess::start_ffmpeg_process(const QStringList &input_parameters)
{
    QStringList arguments = QStringList() << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                                          << "-i" << this->source_file << "-sn" << input_parameters << "-preset"
                                          << "slow" /*<< "-color_primaries" << "1" << "-color_trc"
                                          << "1" << "-colorspace" << "1"*/ << "-codec:a" << "aac" << "-map_metadata" << "0"
#ifdef Q_OS_WIN
                                          << (QFileInfo(this->source_file).absoluteDir()).absolutePath()+"/testfile1.mkv";
#else
                                          << QDir::homePath()+"/Downloads/testfile1.mkv";
#endif
    encoder.start_encoder("ffmpeg", arguments);
}

void TranscodeProcess::ffprobe_standard_output(const QString &output)
{
    //receive the output of ffprobe
    Q_EMIT this->send_ffprobe_output(output);
}

void TranscodeProcess::ffmpeg_standard_output(QString &output)
{
    Q_EMIT this->send_ffmpeg_output(output);
}

void TranscodeProcess::transcode_process_started(const QString &message, const int &timeout)
{
    //send transcode message and /or duration time as message
    Q_EMIT send_transcode_process_message("Transcoding: Started" + message, timeout);
}

void TranscodeProcess::transcode_process_finished(int &exit_code)
{
    if(exit_code == QProcess::NormalExit)
    {
        Q_EMIT send_transcode_process_message("Transcoding: Finished!", TIMEOUT);
    }
    else if(exit_code == QProcess::CrashExit)
    {
        Q_EMIT send_transcode_process_message("Transcoding: Process stopped abnormally", TIMEOUT);
    }
    else
    {
        return;
    }
}

void TranscodeProcess::cancel_transcoding_process()
{
    //check for existence of the input file
    if(this->source_file.isEmpty())
    {
        encoder.stop_encoder_timer();
        Q_EMIT send_transcode_process_message(tr("No Input to cancel"), TIMEOUT);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Input file not specified"));
        return; //nothing is returned
    }
    //check if source_vid_file has data but ffmpeg is not running
    else if(!this->source_file.isEmpty())
    {
        encoder.stop_ffmpeg_process();
        Q_EMIT send_transcode_process_message(tr("Transcoding: Cancelled! "), TIMEOUT);
    }
    else
    {
        encoder.stop_ffmpeg_process();
        Q_EMIT send_transcode_process_message(tr("Transcoding: Cancelled! "), TIMEOUT);
    }
}
