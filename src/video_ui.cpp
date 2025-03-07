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
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::set_encoder_preset_options);
    connect(ui->videoEncoderDial, &QDial::valueChanged,
            this, &VideoUI::select_encoder_preset);
    connect(ui->videoRFSlider, &QSlider::valueChanged,
            this, &VideoUI::select_encoder_crf_qscale);
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::set_encoder_crf_qscale_options);
    connect(ui->videoRFSlider, &QSlider::valueChanged,
            ui->videoRFSpinBox, &QSpinBox::setValue);
    //-------------------------------------------------------------------------
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::select_video_codec);
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
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoUI::set_enc_level_options);
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
    ui->videoRFSlider->setTickInterval(1);
    ui->videoRFSlider->setValue(23);//default value for the slider
    ui->videoRFSlider->setSliderPosition(23);
    ui->videoRFSlider->setToolTip(tr("Constant Rate Factor Selector"));
    crf_qscale_value.setNum(ui->videoRFSlider->value());//send default to encoder
    ui->videoHQLabel->setText(tr("| High Quality"));
    ui->videoLQLabel->setText(tr("Low Quality |"));
    ui->videoHQLabel->setAlignment(Qt::AlignTop);
    ui->videoLQLabel->setAlignment(Qt::AlignTop);

    //encoder dial
    ui->videoDialPreset->setAlignment(Qt::AlignCenter);
    ui->videoEncoderDial->setRange(0, this->videoEncPresetList.size()- 1);
    ui->videoEncoderDial->setNotchesVisible(true);
    ui->videoEncoderDial->setSingleStep(1);
    ui->videoEncoderDial->setToolTip(tr("<p>Select encoder preset/speed. A preset is a collection of options that will"
                                        " provide a certain encoding speed to compression ratio. Use the slowest preset "
                                        "that you have patience for.</p>"));
    ui->videoDialPreset->setText(tr("None"));//default value
    ui->videoDialPreset->setStyleSheet("QLabel { background-color : white }");
    ui->videoDialPreset->setStyleSheet("QLabel { border : 0.5px solid black }");

    //video codec interface
    VideoStandardItem::videoCodecBoxItem = new QStandardItem();
    VideoStandardItem::videoCodecBoxItem->setData(tr("Source"), Qt::DisplayRole);
    QStandardItemModel *videoCodecBoxModel = new QStandardItemModel(this);
    videoCodecBoxModel->setItem(0, VideoStandardItem::videoCodecBoxItem);
    ui->videoCodecBox->setModel(videoCodecBoxModel);

    ui->videoCodecBox->insertSeparator(1);
    videoCodecList << "H264/libx264" << "x264 10-bit" << "HEVC/libx265" << "x265 10-bit"
                   << "x265 12-bit" << "Xvid" << "VP9" << "Theora" << "MPEG-2"
                   << "AV1/SVT-AV1";
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
    //initalize video encoder level slider with default values
    set_video_codec_level_slider_settings(this->videoEncLevelList = QStringList() << "Auto");
    ui->videoEncLevelSlider->setRange(0, this->videoEncLevelList.size() - 1);
    ui->videoEncLevelSlider->setTickPosition(QSlider::TicksBelow);
    ui->videoEncLevelSlider->setSingleStep(1);
    ui->videoEncLevelLabel->setText(tr("Encoder Level:"));
    ui->videoEncLevelSlider->setToolTip(tr("Encoder Level Selector"));
    ui->videoEncLevelDisplay->setAlignment(Qt::AlignCenter);
    ui->videoEncLevelDisplay->setFixedWidth(35);
    ui->videoEncLevelDisplay->setFixedHeight(40);
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

