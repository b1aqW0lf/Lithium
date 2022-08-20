/******************************************************************************
 Copyright (c) 2020-2022 b1aqW0lf
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


#include "ui_save_as_ui.h"
#include "ui_select_source_ui.h"
#include "ui_video_ui.h"
#include "video_ui.h"

#include <QComboBox>
#include <QDial>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>


VideoUI::VideoUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->videoEncoderDial, &QDial::valueChanged,
            this, &VideoUI::encoder_preset);
    connect(ui->videoRFSlider, &QSlider::valueChanged,
            this, &VideoUI::select_crf);
    connect(ui->videoRFSlider, &QSlider::valueChanged,
            this, &VideoUI::select_qscale);
    connect(ui->videoRFSlider, &QSlider::valueChanged,
            ui->videoRFSpinBox, &QSpinBox::setValue);
    //-------------------------------------------------------------------------
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_vid_codec);
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::vid_codec_interface);
    connect(ui->videoContainerBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_container);/**/
    connect(ui->videoResBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_vid_res);
    connect(ui->videoAspectRatBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_aspect_rat);
    connect(ui->videoFPSBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_vid_fps);
    //-------------------------------------------------------------------------
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::set_enc_profile_options);
    connect(ui->videoEncProfileBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_encoder_profile);
    connect(ui->videoEncLevelSlider, &QSlider::valueChanged,
            this, &VideoUI::select_encoder_level);
    //-------------------------------------------------------------------------
    connect(ui->twoPassCheckBox, &QCheckBox::clicked,
            this, &VideoUI::enable_two_pass_encode);
    //-------------------------------------------------------------------------


    //Initialize interface widgets
    //additional radio button defaults
    rateOptButtons = new QButtonGroup(this);
    rateOptButtons->addButton(ui->videoCRFRadio);
    rateOptButtons->addButton(ui->videoAVGBitRadio);
    rateOptButtons->setExclusive(true);
    ui->videoCRFRadio->setChecked(true);

    //constant rf spinbox settings
    ui->videoRFSpinBox->setAlignment(Qt::AlignHCenter);
    ui->videoRFSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->videoRFSpinBox->setFrame(false);
    ui->videoRFSpinBox->setRange(0, 51);
    ui->videoRFSpinBox->setValue(23); //default value for the spinBox
    ui->videoRFSpinBox->setReadOnly(true);

    //slider
    ui->videoRFSlider->setRange(0, 51);
    ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
    ui->videoRFSlider->setSingleStep(1);
    ui->videoRFSlider->setValue(23);//default value for the slider
    ui->videoRFSlider->setSliderPosition(23);
    ui->videoRFSlider->setToolTip(tr("Constant Rate Factor Selector"));
    crf_value.setNum(ui->videoRFSlider->value());//send default to encoder    
    ui->videoLQLabel->setAlignment(Qt::AlignTop);
    ui->videoHQLabel->setAlignment(Qt::AlignTop);

    //encoder dial
    ui->videoDialPreset->setAlignment(Qt::AlignCenter);
    ui->videoEncoderDial->setRange(0, 9);
    ui->videoEncoderDial->setNotchesVisible(true);
    ui->videoEncoderDial->setSingleStep(1);
    ui->videoEncoderDial->setValue(4);//setting default value
    ui->videoEncoderDial->setSliderPosition(4);
    ui->videoEncoderDial->setToolTip(tr("default: medium encoding"));
    ui->videoDialPreset->setText(tr("medium"));//default value
    pr_value = "medium";//send default to encoder
    //ui->videoEncoderDial->setInvertedAppearance(true);

    //video codec interface
    ui->videoCodecBox->insertItem(0, "Source");
    ui->videoCodecBox->insertSeparator(1);
    videoCodecList << "x264" << "x264 10-bit" << "x265" << "x265 10-bit"
                   << "x265 12-bit" << "Xvid" << "VP9" << "Theora" << "MPEG-1"
                   << "MPEG-2" << "RAV1E/AV1";
    ui->videoCodecBox->insertItems(2, videoCodecList);

    //video container
    ui->videoContainerBox->insertItem(0, "Source");
    ui->videoContainerBox->insertSeparator(1);
    videoContainerList << "MP4" << "MKV" << "WebM" << "TS" << "OGV";
    ui->videoContainerBox->insertItems(2, videoContainerList);

    //video resolution
    ui->videoResBox->insertItem(0, "Source");
    ui->videoResBox->insertSeparator(1);
    videoResList << "640x360" << "720x480" << "720x576" << "1024x768"
                 << "1280x720" << "1366x768" << "1600x900" << "1920x1080"
                 << "1920x1200" << "2048x1080" << "2048x1152" << "2560x1440"
                 << "3840x2160" << "4096x2160";
    ui->videoResBox->insertItems(2, videoResList);

    //video aspect ratio
    ui->videoAspectRatBox->insertItem(0, "Source");
    ui->videoAspectRatBox->insertSeparator(1);
    videoAspecRatList << "4:3" << "16:9" << "21:9";
    ui->videoAspectRatBox->insertItems(2, videoAspecRatList);

    //video framerate
    ui->videoFPSBox->insertItem(0, "Source");
    ui->videoFPSBox->insertSeparator(1);
    videoFPSList << "23.976" << "24" << "25" << "29.97" << "30" << "60" << "120";
    ui->videoFPSBox->insertItems(2, videoFPSList);
    //ui->videoFPSBox->setCurrentIndex(5);--->test, it works!

    //video encoder profile
    /*ui->videoEncProfileBox->insertItem(0, "Auto");
    ui->videoEncProfileBox->insertSeparator(1);
    videoEncProfileList << "Baseline" << "Main" << "High";
    ui->videoEncProfileBox->insertItems(2, videoEncProfileList);
    ui->videoEncProfileBox->setToolTip(tr("Default = Auto"));*/

    //video average bitrate
    ui->videoAVGBitRadio->setText(tr("Avg Bitrate (Kbps)"));
    ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
    /*ui->videoAVGBitField->setFixedHeight(25);*/
    ui->videoAVGBitField->setFixedWidth(115);
    ui->videoAVGBitField->setPlaceholderText("10000");

    //video groupbox
    ui->videoGroupBox->setTitle(tr("Video"));
    ui->videoGroupBox->setAlignment(Qt::AlignLeft);

    //video endoder level
    ui->videoEncLevelLabel->setText(tr("Encoder Level"));
    ui->videoEncLevelSlider->setRange(0, 20);
    ui->videoEncLevelSlider->setTickPosition(QSlider::TicksBelow);
    ui->videoEncLevelSlider->setSingleStep(1);
    ui->videoEncLevelSlider->setValue(0);//default value for the encoderlevel slider
    ui->videoEncLevelSlider->setSliderPosition(0);
    ui->videoEncLevelSlider->setToolTip(tr("Encoder Level Selector"));
    ui->videoEncLevelDisplay->setAlignment(Qt::AlignCenter);
    ui->videoEncLevelDisplay->setText(tr("auto")); //default value for encoder level
    ui->videoEncLevelDisplay->setFixedWidth(35);
    ui->videoEncLevelDisplay->setFixedHeight(35);
    ui->videoEncLevelDisplay->setStyleSheet("QLabel { background-color : white }");
    ui->videoEncLevelDisplay->setStyleSheet("QLabel { border : 0.5px solid black }");

    //2-pass encoding checkbox
    ui->twoPassCheckBox->setToolTip(tr("Two-pass encoding"));
    ui->twoPassCheckBox->setText(tr("2-Pass Encoding"));

}

