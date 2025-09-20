#ifndef VIDEO_CRF_INTERFACE_H
#define VIDEO_CRF_INTERFACE_H

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


#include "video_crf_commands.h"

#include <QAbstractButton>
#include <QWidget>

namespace Ui {
class VideoCRFInterface;
}

class VideoCRFInterface : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCRFInterface(QWidget *parent = nullptr);
    ~VideoCRFInterface();

public:
    //function
    QAbstractButton *video_crf_button_widget();
    void set_crf_button_mode();

Q_SIGNALS:
    void send_video_crf_qscale_selection(const QStringList &crf_qscale_selection);
    void send_statusbar_message(const QString &message, const int &timeout);

public Q_SLOTS:
    void receive_selected_video_codec_name(const QString &video_codec);
    void get_video_crf_qscale_selection();

private Q_SLOTS:
    void select_encoder_rate_factor(const int &index);
    void enable_crf_interface();

private:
    Ui::VideoCRFInterface *ui;
    VideoCRFCommands command;

    //functions
    void set_crf_slider_options(const QString &video_codec);
    void set_crf_slider_interface(const QString &video_codec);
    void process_video_crf_qscale_selection();

    //struct
    struct
    {
        QStringList video_crf_selection{};
        //--------------------------------------
        QStringList video_crf_selection_list{};
    }selection;
};

#endif // VIDEO_CRF_INTERFACE_H
