#ifndef VIDEO_AVG_BITRATE_FIELD_H
#define VIDEO_AVG_BITRATE_FIELD_H

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


#include <QAbstractButton>
#include <QAction>
#include <QWidget>


namespace Ui {
class VideoAVGBitrateField;
}

class VideoAVGBitrateField : public QWidget
{
    Q_OBJECT

public:
    explicit VideoAVGBitrateField(QWidget *parent = nullptr);
    ~VideoAVGBitrateField();

public:
    //functions
    QAbstractButton *video_avg_bitrate_bttn_widget();
    void set_avg_bitrate_button_mode();

Q_SIGNALS:
    void two_pass_encode_enabled(const bool &status);
    void average_bitrate_encode_enabled(const bool &status);
    void send_average_bitrate_value(const QString &bitrate);
    void send_avg_bitrate_statusbar_message(const QString &message, const int &timeout);//for testing

public Q_SLOTS:
    void receive_source_video_bitrate(const QString &bitrate);

private Q_SLOTS:
    void enable_average_bitrate_field();
    void get_vid_bitrate_field_data();
    void enable_two_pass_encode();
    void decrease_vid_bitrate_field_value();
    void increase_vid_bitrate_field_value();

private:
    Ui::VideoAVGBitrateField *ui;

private:
    //functions
    void setup_avg_bitrate_field_settings();
    void setup_avg_bitrate_field_validator();
    void process_source_video_bitrate(const QString &bitrate);

    //misc
    QAction *minusAction;
    QAction *plusAction;
};

#endif // VIDEO_AVG_BITRATE_FIELD_H
