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


#include "video_encoder_preset.h"
#include "ui_video_encoder_preset.h"

#include <QButtonGroup>
#include <QStyleFactory>


VideoEncoderPreset::VideoEncoderPreset(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoEncoderPreset)
{
    ui->setupUi(this);

#ifdef Q_OS_WINDOWS
    //set video encoder slider style
    ui->videoEncPresetSlider->setStyle(QStyleFactory::create("windowsvista"));
#endif

    this->set_preset_slider_default_size();

    connect(ui->videoEncPresetSlider, &QSlider::valueChanged, this, &VideoEncoderPreset::select_encoder_preset);
    connect(ui->fastDecodeCheckBox, &QCheckBox::clicked, this, &VideoEncoderPreset::enable_fast_decode);
    connect(ui->zeroLatencyCheckBox, &QCheckBox::clicked, this, &VideoEncoderPreset::enable_zero_latency);
}

VideoEncoderPreset::~VideoEncoderPreset()
{
    delete ui;
}

void VideoEncoderPreset::receive_selected_video_codec_name(const QString &video_codec)
{
    //receive the selected video codec
    this->video_codec = video_codec;
    this->set_preset_slider_default_position(video_codec);
}

void VideoEncoderPreset::set_preset_slider_default_position(const QString &video_codec)
{
    if(video_codec.contains("H264", Qt::CaseInsensitive) ||
        video_codec.contains("H.264", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.h264_presets);
        ui->videoEncPresetSlider->setValue(5);
    }
    else if(video_codec.contains("x264 10-bit", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.x26410bit_presets);
        ui->videoEncPresetSlider->setValue(5);
    }
    else if(video_codec.contains("HEVC", Qt::CaseInsensitive) ||
             video_codec.contains("H265", Qt::CaseInsensitive) ||
             video_codec.contains("H.265", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.hevc_presets);
        ui->videoEncPresetSlider->setValue(5);
    }
    else if(video_codec.contains("x265 10-bit", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.x26510bit_presets);
        ui->videoEncPresetSlider->setValue(5);
    }
    else if(video_codec.contains("x265 12-bit", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.x26512bit_presets);
        ui->videoEncPresetSlider->setValue(5);
    }
    else if(video_codec.contains("VP9", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.vp9_presets);
        ui->videoEncPresetSlider->setValue(17);
    }
    else if(video_codec.contains("Xvid", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.xvid_presets);
        ui->videoEncPresetSlider->setValue(5);
    }
    else if(video_codec.contains("Theora", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.theora_presets);
        ui->videoEncPresetSlider->setValue(1);
    }
    else if(video_codec.contains("MPEG-2", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.mpeg2_presets);
        ui->videoEncPresetSlider->setValue(5);
    }
    else if(video_codec.contains("AV1", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.av1_presets);
        ui->videoEncPresetSlider->setValue(10);
    }
    else
    {
        return;
    }
}

void VideoEncoderPreset::select_encoder_preset(const int &index)
{
    const int timeout{0};
    this->selection.video_preset_selection.clear();

    if(video_codec.contains("H264", Qt::CaseInsensitive) ||
        video_codec.contains("H.264", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.h264_presets);
        this->set_preset_label_text(preset_list.h264_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.h264_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.h264_presets[index], timeout);
    }
    else if(video_codec.contains("x264 10-bit", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.x26410bit_presets);
        this->set_preset_label_text(preset_list.x26410bit_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.x26410bit_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.x26410bit_presets[index], timeout);
    }
    else if(video_codec.contains("HEVC", Qt::CaseInsensitive) ||
             video_codec.contains("H265", Qt::CaseInsensitive) ||
             video_codec.contains("H.265", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.hevc_presets);
        this->set_preset_label_text(preset_list.hevc_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.hevc_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.hevc_presets[index], timeout);
    }
    else if(video_codec.contains("x265 10-bit", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.x26510bit_presets);
        this->set_preset_label_text(preset_list.x26510bit_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.x26510bit_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.x26510bit_presets[index], timeout);
    }
    else if(video_codec.contains("x265 12-bit", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.x26512bit_presets);
        this->set_preset_label_text(preset_list.x26512bit_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.x26512bit_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.x26512bit_presets[index], timeout);
    }
    else if(video_codec.contains("VP9", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.vp9_presets);
        this->set_preset_label_text(preset_list.vp9_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.vp9_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.vp9_presets[index], timeout);
    }
    else if(video_codec.contains("Xvid", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.xvid_presets);
        this->set_preset_label_text(preset_list.xvid_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.xvid_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.xvid_presets[index], timeout);
    }
    else if(video_codec.contains("Theora", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.theora_presets);
        this->set_preset_label_text(preset_list.theora_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.theora_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.theora_presets[index], timeout);
    }
    else if(video_codec.contains("MPEG-2", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.mpeg2_presets);
        this->set_preset_label_text(preset_list.mpeg2_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.mpeg2_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.mpeg2_presets[index], timeout);
    }
    else if(video_codec.contains("AV1", Qt::CaseInsensitive))
    {
        this->set_preset_slider_range(preset_list.av1_presets);
        this->set_preset_label_text(preset_list.av1_presets[index]);
        this->selection.video_preset_selection << command.video_preset_command
                                               << preset_list.av1_presets[index];
        Q_EMIT this->send_statusbar_message(preset_list.av1_presets[index], timeout);
    }
    else
    {
        return;
    }
}

void VideoEncoderPreset::set_preset_slider_default_size()
{
    const int width{150};
    ui->videoEncPresetSlider->setFixedWidth(width);
}

void VideoEncoderPreset::set_preset_slider_range(const QStringList &list)
{
    const int minimum{0};
    ui->videoEncPresetSlider->setRange(minimum, list.size()-1);
}

void VideoEncoderPreset::set_preset_label_text(const QString &text)
{
    ui->videoEncPresetLabel->setText(text);
}

void VideoEncoderPreset::enable_fast_decode()
{
    const int timeout{0};
    this->selection.fast_decode_selection.clear();
    if(ui->fastDecodeCheckBox->isChecked() == true)
    {
        if(ui->zeroLatencyCheckBox->isChecked() == true)
        {
            ui->zeroLatencyCheckBox->setChecked(false);
        }
        this->selection.fast_decode_selection << command.codec_tune_flag
                                              << command.fast_decode_command;
        Q_EMIT this->send_statusbar_message("Fast Decode Enabled", timeout);
    }
    else
    {
        this->selection.fast_decode_selection << "";
        Q_EMIT this->send_statusbar_message("", timeout);
    }
}

void VideoEncoderPreset::enable_zero_latency()
{
    const int timeout{0};
    this->selection.zero_latency_selection.clear();
    if(ui->zeroLatencyCheckBox->isChecked() == true)
    {
        if(ui->fastDecodeCheckBox->isChecked() == true)
        {
            ui->fastDecodeCheckBox->setChecked(false);
        }
        this->selection.zero_latency_selection << command.codec_tune_flag
                                               << command.zero_latency_command;
        Q_EMIT this->send_statusbar_message("Zero Latency Enabled", timeout);
    }
    else
    {
        this->selection.zero_latency_selection << "";
        Q_EMIT this->send_statusbar_message("", timeout);
    }
}
