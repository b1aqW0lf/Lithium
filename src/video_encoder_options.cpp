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


#include "video_encoder_options.h"
#include "ui_video_encoder_options.h"


VideoEncoderOptions::VideoEncoderOptions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoEncoderOptions)
{
    ui->setupUi(this);

    connect(ui->videoEncoderProfileBox, &QComboBox::textActivated,
            this, &VideoEncoderOptions::select_encoder_profile);
    connect(ui->videoEncoderLevelBox, &QComboBox::textActivated,
            this, &VideoEncoderOptions::select_encoder_level);

    this->setup_encoder_options_defaults();
}

VideoEncoderOptions::~VideoEncoderOptions()
{
    delete ui;
}

void VideoEncoderOptions::setup_encoder_options_defaults()
{
    //set h264/libx264 as the default settings
    const int index0{0};
    const int separator{1};
    ui->videoEncoderProfileBox->insertItems(index0, profile_data.h264_profiles);
    ui->videoEncoderLevelBox->insertItems(index0, level_data.h264_levels);
    ui->videoEncoderProfileBox->insertSeparator(separator);
    ui->videoEncoderLevelBox->insertSeparator(separator);
    ui->videoEncoderProfileBox->setCurrentIndex(2);
    ui->videoEncoderLevelBox->setCurrentIndex(2);
}

void VideoEncoderOptions::receive_selected_video_codec_name(const QString &video_codec)
{
    this->initialize_video_encoder_options(video_codec);
}

void VideoEncoderOptions::receive_video_codec_profile_level(const QString &codec_profile, const QString &codec_level)
{
    this->process_source_video_options_data(codec_profile, codec_level);
}

void VideoEncoderOptions::process_source_video_options_data(const QString &codec_profile, const QString &codec_level)
{
    const int index0{0};
    this->selection.encoder_profile_selection.clear();
    this->selection.encoder_level_selection.clear();

    this->selection.encoder_profile_selection << codec_profile;
    this->selection.encoder_level_selection << codec_level;
    ui->videoEncoderProfileBox->setItemData(index0, codec_profile, Qt::UserRole);
    ui->videoEncoderLevelBox->setItemData(index0, codec_level, Qt::UserRole);
}

void VideoEncoderOptions::initialize_video_encoder_options(const QString &video_codec)
{
    this->initialize_encoder_profiles(video_codec);
    this->initialize_encoder_levels(video_codec);
}

