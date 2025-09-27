#ifndef VIDEO_ENCODER_OPTIONS_H
#define VIDEO_ENCODER_OPTIONS_H

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


#include "encoder_level_data.h"
#include "encoder_profile_data.h"

#include <QWidget>


namespace Ui {
class VideoEncoderOptions;
}

class VideoEncoderOptions : public QWidget
{
    Q_OBJECT

public:
    explicit VideoEncoderOptions(QWidget *parent = nullptr);
    ~VideoEncoderOptions();

Q_SIGNALS:
    void send_statusbar_message(const QString &message, const int &timeout);
    void send_video_options_selections(const QStringList &options_selections);

public Q_SLOTS:
    void receive_selected_video_codec_name(const QString &video_codec);
    void receive_source_video_codec_profile(const QString &codec_profile);
    void get_video_encoder_options_selections();

private Q_SLOTS:
    void select_encoder_profile(const QString &codec_profile);
    void select_encoder_level(const QString &codec_level);

private:
    Ui::VideoEncoderOptions *ui;
    VideoEncoderLevelData level_data;
    VideoEncoderProfileData profile_data;

    //functions
    void setup_encoder_options_defaults();
    void initialize_video_encoder_options(const QString &video_codec);
    void initialize_encoder_profiles(const QString &video_codec);
    void initialize_encoder_levels(const QString &video_codec);
    void process_video_options_selections();

    //variables
    QString codec_profile{};

    struct
    {
        QString encoder_profile_command{"-profile:v"};
        QString encoder_level_command{"-level:v"};
        QStringList encoder_profile_selection{};
        QStringList encoder_level_selection{};
        QStringList video_options_selections{};
    }selection;
};

#endif // VIDEO_ENCODER_OPTIONS_H
