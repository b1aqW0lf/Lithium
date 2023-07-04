#ifndef TRANSCODE_H
#define TRANSCODE_H


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

#include <QObject>
#include <QWidget>


class Transcode : public QWidget
{
    Q_OBJECT

    friend class FFprocess;

public:
    explicit Transcode(QWidget *parent = nullptr);
    ~Transcode();

Q_SIGNALS:
    void source_vid_file_status(const QString &status, const int &timeout);
    void output_vid_file_status(const QString &status, const int &timeout);
    bool encode_button_set_checked(const bool &checked);

public Q_SLOTS:
    void receive_source_video_file(const QString &source_video);
    void receive_output_video_path(const QString &output_path);
    //void verify_output_video_file(const QString &output_video);
    void enable_two_pass_encode(const bool &status);
    void receive_vid_avg_bitrate(const QString &vid_avg_bitrate);
    void receive_video_crf_val(const QString &crf_val);
    void receive_video_qscale_val(const QString &qs_val);
    void receive_video_codec_name(const QString &codec);
    void receive_video_res_value(const QString &value);
    void start_normal_mode_transcode();

private:
    FFprocess process;

    //functions
    void source_input_file_check();
    void output_video_path_check();
    void normal_mode_transcode();

    //processing variables
    QString source_vid_file{};
    QString output_vid_file{};
    bool two_pass_val{};
    QString vid_avg_bitrate{};

    //transcoder variables
    QString video_codec{};
    QString crf_value{};
    QString qscale_value{};
    QString video_res{};

};

#endif // TRANSCODE_H