VideoUI::~VideoUI()
{
    delete ui;
}

//ui->videoEncoderDial->setRange(0, 9);
void VideoUI::encoder_preset()
{
    if(ui->videoEncoderDial->value() == 0 &&
            ui->videoEncoderDial->sliderPosition() == 0)
    {
        //test for AV1 codec/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 0"));
            pr_value = "0";
            ui->videoEncoderDial->setToolTip(tr("Speed 0 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("placebo"));
            pr_value = "placebo";
            ui->videoEncoderDial->setToolTip(tr("placebo encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 1 &&
            ui->videoEncoderDial->sliderPosition() == 1)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 1"));
            pr_value = "1";
            ui->videoEncoderDial->setToolTip(tr("Speed 1 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("veryslow"));
            pr_value = "veryslow";
            ui->videoEncoderDial->setToolTip(tr("veryslow encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 2 &&
            ui->videoEncoderDial->sliderPosition() == 2)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 2"));
            pr_value = "2";
            ui->videoEncoderDial->setToolTip(tr("Speed 2 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("slower"));
            pr_value = "slower";
            ui->videoEncoderDial->setToolTip(tr("slower encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 3 &&
            ui->videoEncoderDial->sliderPosition() == 3)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 3"));
            pr_value = "3";
            ui->videoEncoderDial->setToolTip(tr("Speed 3 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("slow"));
            pr_value = "slow";
            ui->videoEncoderDial->setToolTip(tr("slow encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 4 &&
            ui->videoEncoderDial->sliderPosition() == 4)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 4"));
            pr_value = "4";
            ui->videoEncoderDial->setToolTip(tr("Speed 4 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("medium"));
            pr_value = "medium";
            ui->videoEncoderDial->setToolTip(tr("medium encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 5 &&
            ui->videoEncoderDial->sliderPosition() == 5)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 5"));
            pr_value = "5";
            ui->videoEncoderDial->setToolTip(tr("Speed 5 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("fast"));
            pr_value = "fast";
            ui->videoEncoderDial->setToolTip(tr("fast encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 6 &&
            ui->videoEncoderDial->sliderPosition() == 6)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 6"));
            pr_value = "6";
            ui->videoEncoderDial->setToolTip(tr("Speed 6 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("faster"));
            pr_value = "faster";
            ui->videoEncoderDial->setToolTip(tr("faster encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 7 &&
            ui->videoEncoderDial->sliderPosition() == 7)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 7"));
            pr_value = "7";
            ui->videoEncoderDial->setToolTip(tr("Speed 7 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("veryfast"));
            pr_value = "veryfast";
            ui->videoEncoderDial->setToolTip(tr("veryfast encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 8 &&
            ui->videoEncoderDial->sliderPosition() == 8)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 8"));
            pr_value = "8";
            ui->videoEncoderDial->setToolTip(tr("Speed 8 Encoding"));
        }
        //any other codec
        else
        {
            ui->videoDialPreset->setText(tr("superfast"));
            pr_value = "superfast";
            ui->videoEncoderDial->setToolTip(tr("superfast encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 9 &&
            ui->videoEncoderDial->sliderPosition() == 9)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 9"));
            pr_value = "9";
            ui->videoEncoderDial->setToolTip(tr("Speed 9 Encoding"));
        }
        //any other codec
        else
        {
            ui->videoDialPreset->setText(tr("ultrafast"));
            pr_value = "ultrafast";
            ui->videoEncoderDial->setToolTip(tr("ultrafast encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 10 &&
            ui->videoEncoderDial->sliderPosition() == 10)//<==Speed 10 does not appear in application
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 10"));
            pr_value = "10";
            ui->videoEncoderDial->setToolTip(tr("Speed 10 Encoding"));
        }
        else
        {
            ui->videoEncoderDial->setRange(0, 9);
        }
    }
    //test, it works!
    //QString str = "";
    //ui->videoDialPreset->setText("placebo " +
    //str.setNum(ui->videoEncoderDial->value()));
}

//initalizing selected crf value
void VideoUI::select_crf()
{
    //crf_value is part of input for ffmpeg args in encoding_started()
    //setting the crf value to string
    crf_value.setNum(ui->videoRFSlider->value());
}

//initalizing selected qscale value
void VideoUI::select_qscale()
{
    //setting qscale value to string
    video_qs_value.setNum(ui->videoRFSlider->value());
}

void VideoUI::select_vid_codec()
{
    //select video codec
    if(ui->videoCodecBox->currentIndex() == 0)
    {
        //copy
        video_codec = "copy";
    }
    if(ui->videoCodecBox->currentIndex() == 2)
    {
        //x264
        video_codec = "libx264";
    }
    if(ui->videoCodecBox->currentIndex() == 3)
    {
        //x264 10bit
        video_codec = "libx264";
        pixel_format[1] = "yuv420p10le";
    }
    if(ui->videoCodecBox->currentIndex() == 4)
    {
        //x265
        video_codec = "libx265";
    }
    if(ui->videoCodecBox->currentIndex() == 5)
    {
        //x265 10bit
        video_codec = "libx265";
        pixel_format[1] = "yuv444p10le";
    }
    if(ui->videoCodecBox->currentIndex() == 6)
    {
        //x265 12bit
        video_codec = "libx265";
        pixel_format[1] = "yuv444p12le";
    }
    if(ui->videoCodecBox->currentIndex() == 7)
    {
        //Xvid
        video_codec = "libxvid";
    }
    if(ui->videoCodecBox->currentIndex() == 8)
    {
        //vp9
        video_codec = "libvpx-vp9";
    }
    if(ui->videoCodecBox->currentIndex() == 9)
    {
        //theora
        video_codec = "libtheora";
    }
    if(ui->videoCodecBox->currentIndex() == 10)
    {
        //MPEG-1
        video_codec = "mpeg1video";
    }
    if(ui->videoCodecBox->currentIndex() == 11)
    {
        //MPEG-2
        video_codec = "mpeg2video";
    }
    if(ui->videoCodecBox->currentIndex() == 12)
    {
        //rav1e
        video_codec = "librav1e";
    }
    /*else
    {
        video_codec = ui->videoCodecBox->currentText().toLower();
    }*/
}

void VideoUI::vid_codec_interface()
{
    //video codec interface settings
    if(ui->videoCodecBox->currentIndex() == 2 ||
            ui->videoCodecBox->currentIndex() == 3)//x264 10bit
    {
        //default libx264 crf value is 23
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setRange(0, 51);
        //x264
        if(ui->videoCodecBox->currentIndex() == 2)
        {
            ui->videoRFSlider->setValue(23);
            ui->videoRFSpinBox->setValue(23);
            ui->videoRFSlider->setSliderPosition(23);
        }
        //x264 10bit
        if(ui->videoCodecBox->currentIndex() == 3)
        {
            ui->videoRFSlider->setValue(20);
            ui->videoRFSpinBox->setValue(20);
            ui->videoRFSlider->setSliderPosition(20);
        }
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setRange(0, 9);
        ui->videoEncLevelSlider->setRange(0,20);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
    else if(ui->videoCodecBox->currentIndex() == 4 ||
            ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
            ui->videoCodecBox->currentIndex() == 6) //x265 12bit
    {
        //default libx265 crf value is 28
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setRange(0, 51);
        if(ui->videoCodecBox->currentIndex() == 4)
        {
            ui->videoRFSlider->setValue(28);
            ui->videoRFSpinBox->setValue(28);
            ui->videoRFSlider->setSliderPosition(28);
        }
        if(ui->videoCodecBox->currentIndex() == 5 ||//x265 10bit
           ui->videoCodecBox->currentIndex() == 6) //x265 12bit
        {
            ui->videoRFSlider->setValue(20);
            ui->videoRFSpinBox->setValue(20);
            ui->videoRFSlider->setSliderPosition(20);
        }
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setRange(0, 9);
        ui->videoEncLevelSlider->setRange(0,13);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
    else if(ui->videoCodecBox->currentIndex() == 7)
    {
        //default Xvid settings
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(1, 31);
        ui->videoRFSlider->setRange(1, 31);
        ui->videoRFSlider->setValue(12);
        ui->videoRFSpinBox->setValue(12);
        ui->videoRFSlider->setSliderPosition(12);
        ui->videoCRFRadio->setText(tr("Variable Bit Rate"));
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setRange(0, 9);
        video_qs_value.setNum(ui->videoRFSlider->value());//setting qscale value
        ui->videoEncLevelSlider->setRange(0,20);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
    else if(ui->videoCodecBox->currentIndex() == 8)
    {
        //default libvpx-vp9 crf value is 31, range is 0-63
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRFSlider->setRange(0, 63);
        ui->videoRFSlider->setValue(31);
        ui->videoRFSpinBox->setValue(31);
        ui->videoRFSlider->setSliderPosition(31);
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setRange(0, 9);
        video_br_value = "0";
        ui->videoEncLevelSlider->setRange(0, 14);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
    else if(ui->videoCodecBox->currentIndex() == 9)
    {
        //default libtheora settings, range is 0-10, 10 is higest quality
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(0, 10);
        ui->videoRFSlider->setRange(0, 10);
        ui->videoRFSlider->setValue(7);
        ui->videoRFSpinBox->setValue(7);
        ui->videoRFSlider->setSliderPosition(7);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoCRFRadio->setText(tr("Variable Bit Rate"));
        ui->videoLQLabel->setText(tr("High Quality |"));
        ui->videoHQLabel->setText(tr("| Low Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setRange(0, 9);
        video_qs_value.setNum(ui->videoRFSlider->value());//setting qscale value
        ui->videoEncLevelSlider->setRange(0,20);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
    else if(ui->videoCodecBox->currentIndex() == 10 ||
            ui->videoCodecBox->currentIndex() == 11)
    {
        //default mpeg1 and mpeg2 settimgs, range is 1-31, 1 is highest
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(1, 31);
        ui->videoRFSlider->setRange(1, 31);
        ui->videoRFSlider->setValue(5);
        ui->videoRFSpinBox->setValue(5);
        ui->videoRFSlider->setSliderPosition(5);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoCRFRadio->setText(tr("Quality Rate Factor"));
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setRange(0, 9);
        video_qs_value.setNum(ui->videoRFSlider->value());//setting qscale value
        ui->videoEncLevelSlider->setRange(0,20);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
    else if(ui->videoCodecBox->currentIndex() == 12)
    {
        //default rav1e settings, qp range is 0-255, 0 is highest
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(0, 255);
        ui->videoRFSlider->setRange(0, 255);
        ui->videoEncoderDial->setRange(0, 10);
        ui->videoRFSlider->setValue(100);
        ui->videoRFSpinBox->setValue(100);
        ui->videoRFSlider->setSliderPosition(100);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoCRFRadio->setText(tr("Quantizer Factor"));
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setToolTip(tr("default: Speed 4 Encoding"));
        ui->videoDialPreset->setText(tr("Speed 4"));//default value
        video_qs_value.setNum(ui->videoRFSlider->value());//setting qp value
        ui->videoEncLevelSlider->setRange(0,20);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
    else
    {
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoRFSlider->setRange(0, 51);
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setValue(23);
        ui->videoRFSpinBox->setValue(23);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoRFSlider->setSliderPosition(23);
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setRange(0, 9);
        ui->videoEncoderDial->setToolTip(tr("default: medium encoding"));
        ui->videoDialPreset->setText(tr("medium"));//default value
        ui->videoEncLevelSlider->setRange(0,20);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
}

void VideoUI::select_container()//fixed!!!
{
    /*connect(&SUI, &SaveAsUI::saveas_value,
            this, &VideoUI::select_container);//step closer!
    output_file = SUI.ui->saveASEdit->text();
    //based on code from qtffmpeg
    output_file = SUI.ui->saveASEdit->text();//get the current text on the field
    output_file = output_file.left(output_file.lastIndexOf("."));*/

    if(ui->videoContainerBox->currentIndex() == 0)
    {
        //copy source file extension
        QString source_ext{};
        QString input_file1_str = SSUI.ui->sourceInput1Edit->text();
        source_ext = input_file1_str.mid(input_file1_str.lastIndexOf("."));
        output_file.append(source_ext);
        SUI.ui->saveASEdit->setText(output_file);
        vid_ext = source_ext;
    }
    if(ui->videoContainerBox->currentIndex() == 2)
    {
        //mp4
        output_file.append(".mp4");
        SUI.ui->saveASEdit->setText(output_file);
        vid_ext = "mp4";
    }//gui not changing based on selection above. you made changes to SaveASUI and SelectSourceUI
    /*if(ui->videoContainerBox->currentIndex() == 3)
    {
        //mkv
        output_file.append(".mkv");
        ui->saveASEdit->setText(output_file);
        vid_ext = "mkv";
    }
    if(ui->videoContainerBox->currentIndex() == 4)
    {
        //webm
        output_file.append(".webm");
        ui->saveASEdit->setText(output_file);
        vid_ext = "webm";
    }
    if(ui->videoContainerBox->currentIndex() == 5)
    {
        //ts
        output_file.append(".ts");
        ui->saveASEdit->setText(output_file);
        vid_ext = "ts";
    }
    if(ui->videoContainerBox->currentIndex() == 6)
    {
        //ogv
        output_file.append(".ogv");
        ui->saveASEdit->setText(output_file);
        vid_ext = "ogv";
    }*/
}

void VideoUI::select_vid_res()
{
    //select video resolution
    if(ui->videoResBox->currentIndex() == 0)//it works!
    {
        //input1_vid_res holds the default res value for input_file1
        video_res_value = "copy";
    }
    else
    {
        video_res_value = "scale="+ui->videoResBox->currentText();
    }
}

void VideoUI::select_aspect_rat()
{
    if(ui->videoAspectRatBox->currentIndex() == 0)
    {
        vid_aspect_val = "copy";
    }
    else
    {
        vid_aspect_val = ui->videoAspectRatBox->currentText();
    }
}

void VideoUI::select_vid_fps()
{
    if(ui->videoFPSBox->currentIndex() == 0)
    {
        video_fps_val = "copy";
    }
    else
    {
        video_fps_val = ui->videoFPSBox->currentText();
    }
}

//creating options for encoder profile combobox
void VideoUI::set_enc_profile_options()
{
    //x264 implementation
    if(ui->videoCodecBox->currentIndex() == 2)
    {
        ui->videoEncProfileBox->clear();
        videoEncProfileList.clear();
        videoEncProfileList << "Auto" << "Baseline" << "Main" << "High";
        ui->videoEncProfileBox->insertItems(0, videoEncProfileList);
    }
    //x264 10 bit implementation
    if(ui->videoCodecBox->currentIndex() == 3)
    {
        ui->videoEncProfileBox->clear();
        videoEncProfileList.clear();
        videoEncProfileList << "Auto" << "High 10";
        ui->videoEncProfileBox->insertItems(0, videoEncProfileList);
    }
    //x265 implementation
    if(ui->videoCodecBox->currentIndex() == 4)
    {
        ui->videoEncProfileBox->clear();
        videoEncProfileList.clear();
        videoEncProfileList << "Auto" << "Main" << "Main 4:4:4"
                            << "Main Still Picture";
        ui->videoEncProfileBox->insertItems(0, videoEncProfileList);
    }
    //xvid implementation
    if(ui->videoCodecBox->currentIndex() == 7)
    {
        ui->videoEncProfileBox->clear();
        videoEncProfileList.clear();
        videoEncProfileList << "Auto" << "1" << "2" << "3" << "4"
                            << "5" << "6" <<"7" << "8" << "9";
        ui->videoEncProfileBox->insertItems(0, videoEncProfileList);
    }
    //vp9 implementation
    if(ui->videoCodecBox->currentIndex() == 8)
    {
        ui->videoEncProfileBox->clear();
        videoEncProfileList.clear();
        videoEncProfileList << "0" << "1" << "2" << "3";
        ui->videoEncProfileBox->insertItems(0, videoEncProfileList);
    }
}

//select encoder profile
void VideoUI::select_encoder_profile()
{
    //default: auto - default in FFmpeg
    //ffmpeg suggests omitting encoder profile option to allow encoder to choose
    if(ui->videoEncProfileBox->currentIndex() > 0)
    {
        //enc_profile index[0] is -profile:v
        enc_profile[1] = ui->videoEncProfileBox->currentText().toLower().remove(" ");
    }
}

//initializing selected encoder level
void VideoUI::select_encoder_level()
{
    //encoder level settings
    if(ui->videoEncLevelSlider->value() == 0 &&
            ui->videoEncLevelSlider->sliderPosition() == 0)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
           ui->videoCodecBox->currentIndex() == 3)//x264 10bit
        {
            //default auto setting for ffmpeg
            ui->videoEncLevelDisplay->setText("auto");
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit
        {
            //default auto setting for ffmpeg
            ui->videoEncLevelDisplay->setText("auto");
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            //default auto setting for ffmpeg
            ui->videoEncLevelDisplay->setText("auto");
        }
        else
        {
            //every other codec
            //default auto setting for ffmpeg
            ui->videoEncLevelDisplay->setText("auto");
        }
    }
    if(ui->videoEncLevelSlider->value() == 1 &&
            ui->videoEncLevelSlider->sliderPosition() == 1)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("1.0");
            enc_level = "1.0";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("1.0");
            enc_level = "1.0";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("1.0");
            enc_level = "1.0";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("1.0");
            enc_level = "1.0";
        }
    }
    if(ui->videoEncLevelSlider->value() == 2 &&
            ui->videoEncLevelSlider->sliderPosition() == 2)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("1b");
            enc_level = "1b";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("2.0");
            enc_level = "2.0";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("1.1");
            enc_level = "1.1";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("1b");
            enc_level = "1b";
        }
    }
    if(ui->videoEncLevelSlider->value() == 3 &&
            ui->videoEncLevelSlider->sliderPosition() == 3)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("1.1");
            enc_level = "1.1";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("2.1");
            enc_level = "2.1";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("2.0");
            enc_level = "2.0";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("1.1");
            enc_level = "1.1";
        }
    }
    if(ui->videoEncLevelSlider->value() == 4 &&
            ui->videoEncLevelSlider->sliderPosition() == 4)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("1.2");
            enc_level = "1.2";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("3.0");
            enc_level = "3";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("2.1");
            enc_level = "2.1";
        }
        else
        {
            ui->videoEncLevelDisplay->setText("1.2");
            enc_level = "1.2";
        }
    }
    if(ui->videoEncLevelSlider->value() == 5 &&
            ui->videoEncLevelSlider->sliderPosition() == 5)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("1.3");
            enc_level = "1.3";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("3.1");
            enc_level = "3.1";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("3.0");
            enc_level = "3.0";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("1.3");
            enc_level = "1.3";
        }
    }
    if(ui->videoEncLevelSlider->value() == 6 &&
            ui->videoEncLevelSlider->sliderPosition() == 6)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("2.0");
            enc_level = "2.0";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("4.0");
            enc_level = "4.0";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("3.1");
            enc_level = "3.1";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("2.0");
            enc_level = "2.0";
        }
    }
    if(ui->videoEncLevelSlider->value() == 7 &&
            ui->videoEncLevelSlider->sliderPosition() == 7)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("2.1");
            enc_level = "2.1";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("4.1");
            enc_level = "4.1";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("4.0");
            enc_level = "4.0";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("2.1");
            enc_level = "2.1";
        }
    }
    if(ui->videoEncLevelSlider->value() == 8 &&
            ui->videoEncLevelSlider->sliderPosition() == 8)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("2.2");
            enc_level = "2.2";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("5.0");
            enc_level = "5.0";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("4.1");
            enc_level = "4.1";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("2.2");
            enc_level = "2.2";
        }
    }
    if(ui->videoEncLevelSlider->value() == 9 &&
            ui->videoEncLevelSlider->sliderPosition() == 9)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("3.0");
            enc_level = "3.0";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("5.1");
            enc_level = "5.1";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("5.0");
            enc_level = "5.0";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("3.0");
            enc_level = "3.0";
        }
    }
    if(ui->videoEncLevelSlider->value() == 10 &&
            ui->videoEncLevelSlider->sliderPosition() == 10)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("3.1");
            enc_level = "3.1";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("5.2");
            enc_level = "5.2";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("5.1");
            enc_level = "5.1";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("3.1");
            enc_level = "3.1";
        }
    }
    if(ui->videoEncLevelSlider->value() == 11 &&
            ui->videoEncLevelSlider->sliderPosition() == 11)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("3.2");
            enc_level = "3.2";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("6.0");
            enc_level = "6.0";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("5.2");
            enc_level = "5.2";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("3.2");
            enc_level = "3.2";
        }
    }
    if(ui->videoEncLevelSlider->value() == 12 &&
            ui->videoEncLevelSlider->sliderPosition() == 12)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("4.0");
            enc_level = "4.0";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("6.1");
            enc_level = "6.1";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("6.0");
            enc_level = "6.0";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("4.0");
            enc_level = "4.0";
        }
    }
    if(ui->videoEncLevelSlider->value() == 13 &&
            ui->videoEncLevelSlider->sliderPosition() == 13)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
           ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("4.1");
            enc_level = "4.1";
        }
        //x265
        else if(ui->videoCodecBox->currentIndex() == 4 ||
                ui->videoCodecBox->currentIndex() == 5 || //x265 10bit
                ui->videoCodecBox->currentIndex() == 6) //x265 12bit)
        {
            ui->videoEncLevelDisplay->setText("6.2");
            enc_level = "6.2";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("6.1");
            enc_level = "6.1";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("4.1");
            enc_level = "4.1";
        }
    }
    if(ui->videoEncLevelSlider->value() == 14 &&
            ui->videoEncLevelSlider->sliderPosition() == 14)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
           ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("4.2");
            enc_level = "4.2";
        }
        //vp9
        else if(ui->videoCodecBox->currentIndex() == 8)
        {
            ui->videoEncLevelDisplay->setText("6.2");
            enc_level = "6.2";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("4.2");
            enc_level = "4.2";
        }
    }
    if(ui->videoEncLevelSlider->value() == 15 &&
       ui->videoEncLevelSlider->sliderPosition() == 15)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("5.0");
            enc_level = "5.0";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("5.0");
            enc_level = "5.0";
        }
    }
    if(ui->videoEncLevelSlider->value() == 16 &&
            ui->videoEncLevelSlider->sliderPosition() == 16)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("5.1");
            enc_level = "5.1";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("5.1");
            enc_level = "5.1";
        }
    }
    if(ui->videoEncLevelSlider->value() == 17 &&
            ui->videoEncLevelSlider->sliderPosition() == 17)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("5.2");
            enc_level = "5.2";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("5.2");
            enc_level = "5.2";
        }
    }
    if(ui->videoEncLevelSlider->value() == 18 &&
            ui->videoEncLevelSlider->sliderPosition() == 18)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("6.0");
            enc_level = "6.0";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("6.0");
            enc_level = "6.0";
        }
    }
    if(ui->videoEncLevelSlider->value() == 19 &&
            ui->videoEncLevelSlider->sliderPosition() == 19)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("6.1");
            enc_level = "6.1";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("6.1");
            enc_level = "6.1";
        }
    }
    if(ui->videoEncLevelSlider->value() == 20 &&
            ui->videoEncLevelSlider->sliderPosition() == 20)
    {
        //x264
        if(ui->videoCodecBox->currentIndex() == 2 ||
                ui->videoCodecBox->currentIndex() == 3)//x264 10bit)
        {
            ui->videoEncLevelDisplay->setText("6.2");
            enc_level = "6.2";
        }
        else
        {
            //every other codec
            ui->videoEncLevelDisplay->setText("6.2");
            enc_level = "6.2";
        }
    }
}

