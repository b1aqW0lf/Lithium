#ifndef VIDEO_ENCODER_PRESET_H
#define VIDEO_ENCODER_PRESET_H

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


#include "video_encoder_preset_data.h"

#include <QWidget>


namespace Ui {
class VideoEncoderPreset;
}

class VideoEncoderPreset : public QWidget
{
    Q_OBJECT

public:
    explicit VideoEncoderPreset(QWidget *parent = nullptr);
    ~VideoEncoderPreset();

Q_SIGNALS:
    void send_statusbar_message(const QString &message, const int &timeout);

public Q_SLOTS:
    void receive_selected_video_codec_name(const QString &video_codec);

private Q_SLOTS:
    void select_encoder_preset(const int &index);
    void enable_fast_decode();
    void enable_zero_latency();

private:
    Ui::VideoEncoderPreset *ui;
    VideoEncoderPresetData preset_list;

    //functions
    void set_preset_slider_default_settings();
    void set_preset_slider_position(const QString &video_codec);
    void set_preset_slider_range(const QStringList &list);
    void set_preset_label_text(const QString &text);

    //variables
    QString video_codec{};

    //struct
    struct
    {
        QString video_preset_command{"-preset:v"};
        QStringList video_encoder_presets{};
        QStringList video_preset_selection{};
    }selection;
};

#endif // VIDEO_ENCODER_PRESET_H
