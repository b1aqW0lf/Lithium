#ifndef VIDEO_UI_H
#define VIDEO_UI_H

/******************************************************************************
 Copyright (c) 2020-2022 b1aqW0lf

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/


#include "save_as_ui.h"
#include "select_source_ui.h"

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

private:
    Ui::VideoUI *ui;
    SaveAsUI SUI;
    SelectSourceUI SSUI;

private Q_SLOTS:
    void encoder_preset();
    void select_crf();
    void select_qscale();
    void select_vid_codec();
    void select_vid_res();
    void select_container();
    void select_aspect_rat();
    void select_vid_fps();
    void vid_codec_interface();
    void set_enc_profile_options();
    void select_encoder_profile();
    void select_encoder_level();
    void enable_two_pass_encode();

private:
    //processing variables
    QString preset{"-preset"};
    QString enc_preset{};
    QString enc_level{};
    QString crf_value{};
    QString pr_value{};
    QString video_codec{};
    QString video_br_value{};
    QString video_qs_value{};
    QString video_res_value{};
    QString vid_aspect_val{};
    QString video_fps_val{};
    QString video_bitrate{};
    QString output_file{};
    QString vid_ext{};
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

    //button group
    QButtonGroup *rateOptButtons;

public:
    //function
    QString get_vid_bitrate_field_text();
};

#endif // VIDEO_UI_H