//average bitrate field
//maybe make it a slot??
QString VideoUI::get_vid_bitrate_field_text()
{
    if(!ui->videoAVGBitField->text().isEmpty())
    {
        //check contents of videoAVGBitField entry for valid data
        QRegExp rx("^(\\d+)(\\.?)(\\d*)(\\s*)([\\kmKM]?)$");
        if (rx.exactMatch(ui->videoAVGBitField->text()))
        {
            //videoAVGBitField data ends with a "k", "m", "K", or "M"
            if(ui->videoAVGBitField->text().endsWith("K", Qt::CaseInsensitive) ||
                    ui->videoAVGBitField->text().endsWith("M", Qt::CaseInsensitive))
            {
                video_bitrate = ui->videoAVGBitField->text().toLower().remove(" ");
            }
            else
            {
                //videoAVGBitField data with numbers only
                video_bitrate = ui->videoAVGBitField->text().remove(" ");
            }
        }
        else
        {
            //found invalid data->send message to statusbar->below is placeholder
            ui->videoAVGBitField->setText(tr("invalid, defaulting to 10000k"));

            //default to 10000k
            video_bitrate = "10000k";
        }
    }
    else
    {
        video_bitrate = "10000k";
        //ui->statusbar->showMessage("defaulting data to 10000k");
    }

    return video_bitrate;
}

void VideoUI::enable_two_pass_encode()
{

}
