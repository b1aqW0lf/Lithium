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


#include <QAction>
#include <QComboBox>
#include <QDial>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QRegularExpression>
#include <QSlider>
#include <QSpinBox>
#include <QStandardItem>
#include <QStandardItemModel>

#include "ui_video_ui.h"
#include "video_ui.h"


namespace VideoStandardItem
{
    QStandardItem *videoCodecBoxItem{};
    QStandardItem *videoContainerBoxItem{};
    QStandardItem *videoResBoxItem{};
    QStandardItem *videoAspectRatBoxItem{};
    QStandardItem *videoFPSBoxItem{};
}

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
    //experimental-----------------------------------------------------------------//
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::select_vid_codec);
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::vid_codec_interface);
    connect(ui->videoContainerBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::select_container);
    connect(ui->videoResolutionBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::select_video_res);
    connect(ui->videoAspectRatBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::select_dar_value);
    connect(ui->videoFPSBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::select_video_fps);
    //------------------------------------------------------------------------------//
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::set_enc_profile_options);
    connect(ui->videoEncProfileBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::select_encoder_profile);
    connect(ui->videoEncLevelSlider, &QSlider::valueChanged,
            this, &VideoUI::select_encoder_level);
    //-------------------------------------------------------------------------
    connect(ui->videoCRFRadio, &QRadioButton::clicked,
            this, &VideoUI::enable_crf_option);
    //-------------------------------------------------------------------------
    connect(ui->videoAVGBitRadio, &QRadioButton::clicked,
            this, &VideoUI::enable_average_bitrate_field);
    connect(ui->videoAVGBitRadio, &QRadioButton::clicked,
            this, &VideoUI::get_vid_bitrate_field_data);
    connect(ui->videoAVGBitField, &QLineEdit::returnPressed,
            this, &VideoUI::get_vid_bitrate_field_data);
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
    ui->videoAVGBitField->setDisabled(true);
    ui->twoPassCheckBox->setDisabled(true);

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
    ui->videoHQLabel->setText(tr("| High Quality"));
    ui->videoLQLabel->setText(tr("Low Quality |"));
    ui->videoHQLabel->setAlignment(Qt::AlignTop);
    ui->videoLQLabel->setAlignment(Qt::AlignTop);

    //encoder dial
    ui->videoDialPreset->setAlignment(Qt::AlignCenter);
    ui->videoEncoderDial->setRange(0, 9);
    ui->videoEncoderDial->setNotchesVisible(true);
    ui->videoEncoderDial->setSingleStep(1);
    ui->videoEncoderDial->setValue(4);//setting default value
    ui->videoEncoderDial->setSliderPosition(4);
    ui->videoEncoderDial->setToolTip(tr("default: medium encoding"));
    ui->videoDialPreset->setText(tr("medium"));//default value
    ui->videoDialPreset->setStyleSheet("QLabel { background-color : white }");
    ui->videoDialPreset->setStyleSheet("QLabel { border : 0.5px solid black }");
    encoder_preset_val = "medium";//send default to encoder
    //ui->videoEncoderDial->setInvertedAppearance(true);

    //video codec interface
    VideoStandardItem::videoCodecBoxItem = new QStandardItem();
    VideoStandardItem::videoCodecBoxItem->setData(tr("Source"), Qt::DisplayRole);
    QStandardItemModel *videoCodecBoxModel = new QStandardItemModel(this);
    videoCodecBoxModel->setItem(0, VideoStandardItem::videoCodecBoxItem);
    ui->videoCodecBox->setModel(videoCodecBoxModel);

    ui->videoCodecBox->insertSeparator(1);
    videoCodecList << "H264/libx264" << "x264 10-bit" << "HEVC/libx265" << "x265 10-bit"
                   << "x265 12-bit" << "Xvid" << "VP9" << "Theora" << "MPEG-1"
                   << "MPEG-2" << "AV1/SVT-AV1";
    ui->videoCodecBox->insertItems(2, videoCodecList);
    ui->videoCodecLabel->setText(tr("Codec "));

    //video container
    VideoStandardItem::videoContainerBoxItem = new QStandardItem();
    VideoStandardItem::videoContainerBoxItem->setData(tr("Source"), Qt::DisplayRole);
    QStandardItemModel *videoContainerBoxModel = new QStandardItemModel(this);
    videoContainerBoxModel->setItem(0, VideoStandardItem::videoContainerBoxItem);
    ui->videoContainerBox->setModel(videoContainerBoxModel);

    ui->videoContainerBox->insertSeparator(1);
    videoContainerList << "MP4" << "MKV" << "WebM" << "TS" << "OGV";
    ui->videoContainerBox->insertItems(2, videoContainerList);
    ui->videoContainerLabel->setText(tr("Container"));

    //video resolution
    VideoStandardItem::videoResBoxItem = new QStandardItem();
    VideoStandardItem::videoResBoxItem->setData(tr("Source"), Qt::DisplayRole);
    QStandardItemModel *videoResBoxModel = new QStandardItemModel(this);
    videoResBoxModel->setItem(0, VideoStandardItem::videoResBoxItem);
    ui->videoResolutionBox->setModel(videoResBoxModel);

    ui->videoResolutionBox->insertSeparator(1);
    videoResList << "640x360" << "720x480" << "720x576" << "1024x768"
                 << "1280x720" << "1366x768" << "1600x900" << "1920x1080"
                 << "1920x1200" << "2048x1080" << "2048x1152" << "2560x1440"
                 << "3840x2160" << "4096x2160";
    ui->videoResolutionBox->insertItems(2, videoResList);
    ui->videoResolutionLabel->setText(tr("Resolution"));

    //video aspect ratio
    VideoStandardItem::videoAspectRatBoxItem = new QStandardItem();
    VideoStandardItem::videoAspectRatBoxItem->setData(tr("Source"), Qt::DisplayRole);
    QStandardItemModel *videoAspectRatBoxModel = new QStandardItemModel(this);
    videoAspectRatBoxModel->setItem(0, VideoStandardItem::videoAspectRatBoxItem);
    ui->videoAspectRatBox->setModel(videoAspectRatBoxModel);

    ui->videoAspectRatBox->insertSeparator(1);
    videoAspecRatList << "4:3" << "16:9" << "21:9";
    ui->videoAspectRatBox->insertItems(2, videoAspecRatList);
    ui->videoAspectRatLabel->setText(tr("Aspect Ratio"));

    //video framerate
    VideoStandardItem::videoFPSBoxItem = new QStandardItem();
    VideoStandardItem::videoFPSBoxItem->setData(tr("Source"), Qt::DisplayRole);
    QStandardItemModel *videoFPSBoxModel = new QStandardItemModel(this);
    videoFPSBoxModel->setItem(0, VideoStandardItem::videoFPSBoxItem);
    ui->videoFPSBox->setModel(videoFPSBoxModel);

    ui->videoFPSBox->insertSeparator(1);
    videoFPSList << "23.976" << "24" << "25" << "29.97" << "30" << "60" << "120";
    ui->videoFPSBox->insertItems(2, videoFPSList);
    ui->videoFPSLabel->setText(tr("Framerate (FPS)"));
    //ui->videoFPSBox->setCurrentIndex(5);--->test, it works!

    //video encoder profile
    ui->videoEncProfileBox->insertItem(0, "Source");
    ui->videoEncProfileBox->insertSeparator(1);
    videoEncProfileList << "auto" << "baseline" << "main"
                        << "high" << "high10" << "high422" << "high444";
    ui->videoEncProfileBox->insertItems(2, videoEncProfileList);
    ui->videoEncProfileBox->setCurrentIndex(2);

    //video average bitrate
    ui->videoAVGBitRadio->setText(tr("Avg Bitrate (Kbps)"));
    ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
    /*ui->videoAVGBitField->setFixedHeight(25);*/
    ui->videoAVGBitField->setFixedWidth(115);
    ui->videoAVGBitField->setPlaceholderText("6000");
    QRegularExpression rx("^[1-9]+[0]*[.]?\\d*[\\s]?[kKmM]?$");//accept only digits as input - first digit cannot be zero
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->videoAVGBitField->setValidator(validator);

    minusAction = new QAction(this);
    plusAction = new QAction(this);
    ui->videoAVGBitField->addAction(minusAction, QLineEdit::TrailingPosition);
    ui->videoAVGBitField->addAction(plusAction, QLineEdit::TrailingPosition);
    minusAction->setIcon(QIcon(":/images/resources/button_minus.svg"));
    plusAction->setIcon(QIcon(":/images/resources/button_plus.svg"));
    minusAction->setToolTip(tr("Decrease the entered average bitrate"));
    plusAction->setToolTip(tr("Increase the entered average bitrate"));
    connect(minusAction, &QAction::triggered, this, &VideoUI::decrease_vid_bitrate_field_value);
    connect(plusAction, &QAction::triggered, this, &VideoUI::increase_vid_bitrate_field_value);

    //video groupbox
    ui->videoGroupBox->setTitle(tr("Video"));
    ui->videoGroupBox->setAlignment(Qt::AlignLeft);

    //video endoder level
    ui->videoEncLevelLabel->setText(tr("Encoder Level:"));
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

    //encoder profile box
    ui->videoEncProfileLabel->setText(tr("Encoder Profile"));

    //average bitrate
    ui->sourceVideoAVGBitDisplay->setText("");
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
        //test for AV1 codec/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 0"));
            encoder_preset_val = "0";
            ui->videoEncoderDial->setToolTip(tr("Speed 0 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("placebo"));
            encoder_preset_val = "placebo";
            ui->videoEncoderDial->setToolTip(tr("placebo encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 1 &&
            ui->videoEncoderDial->sliderPosition() == 1)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 1"));
            encoder_preset_val = "1";
            ui->videoEncoderDial->setToolTip(tr("Speed 1 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("veryslow"));
            encoder_preset_val = "veryslow";
            ui->videoEncoderDial->setToolTip(tr("veryslow encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 2 &&
            ui->videoEncoderDial->sliderPosition() == 2)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 2"));
            encoder_preset_val = "2";
            ui->videoEncoderDial->setToolTip(tr("Speed 2 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("slower"));
            encoder_preset_val = "slower";
            ui->videoEncoderDial->setToolTip(tr("slower encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 3 &&
            ui->videoEncoderDial->sliderPosition() == 3)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 3"));
            encoder_preset_val = "3";
            ui->videoEncoderDial->setToolTip(tr("Speed 3 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("slow"));
            encoder_preset_val = "slow";
            ui->videoEncoderDial->setToolTip(tr("slow encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 4 &&
            ui->videoEncoderDial->sliderPosition() == 4)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 4"));
            encoder_preset_val = "4";
            ui->videoEncoderDial->setToolTip(tr("Speed 4 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("medium"));
            encoder_preset_val = "medium";
            ui->videoEncoderDial->setToolTip(tr("medium encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 5 &&
            ui->videoEncoderDial->sliderPosition() == 5)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 5"));
            encoder_preset_val = "5";
            ui->videoEncoderDial->setToolTip(tr("Speed 5 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("fast"));
            encoder_preset_val = "fast";
            ui->videoEncoderDial->setToolTip(tr("fast encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 6 &&
            ui->videoEncoderDial->sliderPosition() == 6)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 6"));
            encoder_preset_val = "6";
            ui->videoEncoderDial->setToolTip(tr("Speed 6 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("faster"));
            encoder_preset_val = "faster";
            ui->videoEncoderDial->setToolTip(tr("faster encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 7 &&
            ui->videoEncoderDial->sliderPosition() == 7)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 7"));
            encoder_preset_val = "7";
            ui->videoEncoderDial->setToolTip(tr("Speed 7 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("veryfast"));
            encoder_preset_val = "veryfast";
            ui->videoEncoderDial->setToolTip(tr("veryfast encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 8 &&
            ui->videoEncoderDial->sliderPosition() == 8)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 8"));
            encoder_preset_val = "8";
            ui->videoEncoderDial->setToolTip(tr("Speed 8 Encoding"));
        }
        //any other codec
        else
        {
            ui->videoDialPreset->setText(tr("superfast"));
            encoder_preset_val = "superfast";
            ui->videoEncoderDial->setToolTip(tr("superfast encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 9 &&
            ui->videoEncoderDial->sliderPosition() == 9)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 9"));
            encoder_preset_val = "9";
            ui->videoEncoderDial->setToolTip(tr("Speed 9 Encoding"));
        }
        //any other codec
        else
        {
            ui->videoDialPreset->setText(tr("ultrafast"));
            encoder_preset_val = "ultrafast";
            ui->videoEncoderDial->setToolTip(tr("ultrafast encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 10 &&
            ui->videoEncoderDial->sliderPosition() == 10)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 10"));
            encoder_preset_val = "10";
            ui->videoEncoderDial->setToolTip(tr("Speed 10 Encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 11 &&
        ui->videoEncoderDial->sliderPosition() == 11)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 11"));
            encoder_preset_val = "11";
            ui->videoEncoderDial->setToolTip(tr("Speed 11 Encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 12 &&
        ui->videoEncoderDial->sliderPosition() == 12)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 12"));
            encoder_preset_val = "12";
            ui->videoEncoderDial->setToolTip(tr("Speed 12 Encoding"));
        }
    }
    if(ui->videoEncoderDial->value() == 13 &&
        ui->videoEncoderDial->sliderPosition() == 13)
    {
        //test for AV1/svt-av1
        if(ui->videoCodecBox->currentIndex() == 12)
        {
            ui->videoDialPreset->setText(tr("Speed 13"));
            encoder_preset_val = "13";
            ui->videoEncoderDial->setToolTip(tr("Speed 13 Encoding"));
        }
    }//test, it works!
}

//initalizing selected crf value
void VideoUI::select_crf()
{
    //crf_value is part of input for ffmpeg args in encoding_started()
    //setting the crf value to string
    this->crf_value.setNum(ui->videoRFSlider->value());
}

//initalizing selected qscale value
void VideoUI::select_qscale()
{
    //setting qscale value to string
    this->qscale_value.setNum(ui->videoRFSlider->value());
}

void VideoUI::receive_vid_source_codec(const QString &codec)
{
    if(codec.contains("("))
    {
        this->source_codec = codec.left(codec.lastIndexOf("(")).remove(" ");
    }
    else
    {
        this->source_codec = codec;
    }
    VideoStandardItem::videoCodecBoxItem->setData(this->source_codec, Qt::UserRole);
}

void VideoUI::select_vid_codec(const int index)
{
    int timeout{0};
    this->pixel_format.clear();
    this->pixel_format_enabled = false;

    switch(index) {
    case 0:
        //copy from source
        this->video_codec = ui->videoCodecBox->itemData(0).toString();
        break;
    case 1:
        //separator - cannot be selected by user
        break;
    case 2:
        //x264
        this->video_codec = "libx264";
        break;
    case 3:
        //x264 10bit
        this->video_codec = "libx264";
        this->pixel_format << "-pix_fmt" << "yuv420p10le";
        this->pixel_format_enabled = true;
        break;
    case 4:
        //x265
        this->video_codec = "libx265";
        break;
    case 5:
        //x265 10bit
        this->video_codec = "libx265";
        this->pixel_format << "-pix_fmt" << "yuv444p10le";
        this->pixel_format_enabled = true;
        break;
    case 6:
        //x265 12bit
        this->video_codec = "libx265";
        this->pixel_format << "-pix_fmt" << "yuv444p12le";
        this->pixel_format_enabled = true;
        break;
    case 7:
        //Xvid
        this->video_codec = "libxvid";
        break;
    case 8:
        //vp9
        this->video_codec = "libvpx-vp9";
        break;
    case 9:
        //theora
        this->video_codec = "libtheora";
        break;
    case 10:
        //MPEG-1
        this->video_codec = "mpeg1video";
        break;
    case 11:
        //MPEG-2
        this->video_codec = "mpeg2video";
        break;
    case 12:
        //rav1e0
        this->video_codec = "libsvtav1";
        break;
    }
    Q_EMIT send_vid_data(this->video_codec,timeout);
}

void VideoUI::vid_codec_interface()
{
    //video codec interface settings
    if(ui->videoCodecBox->currentIndex() == 0 ||
        ui->videoCodecBox->currentIndex() == 2 ||
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
        ui->videoEncoderDial->setValue(4);//default value
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
        this->qscale_value.setNum(ui->videoRFSlider->value());//setting qscale value
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
        video_bitrate = "0";
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
        this->qscale_value.setNum(ui->videoRFSlider->value());//setting qscale value
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
        this->qscale_value.setNum(ui->videoRFSlider->value());//setting qscale value
        ui->videoEncLevelSlider->setRange(0,20);
        ui->videoEncLevelSlider->setValue(0);
        ui->videoEncLevelSlider->setSliderPosition(0);
    }
    else if(ui->videoCodecBox->currentIndex() == 12)
    {
        //default svt-av1 settings, crf range is 0-63, 0 is highest, 35 is default
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRFSlider->setRange(0, 63);
        ui->videoEncoderDial->setRange(0, 13);
        ui->videoRFSlider->setValue(35);
        ui->videoRFSpinBox->setValue(35);
        ui->videoRFSlider->setSliderPosition(35);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoLQLabel->setAlignment(Qt::AlignTop);
        ui->videoHQLabel->setAlignment(Qt::AlignTop);
        ui->videoEncoderDial->setToolTip(tr("default: Speed 4 Encoding"));
        ui->videoEncoderDial->setValue(10);//default value
        ui->videoDialPreset->setText(tr("Speed 10"));//default value
        this->qscale_value.setNum(ui->videoRFSlider->value());//setting qp value
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

void VideoUI::receive_vid_source_extension(const QString &extension)
{
    this->source_ext = extension;
    VideoStandardItem::videoContainerBoxItem->setData(this->source_ext, Qt::UserRole);
}

void VideoUI::select_container(const int index)
{
    QString vid_ext{};

    if(index == 0)
    {
        //copy from source
        //source file extension from receive_vid_source_extension
        vid_ext = ui->videoContainerBox->itemData(0).toString();
        Q_EMIT send_output_vid_extension(vid_ext);
    }
    else
    {
        vid_ext = ui->videoContainerBox->currentText().toLower();
        Q_EMIT send_output_vid_extension("."+vid_ext);
    }
}

void VideoUI::receive_vid_source_resolution(const QString &display)
{
    this->source_res = display;
    VideoStandardItem::videoResBoxItem->setData(this->source_res, Qt::UserRole);
}

void VideoUI::select_video_res(const int index)
{
    this->video_res_value = this->source_res;

    //select video resolution
    if(index == 0)//it works!
    {
        //video_res_value = source file value
        this->video_res_value = "scale="+ui->videoResolutionBox->itemData(0).toString();
    }
    else
    {
        this->video_res_value = "scale="+ui->videoResolutionBox->currentText();
    }
}

void VideoUI::receive_vid_source_coded_size(const QString &width, const QString &height)
{
    this->source_coded_width = width;
    this->source_coded_height = height;
}

void VideoUI::receive_vid_source_display_aspect_ratio(const QString &dar)
{
    if(dar.contains("N/A", Qt::CaseInsensitive) || dar.isEmpty())
    {
        //force ffmpeg to calculate the display aspect resolution from source
        const QString aspect_ratio{source_coded_width+"/"+source_coded_height};
        this->source_dar = "setdar="+aspect_ratio;
        this->calculate_dar_enabled = true;
    }
    else
    {
        //copy display aspect ratio value
        this->source_dar = dar;
        this->calculate_dar_enabled = false;
    }
    VideoStandardItem::videoAspectRatBoxItem->setData(this->source_dar, Qt::UserRole);
}

//diplay aspect ratio
void VideoUI::select_dar_value(const int index)
{
    int timeout{0};
    //select display aspect ratio
    if(index == 0)
    {
        //copy display aspect ratio value
        this->video_dar_value = ui->videoAspectRatBox->itemData(0).toString();
    }
    else if(index > 0)
    {
        this->video_dar_value = ui->videoAspectRatBox->currentText();
    }
    else
    {
        return;
    }
    Q_EMIT send_vid_data(video_dar_value, timeout);
}

void VideoUI::receive_vid_source_framerate(const QString &framerate)
{
    this->source_fps = framerate;
    VideoStandardItem::videoFPSBoxItem->setData(this->source_fps, Qt::UserRole);
}

void VideoUI::select_video_fps(const int index)
{
    //select video frames per second value
    if(index == 0)
    {
        //video_fps_val = "copy";
        this->video_fps_value = ui->videoFPSBox->itemData(0).toString();
    }
    else
    {
        this->video_fps_value = ui->videoFPSBox->currentText();
    }
    Q_EMIT send_vid_data(video_fps_value,0);
}

void VideoUI::receive_vid_source_codec_profile(const QString &profile)
{
    this->source_codec_profile = profile.toLower();
}

//creating options for encoder profile combobox/
void VideoUI::set_enc_profile_options(int index)
{
    //video encoder profile depends on which encoder is selected
    ui->videoEncProfileBox->clear();
    videoEncProfileList.clear();
    if(ui->videoCodecBox->currentIndex() == 0)
    {
        if(ui->videoCodecBox->findText(this->source_codec.toUpper(),Qt::MatchContains))
        {
            const int val{ui->videoCodecBox->findText(this->source_codec.toUpper(),Qt::MatchContains)};
            index = val;
        }
    }

    if(ui->videoCodecBox->itemText(index).contains("H264", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "baseline" << "main"
                                            << "high" << "high10" << "high422" << "high444";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("x264 10-bit", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "high10" << "high422" << "high444";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("HEVC", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "main" << "main-intra"
                                            << "mainstillpicture" << "main444-8" << "main444-intra"
                                            << "main444-stillpicture";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("x265 10-bit", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "main10" << "main10-intra"
                                            << "main422-10" << "main422-10-intra" << "main444-10"
                                            << "main444-10-intra";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }

    else if(ui->videoCodecBox->itemText(index).contains("x265 12-bit", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "main12" << "main12-intra"
                                            << "main422-12" << "main422-12-intra" << "main444-12"
                                            << "main444-12-intra";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("VP9", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "0" << "1" << "2" << "3";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("Xvid", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "1" << "2" << "3" << "4"
                                            << "5" << "6" <<"7" << "8" << "9";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("MPEG-2", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "main" << "high";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("AV1", Qt::CaseInsensitive))
    {
        videoEncProfileList = QStringList() << "Source" << "auto" << "main";
        set_video_codec_profile_box_settings(videoEncProfileList);
    }
    else
    {
        return;
    }
}

void VideoUI::set_video_codec_profile_box_settings(const QStringList &list)
{
    ui->videoEncProfileBox->insertItems(0, list);
    ui->videoEncProfileBox->insertSeparator(1);
    ui->videoEncProfileBox->setCurrentIndex(2);
}

//select encoder profile
void VideoUI::select_encoder_profile(const int index)
{
    //default: auto - default in FFmpeg
    //ffmpeg suggests omitting encoder profile option to allow ffmpeg to choose
    this->video_codec_profile.clear();
    this->codec_profile_enabled = false;

    if(ui->videoEncProfileBox->itemText(index).contains("Source", Qt::CaseInsensitive))
    {
        //this->video_codec_profile
        this->video_codec_profile = this->source_codec_profile;
        this->codec_profile_enabled = true;
    }
    else if(ui->videoEncProfileBox->itemText(index).contains("Auto", Qt::CaseInsensitive))
    {
        //do not send codec_profile info and let ffmpeg decide
        this->codec_profile_enabled = false;
    }
    else if((ui->videoEncProfileBox->itemText(index).isEmpty()))
    {
        //no profile to send to encoder
        this->codec_profile_enabled = false;
    }
    else
    {
        this->video_codec_profile = ui->videoEncProfileBox->itemText(index);
        this->codec_profile_enabled = true;
    }
    //for testing purposes ONLY!!
    send_vid_data(this->video_codec_profile, 0);
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

void VideoUI::enable_average_bitrate_field()
{
    //enable video bitrate and two pass options
    ui->videoAVGBitField->setDisabled(false);
    ui->twoPassCheckBox->setDisabled(false);
    Q_EMIT average_bitrate_encode_enabled(true);

    //disable the crf option
    ui->videoRFSlider->setDisabled(true);
    ui->videoRFSpinBox->setDisabled(true);
    ui->videoHQLabel->setDisabled(true);
    ui->videoLQLabel->setDisabled(true);
}

void VideoUI::enable_crf_option()
{
    //ffmpeg allows either crf or average bitrate + two pass
    //disable the average bitrate and two pass options
    ui->videoAVGBitField->setDisabled(true);
    ui->twoPassCheckBox->setDisabled(true);

    //enable the crf option
    ui->videoRFSlider->setDisabled(false);
    ui->videoRFSpinBox->setDisabled(false);
    ui->videoHQLabel->setDisabled(false);
    ui->videoLQLabel->setDisabled(false);
}

void VideoUI::receive_vid_source_bitrate(const QString &bitrate)
{
    ui->sourceVideoAVGBitDisplay->setText(" "+bitrate+" Kbps");
    ui->sourceVideoAVGBitDisplay->setToolTip("Video Source Bitrate");
}

//average bitrate field
void VideoUI::get_vid_bitrate_field_data()
{
    if(ui->videoAVGBitRadio->isChecked() == true &&
        ui->videoCRFRadio->isChecked() == false)
    {
        //report that the average bitrate field is enabled
        Q_EMIT average_bitrate_encode_enabled(true);

        if(!ui->videoAVGBitField->text().isEmpty())
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
                //Q_EMIT send_average_bitrate_value(video_bitrate);
            }
        }
        else
        {
            //default to 6000k
            video_bitrate = "6000k";
            //Q_EMIT send_average_bitrate_value(video_bitrate);
            //ui->statusbar->showMessage("defaulting data to 6000k");
        }
    }
    else
    {
        //report that the average bitrate field is disabled
        Q_EMIT average_bitrate_encode_enabled(false);
    }
}

void VideoUI::decrease_vid_bitrate_field_value()
{
    int line_edit_val{ui->videoAVGBitField->text().toInt()};
    int val{10};

    if(ui->videoAVGBitField->text().isEmpty() || line_edit_val <= 0)
    {
        //do nothing
        return;
    }
    else if(line_edit_val <= 10 && line_edit_val > 0)
    {
        //subtract bitrate by itself
        line_edit_val = line_edit_val - line_edit_val;
        ui->videoAVGBitField->setText(QString::number(line_edit_val));
    }
    else
    {
        //decrement bitrate by 10
        line_edit_val = line_edit_val - val;
        ui->videoAVGBitField->setText(QString::number(line_edit_val));
    }
}

void VideoUI::increase_vid_bitrate_field_value()
{
    int line_edit_val{ui->videoAVGBitField->text().toInt()};
    int val{10};

    if(line_edit_val < 0)
    {
        //do nothing
        return;
    }
    else if(ui->videoAVGBitField->text().isEmpty())
    {
        line_edit_val = val;
    }
    else
    {
        //increment bitrate by 10
        line_edit_val = line_edit_val + val;
        ui->videoAVGBitField->setText(QString::number(line_edit_val));
    }
}

void VideoUI::get_selected_video_options()
{
    default_options_check();

    //emit the current selected video options
    Q_EMIT send_current_video_options(video_codec, video_bitrate, crf_value,
                                qscale_value, video_res_value, video_dar_value,
                                calculate_dar_enabled, video_fps_value,
                                encoder_preset_val, pixel_format,
                                pixel_format_enabled, video_codec_profile,
                                codec_profile_enabled);
}

void VideoUI::default_options_check()
{
    //setting default value
    //we need a better solution!--maybe check for mouse clicks?
    if(ui->videoCodecBox->currentIndex() == 0)
    {
        this->video_codec = ui->videoCodecBox->itemData(0).toString();
    }
    if(ui->videoResolutionBox->currentIndex() == 0)
    {
        this->video_res_value = "scale="+ui->videoResolutionBox->itemData(0).toString();
    }
    if(ui->videoContainerBox->currentIndex() == 0)
    {
        Q_EMIT send_output_vid_extension(ui->videoContainerBox->itemData(0).toString());
    }
    if(ui->videoAspectRatBox->currentIndex() == 0)
    {
        this->video_dar_value = ui->videoAspectRatBox->itemData(0).toString();
    }
    if(ui->videoFPSBox->currentIndex() == 0)
    {
        this->video_fps_value = ui->videoFPSBox->itemData(0).toString();
    }
    if(ui->videoEncProfileBox->currentIndex() == 0)
    {
        this->video_codec_profile = this->source_codec_profile;
    }
}

void VideoUI::receive_clear_request()
{
    ui->sourceVideoAVGBitDisplay->clear();
    ui->sourceVideoAVGBitDisplay->setToolTip("");
}

void VideoUI::enable_two_pass_encode()
{
    if(ui->videoAVGBitRadio->isChecked() == true)
    {
        if(ui->twoPassCheckBox->isChecked() == true)
        {
            //send signal that two pass encoding option is enabled
            Q_EMIT two_pass_encode_enabled(true);
        }
        else
        {
            //send signal that two pass encoding option is disabled
            Q_EMIT two_pass_encode_enabled(false);
        }
    }
}

void VideoUI::receive_process_mode_state(const bool &normal, const bool &merge,
                                const bool &extract)
{
    //process mode state
    this->normal_mode = normal;
    this->merge_mode = merge;
    this->extract_mode = extract;

    //enable extract audio settings
    //extracting audio is the only mode that requires changes to the VideoUI interface
    enable_extract_audio_settings(extract_mode);
}

void VideoUI::enable_extract_audio_settings(const bool &extract)
{
    //extract audio settings
    if(extract == true)
    {
        ui->videoAspectRatBox->setEnabled(false);
        ui->videoAspectRatLabel->setEnabled(false);
        ui->videoCodecBox->setEnabled(false);
        ui->videoCodecLabel->setEnabled(false);
        ui->videoContainerBox->setEnabled(false);
        ui->videoContainerLabel->setEnabled(false);
        ui->videoFPSBox->setEnabled(false);
        ui->videoFPSLabel->setEnabled(false);
        ui->videoResolutionBox->setEnabled(false);
        ui->videoResolutionLabel->setEnabled(false);
        //--------------------------------------------
        ui->videoEncoderDial->setEnabled(false);
        ui->videoDialLabel->setEnabled(false);
        ui->videoDialPreset->setEnabled(false);
        ui->videoEncLevelLabel->setEnabled(false);
        ui->videoEncLevelDisplay->setEnabled(false);
        ui->videoEncProfileBox->setEnabled(false);
        ui->videoEncProfileLabel->setEnabled(false);
        ui->videoCRFRadio->setEnabled(false);
        ui->videoRFSlider->setEnabled(false);
        ui->videoRFSpinBox->setEnabled(false);
        ui->videoHQLabel->setEnabled(false);
        ui->videoLQLabel->setEnabled(false);
        ui->videoAVGBitRadio->setEnabled(false);
    }
    else
    {
        ui->videoAspectRatBox->setEnabled(true);
        ui->videoAspectRatLabel->setEnabled(true);
        ui->videoCodecBox->setEnabled(true);
        ui->videoCodecLabel->setEnabled(true);
        ui->videoContainerBox->setEnabled(true);
        ui->videoContainerLabel->setEnabled(true);
        ui->videoFPSBox->setEnabled(true);
        ui->videoFPSLabel->setEnabled(true);
        ui->videoResolutionBox->setEnabled(true);
        ui->videoResolutionLabel->setEnabled(true);
        //--------------------------------------------
        ui->videoEncoderDial->setEnabled(true);
        ui->videoDialLabel->setEnabled(true);
        ui->videoDialPreset->setEnabled(true);
        ui->videoEncLevelLabel->setEnabled(true);
        ui->videoEncLevelDisplay->setEnabled(true);
        ui->videoEncProfileBox->setEnabled(true);
        ui->videoEncProfileLabel->setEnabled(true);
        ui->videoCRFRadio->setEnabled(true);
        ui->videoRFSlider->setEnabled(true);
        ui->videoRFSpinBox->setEnabled(true);
        ui->videoHQLabel->setEnabled(true);
        ui->videoLQLabel->setEnabled(true);
        ui->videoAVGBitRadio->setEnabled(true);
    }
}
