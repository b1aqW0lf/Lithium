#ifndef VIDEO_UI_H
#define VIDEO_UI_H

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


#include <QButtonGroup>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class VideoUI;
}
QT_END_NAMESPACE

class VideoUI : public QWidget
{
    Q_OBJECT

public:
    explicit VideoUI(QWidget *parent = nullptr);
    ~VideoUI();

Q_SIGNALS:
    void send_output_vid_extension(const QString &text);
    void two_pass_encode_enabled(const bool &status);
    /*void send_average_bitrate_value(const QString &value);*/

    //send current video options
    void send_current_video_options(const QString &codec, const QString &video_bitrate,
                               const QString &crf_val, const QString &qs_val,
                               const QString &resolution, const QString &dar,
                               const QString &framerate, const QString &encoder_preset);

    //for testing only!
    void send_vid_data(const QString &data, const int &timeout);
    //-----------------------------------------------------------------//

public Q_SLOTS:
    void receive_vid_source_codec(const QString &codec);
    void receive_vid_source_extension(const QString &extension);
    void receive_vid_source_resolution(const QString &display);
    //void receive_vid_source_aspect_ratio(const QString &aratio);
    void receive_vid_source_framerate(const QString &framerate);
    void receive_vid_source_bitrate(const QString &bitrate);
    void receive_vid_source_display_aspect_ratio(const QString &dar);
    void receive_clear_request();

    //experimental
    void get_selected_video_options();

private:
    Ui::VideoUI *ui;

private Q_SLOTS:
    void encoder_preset();
    void select_crf();
    void select_qscale();
    void select_vid_codec();
    void select_vid_res();
    void select_container();
    void select_dar_value();
    void select_vid_fps();
    void vid_codec_interface();
    void set_enc_profile_options();
    void select_encoder_profile();
    void select_encoder_level();
    void enable_two_pass_encode();
    void enable_average_bitrate_field();
    void enable_crf_option();
    void get_vid_bitrate_field_data();

private:
    //processing variables
    QString preset{"-preset"};
    QString encoder_preset_val{};
    QString enc_level{};
    QString crf_value{};
    QString qscale_value{};
    QString video_codec{};
    QString video_res_value{};
    QString video_dar_value{};
    QString video_fps_val{};
    QString video_bitrate{};

    //video source variables
    QString source_ext{};
    QString source_res{};
    QString source_codec{};
    QString source_fps{};
    QString source_dar{};
    //-----------------------------------

    //Interface String Lists
    QStringList videoCodecList{};
    QStringList videoContainerList{};
    QStringList videoResList{};
    QStringList videoAspecRatList{};
    QStringList videoFPSList{};
    QStringList videoEncProfileList{};
    QStringList pixel_format{"-pix_fmt", "yuv420p"};
    QStringList enc_profile{"-profile:v"};

    //functions

    //button group
    QButtonGroup *rateOptButtons;
};

#endif // VIDEO_UI_H