void VideoUI::set_encoder_preset_options(int index)
{
    this->videoEncPresetList.clear();
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
        this->videoEncPresetList = QStringList() << "placebo" << "veryslow" << "slower" << "slow" << "medium"
                                                 << "fast" << "faster"<< "veryfast" << "superfast" << "ultrafast";
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(4);
    }
    else if(ui->videoCodecBox->itemText(index).contains("x264 10-bit", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "placebo" << "veryslow" << "slower" << "slow" << "medium"
                                                 << "fast" << "faster"<< "veryfast" << "superfast" << "ultrafast";
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(4);
    }
    else if(ui->videoCodecBox->itemText(index).contains("HEVC", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "placebo" << "veryslow" << "slower" << "slow" << "medium"
                                                 << "fast" << "faster"<< "veryfast" << "superfast" << "ultrafast";
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(4);
    }
    else if(ui->videoCodecBox->itemText(index).contains("x265 10-bit", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "placebo" << "veryslow" << "slower" << "slow" << "medium"
                                                 << "fast" << "faster"<< "veryfast" << "superfast" << "ultrafast";
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(4);
    }
    else if(ui->videoCodecBox->itemText(index).contains("x265 12-bit", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "placebo" << "veryslow" << "slower" << "slow" << "medium"
                                                 << "fast" << "faster"<< "veryfast" << "superfast" << "ultrafast";
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(4);
    }
    else if(ui->videoCodecBox->itemText(index).contains("VP9", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "-16" << "-15" << "-14" << "-13" << "-12" << "-11"//-speed
                                                 << "-10" << "-9" << "-8" << "-7" << "-6" << "-5"
                                                 << "-4" << "-3" << "-2" << "-1" << "0" << "1" << "2"
                                                 << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10"
                                                 << "11" << "12" << "13" << "14" << "15" << "16";
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(17);
    }
    else if(ui->videoCodecBox->itemText(index).contains("Xvid", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "placebo" << "veryslow" << "slower" << "slow" << "medium"
                                                 << "fast" << "faster"<< "veryfast" << "superfast" << "ultrafast";
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(4);
    }
    else if(ui->videoCodecBox->itemText(index).contains("Theora", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "good" << "best" << "realtime"; //accessed with -quality or -cpu-used
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(1);
    }
    else if(ui->videoCodecBox->itemText(index).contains("MPEG-2", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "placebo" << "veryslow" << "slower" << "slow" << "medium"
                                                 << "fast" << "faster"<< "veryfast" << "superfast" << "ultrafast";
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(4);
    }
    else if(ui->videoCodecBox->itemText(index).contains("AV1", Qt::CaseInsensitive))
    {
        this->videoEncPresetList = QStringList() << "0" << "1" << "2" << "3" << "4" << "5" << "6"  << "7" << "8"
                                                 << "9" << "10" << "11" << "12" << "13"; //accessed with -preset
        set_video_codec_preset_dial_settings(this->videoEncPresetList);
        ui->videoEncoderDial->setSliderPosition(10);
    }
}

void VideoUI::set_video_codec_preset_dial_settings(const QStringList &list)
{
    ui->videoEncoderDial->setRange(0, list.size() - 1);
    ui->videoDialPreset->setText(list[ui->videoEncoderDial->value()]);
}

//ui->videoEncoderDial->setRange(0, 9);
void VideoUI::select_encoder_preset(const int &index)
{
    ui->videoDialPreset->setText(this->videoEncPresetList[index]);
    this->encoder_preset_val = this->videoEncPresetList[index];
}

void VideoUI::set_encoder_crf_qscale_options(int index)
{
    //get_video_source_index(index);
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
        //default libx264 crf value is 23
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setRange(0, 51);
        ui->videoRFSlider->setValue(23);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("x264 10-bit", Qt::CaseInsensitive))
    {
        //default libx264 10-bit crf value is 23
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRFSlider->setRange(0, 63);
        ui->videoRFSlider->setValue(23);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("HEVC", Qt::CaseInsensitive))
    {
        //default libx265 crf value is 28
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setRange(0, 51);
        ui->videoRFSlider->setValue(28);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("x265 10-bit", Qt::CaseInsensitive))
    {
        //default libx265 10-bit crf value is 28
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setRange(0, 51);
        ui->videoRFSlider->setValue(28);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("x265 12-bit", Qt::CaseInsensitive))
    {
        //default libx265 12-bit crf value is 28
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setRange(0, 51);
        ui->videoRFSlider->setValue(28);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("VP9", Qt::CaseInsensitive))
    {
        //default libvpx-vp9 crf value is 31
        //requires -b:v 0 flag as well to enable constant quality (cq) mode ---> -crf val -b:v 0
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRFSlider->setRange(0, 63);
        ui->videoRFSlider->setValue(31);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("Xvid", Qt::CaseInsensitive))
    {
        //default libxvid crf value is 12
        ui->videoRFSpinBox->setRange(1, 31);
        ui->videoRFSlider->setRange(1, 31);
        ui->videoRFSlider->setValue(12);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("Theora", Qt::CaseInsensitive))
    {
        //default libtheora variable bitrate (vbr) value is 7
        //accessed with the -qscale:v flag, higher numbers equals higher/better quality
        //requires -b:v 0 flag as well to enable constant quality (vbr) mode ---> -qscale:v val -b:v 0
        ui->videoRFSpinBox->setRange(0, 10);
        ui->videoRFSlider->setRange(0, 10);
        ui->videoRFSlider->setValue(7);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("MPEG-2", Qt::CaseInsensitive))
    {
        //default mpeg2video crf value is 5
        ui->videoRFSpinBox->setRange(1, 31);
        ui->videoRFSlider->setRange(1, 31);
        ui->videoRFSlider->setValue(5);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else if(ui->videoCodecBox->itemText(index).contains("AV1", Qt::CaseInsensitive))
    {
        //default libsvt-av1 crf value is 35
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRFSlider->setRange(0, 63);
        ui->videoRFSlider->setValue(35);
        set_video_codec_crf_qscale_level_ui(index);
    }
    else
    {
        return;
    }
}

void VideoUI::set_video_codec_crf_qscale_level_ui(const int index)
{
    if(ui->videoCodecBox->itemText(index).contains("Theora", Qt::CaseInsensitive))
    {
        ui->videoCRFRadio->setText(tr("Variable Bit Rate"));
        ui->videoLQLabel->setText(tr("High Quality |"));
        ui->videoHQLabel->setText(tr("| Low Quality"));
    }
    else if(ui->videoCodecBox->itemText(index).contains("MPEG-2",Qt::CaseInsensitive))
    {
        ui->videoCRFRadio->setText(tr("Quality Rate Factor"));
    }
    else
    {
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
    }
}

//initalizing selected crf value
void VideoUI::select_encoder_crf_qscale(int index)
{
    //crf_value is part of input for ffmpeg args in encoding_started()
    //setting the crf value to string
    this->crf_qscale_value.setNum(index);
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
    load_video_source_options(ui->videoCodecBox->findText(this->source_codec.toUpper(),Qt::MatchContains));
}

void VideoUI::select_video_codec(const int index)
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
        //MPEG-2
        this->video_codec = "mpeg2video";
        break;
    case 11:
        //rav1e0
        this->video_codec = "libsvtav1";
        break;
    }
    Q_EMIT send_vid_data(this->video_codec,timeout);
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
    Q_EMIT send_vid_data(this->video_codec_profile, 0);
}

void VideoUI::set_enc_level_options(int index)
{
    this->videoEncLevelList.clear();
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
        this->videoEncLevelList = QStringList() << "auto" << "1" << "1b" << "1.1" << "1.2" << "1.3" << "2"
                                                << "2.1" << "2.2" << "3" << "3.1" << "3.2" << "4" << "4.1"
                                                << "4.2" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    //same encoder levels a H.264 (8-bit)
    else if(ui->videoCodecBox->itemText(index).contains("x264 10-bit", Qt::CaseInsensitive))
    {   this->videoEncLevelList = QStringList() << "auto" << "1" << "1b" << "1.1" << "1.2" << "1.3" << "2"
                                                << "2.1" << "2.2" << "3" << "3.1" << "3.2" << "4" << "4.1"
                                                << "4.2" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("HEVC", Qt::CaseInsensitive))
    {
        this->videoEncLevelList = QStringList() << "auto" << "1" << "2" << "2.1" << "3" << "3.1" << "4"
                                                << "4.1" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    //same encoder levels a H.265 X265 (8-bit)
    else if(ui->videoCodecBox->itemText(index).contains("x265 10-bit", Qt::CaseInsensitive))
    {
        this->videoEncLevelList = QStringList() << "auto" << "1" << "2" << "2.1" << "3" << "3.1" << "4"
                                                << "4.1" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    //same encoder levels a H.265 X265 (8-bit)
    else if(ui->videoCodecBox->itemText(index).contains("x265 12-bit", Qt::CaseInsensitive))
    {
        this->videoEncLevelList = QStringList() << "auto" << "1" << "2" << "2.1" << "3" << "3.1" << "4"
                                                << "4.1" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("VP9", Qt::CaseInsensitive))
    {
        this->videoEncLevelList = QStringList() << "auto" << "1" << "1.1" << "2" << "2.1" << "3" << "3.1"
                                                << "4" << "4.1" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("Xvid", Qt::CaseInsensitive))
    {
        this->videoEncLevelList = QStringList() << "auto" << "0" << "1" << "2" << "3" << "4";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("MPEG-2", Qt::CaseInsensitive))
    {
        this->videoEncLevelList = QStringList() << "auto" << "low" << "main" << "high1440" << "high";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    else if(ui->videoCodecBox->itemText(index).contains("AV1", Qt::CaseInsensitive))
    {
        this->videoEncLevelList = QStringList() << "auto" << "2.0" << "2.1" << "3.0" << "3.1" << "4.0" << "4.1"
                                            << "5.0" << "5.1" << "5.2" << " 5.3" << "6.0" << " 6.1" << "6.2" << "6.3";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
    }
    else
    {
        //no settings
        this->videoEncLevelList = QStringList() << "None";
        set_video_codec_level_slider_settings(this->videoEncLevelList);
        return;
    }
}

void VideoUI::set_video_codec_level_slider_settings(const QStringList &list)
{
    ui->videoEncLevelSlider->setRange(0, list.size() - 1);
    ui->videoEncLevelSlider->setSliderPosition(0);
    ui->videoEncLevelDisplay->setText(list[ui->videoEncLevelSlider->value()]);
}

//initializing selected encoder level
void VideoUI::select_encoder_level(const int index)
{
    //encoder level settings
    this->codec_level_enabled = false;
    this->video_codec_level.clear();
    if(this->videoEncLevelList[index].contains("Auto", Qt::CaseInsensitive))
    {
        //do not send codec level info and let ffmpeg decide
        ui->videoEncLevelDisplay->setText(this->videoEncLevelList[index]);//display the option
        this->codec_level_enabled = false;
    }
    else
    {
        ui->videoEncLevelDisplay->setText(this->videoEncLevelList[index]);
        this->video_codec_level = this->videoEncLevelList[index];
        this->codec_level_enabled = true;
    }
    //testing only!!
    Q_EMIT send_vid_data(this->video_codec_level, 0);
}

void VideoUI::load_video_source_options(int index)
{
    set_encoder_preset_options(index);
    set_enc_profile_options(index);
    set_enc_level_options(index);
    set_encoder_crf_qscale_options(index);
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
        ui->videoAVGBitField->setText(QString::number(line_edit_val));
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
    Q_EMIT send_current_video_options(video_codec, video_bitrate, crf_qscale_value,
                                video_res_value, video_dar_value, calculate_dar_enabled,
                                video_fps_value, encoder_preset_val, pixel_format,
                                pixel_format_enabled, video_codec_profile,
                                codec_profile_enabled, video_codec_level,
                                codec_level_enabled);
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