void VideoEncoderOptions::initialize_encoder_profiles(const QString &video_codec)
{
    //initialize videoEncoderProfileBox with the profiles of the selected codec
    ui->videoEncoderProfileBox->clear();
    const int index0{0};
    const int separator{1};

    if(video_codec.contains("H264", Qt::CaseInsensitive) ||
        video_codec.contains("H.264", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.h264_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("x264 10-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.x26410bit_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("HEVC", Qt::CaseInsensitive) ||
               video_codec.contains("H265", Qt::CaseInsensitive) ||
               video_codec.contains("H.265", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.hevc_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("x265 10-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.x26510bit_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("x265 12-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.x26512bit_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("VP9", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.vp9_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("Xvid", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.xvid_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("Theora", Qt::CaseInsensitive))
    {
        //theora does not have have video codec profiles
        return;
    }
    else if(video_codec.contains("MPEG-2", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.mpeg2_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("AV1", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.av1_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
        ui->videoEncoderProfileBox->setCurrentIndex(2);
    }
    else
    {
        return;
    }
}

void VideoEncoderOptions::initialize_encoder_levels(const QString &video_codec)
{
    //initialize videoEncoderLevelBox with the levels of the selected codec
    ui->videoEncoderLevelBox->clear();
    const int index0{0};
    const int separator{1};

    if(video_codec.contains("H264", Qt::CaseInsensitive) ||
        video_codec.contains("H.264", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.h264_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("x264 10-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.x26410bit_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("HEVC", Qt::CaseInsensitive) ||
             video_codec.contains("H265", Qt::CaseInsensitive) ||
             video_codec.contains("H.265", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.hevc_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("x265 10-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.x26510bit_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("x265 12-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.x26512bit_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("VP9", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.vp9_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("Xvid", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.xvid_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("Theora", Qt::CaseInsensitive))
    {
        //theora does not have have video codec levels
        return;
    }
    else if(video_codec.contains("MPEG-2", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.mpeg2_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else if(video_codec.contains("AV1", Qt::CaseInsensitive))
    {
        ui->videoEncoderLevelBox->insertItems(index0, level_data.av1_levels);
        ui->videoEncoderLevelBox->insertSeparator(separator);
        ui->videoEncoderLevelBox->setCurrentIndex(2);
    }
    else
    {
        return;
    }
}

void VideoEncoderOptions::select_encoder_profile(const QString &codec_profile)
{
    this->selection.encoder_profile_selection.clear();
    const int timeout{0};
    const int index0{0};

    if(codec_profile.contains("Source"))
    {
        //Set the User Role item data to the value of source file's profile
        this->selection.encoder_profile_selection << ui->videoEncoderProfileBox->itemData(index0, Qt::UserRole).toString().toLower();
        Q_EMIT this->send_statusbar_message(ui->videoEncoderProfileBox->itemData(index0, Qt::UserRole).toString(), timeout);
    }
    else
    {
        this->selection.encoder_profile_selection << codec_profile;
        Q_EMIT this->send_statusbar_message(codec_profile, timeout);
    }

    //set tooltip
    ui->videoEncoderProfileBox->setToolTip(ui->videoEncoderProfileBox->currentText());
}

void VideoEncoderOptions::select_encoder_level(const QString &codec_level)
{
    this->selection.encoder_level_selection.clear();
    const int timeout{0};
    const int index0{0};

    if(codec_level.contains("Source"))
    {
        //Set the User Role item data to the value of source file's level
        this->selection.encoder_level_selection << ui->videoEncoderLevelBox->itemData(index0, Qt::UserRole).toString().toLower();
        Q_EMIT this->send_statusbar_message(ui->videoEncoderLevelBox->itemData(index0, Qt::UserRole).toString(), timeout);
    }
    else
    {
        this->selection.encoder_level_selection << codec_level;
        Q_EMIT this->send_statusbar_message(codec_level, timeout);
    }

    //set tooltip
    ui->videoEncoderLevelBox->setToolTip(ui->videoEncoderLevelBox->currentText());
}

void VideoEncoderOptions::get_video_encoder_options_selections()
{
    this->process_video_options_selections();
}

void VideoEncoderOptions::process_video_options_selections()
{
    this->selection.video_options_selections.clear();

    //auto is not a valid parameter accepted by ffmpeg for profile
    //do not assign profile a value for auto to force ffmpeg to decide
    if(ui->videoEncoderProfileBox->currentText() == "auto" &&
        ui->videoEncoderLevelBox->currentText() != "auto")
    {
        //remove the selected "auto" value and command for encoder_profile_selection
        this->selection.video_options_selections << selection.encoder_level_command
                                                 << this->selection.encoder_level_selection;
    }
    else if(ui->videoEncoderLevelBox->currentText() == "auto" &&
            ui->videoEncoderProfileBox->currentText() != "auto")
    {
        //ffmpeg level command accepts -1 for auto
        //set selection.encoder_level_selection to -1
        this->selection.encoder_level_selection.clear();
        this->selection.video_options_selections << selection.encoder_profile_command << this->selection.encoder_profile_selection
                                                 << selection.encoder_level_command << this->selection.encoder_level_selection << "-1";
    }
    else if(ui->videoEncoderProfileBox->currentText() == "auto" &&
               ui->videoEncoderLevelBox->currentText() == "auto")
    {
        //set selection.encoder_level_selection to -1 for auto
        //and do not assign profile a value for auto to force ffmpeg to decide
        this->selection.encoder_level_selection.clear();
        this->selection.video_options_selections << selection.encoder_level_command
                                                 << this->selection.encoder_level_selection << "-1";
    }
    else
    {
        this->selection.video_options_selections << selection.encoder_profile_command << this->selection.encoder_profile_selection
                                                 << selection.encoder_level_command << this->selection.encoder_level_selection;
    }

    Q_EMIT this->send_video_options_selections(this->selection.video_options_selections);
}
