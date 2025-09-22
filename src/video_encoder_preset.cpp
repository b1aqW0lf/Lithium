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

    //this->setup_preset_slider_default_range();

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
    this->set_preset_slider_options(video_codec);
}

void VideoEncoderPreset::set_preset_slider_options(const QString &video_codec)
{
    if(video_codec.contains("H264", Qt::CaseInsensitive) ||
        video_codec.contains("H.264", Qt::CaseInsensitive) ||
        video_codec.contains("x264", Qt::CaseInsensitive))
    {
        //this->set_preset_slider_default_range(preset_data.h264_presets);
        this->selection.video_encoder_presets = preset_data.h264_presets;//new
        //ui->videoEncPresetSlider->setSliderPosition(4);//default position
    }
    else if(video_codec.contains("HEVC", Qt::CaseInsensitive) ||
               video_codec.contains("H.265", Qt::CaseInsensitive) ||
               video_codec.contains("H265", Qt::CaseInsensitive) ||
               video_codec.contains("x265", Qt::CaseInsensitive))
    {
        //this->set_preset_slider_default_range(preset_data.hevc_presets);
        this->selection.video_encoder_presets = preset_data.h264_presets;//new
        //ui->videoEncPresetSlider->setSliderPosition(4);//default position
    }
}

/*void VideoEncoderPreset::set_preset_slider_default_range(const QStringList &preset_list)
{
    const int min{0};
    ui->videoEncPresetSlider->setRange(min, preset_list.size()-1);
    this->set_preset_slider_label_text(preset_list);
}

void VideoEncoderPreset::set_preset_slider_label_text(const QStringList &preset_list)
{
    //show the selected preset text
    //ui->videoEncPresetLabel->setText(preset_list[ui->videoEncPresetSlider->value()]);
    ui->videoEncPresetLabel->setText(preset_list[ui->videoEncPresetSlider->value()]);
}*/

void VideoEncoderPreset::select_encoder_preset(const int &index)
{
    const int timeout{0};
    this->selection.video_preset_selection.clear();

    //select the desired codec's video encoder preset
    //ui->videoEncPresetLabel->setText(this->selection.video_preset_value[index]);
    //Q_EMIT this->send_statusbar_message(this->selection.video_encoder_presets[index], timeout);

    Q_EMIT this->send_statusbar_message(QString::number(index), timeout);//works
    ui->videoEncPresetLabel->setText(preset_data.h264_presets[index]);//does not work

}

void VideoEncoderPreset::enable_fast_decode()
{
    const int timeout{0};
    if(ui->fastDecodeCheckBox->isChecked() == true)
    {
        Q_EMIT this->send_statusbar_message("Fast Decode Enabled", timeout);
    }
    else
    {
        Q_EMIT this->send_statusbar_message("", timeout);
    }
}

void VideoEncoderPreset::enable_zero_latency()
{
    const int timeout{0};
    if(ui->zeroLatencyCheckBox->isChecked() == true)
    {
        Q_EMIT this->send_statusbar_message("Zero Latency Enabled", timeout);
    }
    else
    {
        Q_EMIT this->send_statusbar_message("", timeout);
    }
}
