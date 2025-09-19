#include "video_crf_interface.h"
#include "src/ui_video_crf_interface.h"

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


#include <QStyleFactory>


VideoCRFInterface::VideoCRFInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoCRFInterface)
{
    ui->setupUi(this);

#ifdef Q_OS_WINDOWS
    //set slider style
    ui->videoRateFactorSlider->setStyle(QStyleFactory::create("windowsvista"));
#endif

    connect(ui->videoRateFactorSlider, &QSlider::valueChanged,
            this, &VideoCRFInterface::select_encoder_rate_factor);
    connect(ui->videoRateFactorSlider, &QSlider::valueChanged,
            ui->videoRFSpinBox, &QSpinBox::setValue);

    ui->videoRFSpinBox->setStyleSheet("QSpinBox { background-color: transparent; } QLineEdit { background-color: transparent; }");}

VideoCRFInterface::~VideoCRFInterface()
{
    delete ui;
}

void VideoCRFInterface::select_encoder_rate_factor(const int &index)
{
    const int timeout{0};
    this->selection.video_crf_selection_list.clear();

    //select the desired codec's constant rate factor (crf) or qscale value
    this->selection.video_crf_selection << QString::number(index); //receive selected crf/qscale value
    Q_EMIT this->send_statusbar_message(QString::number(index), timeout);
}

void VideoCRFInterface::receive_selected_video_codec_name(const QString &video_codec)
{
    this->set_crf_slider_options(video_codec);
}

void VideoCRFInterface::set_crf_slider_options(const QString &video_codec)
{
    command.video_crf_qscale.clear();

    if(video_codec.contains("H264", Qt::CaseInsensitive) ||
        video_codec.contains("H.264", Qt::CaseInsensitive))
    {
        //default libx264 crf value is 23
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRateFactorSlider->setRange(0, 51);
        ui->videoRateFactorSlider->setValue(23);
        command.video_crf_qscale << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("x264 10-bit", Qt::CaseInsensitive))
    {
        //default libx264 10-bit crf value is 23
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRateFactorSlider->setRange(0, 63);
        ui->videoRateFactorSlider->setValue(23);
        command.video_crf_qscale << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("HEVC", Qt::CaseInsensitive) ||
             video_codec.contains("H265", Qt::CaseInsensitive) ||
             video_codec.contains("H.265", Qt::CaseInsensitive))
    {
        //default libx265 crf value is 28
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRateFactorSlider->setRange(0, 51);
        ui->videoRateFactorSlider->setValue(28);
        command.video_crf_qscale << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("x265 10-bit", Qt::CaseInsensitive))
    {
        //default libx265 10-bit crf value is 28
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRateFactorSlider->setRange(0, 51);
        ui->videoRateFactorSlider->setValue(28);
        command.video_crf_qscale << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("x265 12-bit", Qt::CaseInsensitive))
    {
        //default libx265 12-bit crf value is 28
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRateFactorSlider->setRange(0, 51);
        ui->videoRateFactorSlider->setValue(28);
        command.video_crf_qscale << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("VP9", Qt::CaseInsensitive))
    {
        //default libvpx-vp9 crf value is 31
        //requires -b:v 0 flag as well to enable constant quality (cq) mode ---> -crf val -b:v 0
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRateFactorSlider->setRange(0, 63);
        ui->videoRateFactorSlider->setValue(31);
        command.video_crf_qscale << command.video_bitrate_flag << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("Xvid", Qt::CaseInsensitive))
    {
        //default libxvid crf value is 12
        ui->videoRFSpinBox->setRange(1, 31);
        ui->videoRateFactorSlider->setRange(1, 31);
        ui->videoRateFactorSlider->setValue(12);
        command.video_crf_qscale << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("Theora", Qt::CaseInsensitive))
    {
        //default libtheora variable bitrate (vbr) value is 7
        //accessed with the -qscale:v flag, higher numbers equals higher/better quality
        //requires -b:v 0 flag as well to enable constant quality (vbr) mode ---> -qscale:v val -b:v 0
        ui->videoRFSpinBox->setRange(0, 10);
        ui->videoRateFactorSlider->setRange(0, 10);
        ui->videoRateFactorSlider->setValue(7);
        command.video_crf_qscale << command.video_bitrate_flag << command.video_qscale_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("MPEG-2", Qt::CaseInsensitive))
    {
        //default mpeg2video crf value is 5
        ui->videoRFSpinBox->setRange(1, 31);
        ui->videoRateFactorSlider->setRange(1, 31);
        ui->videoRateFactorSlider->setValue(5);
        command.video_crf_qscale << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else if(video_codec.contains("AV1", Qt::CaseInsensitive))
    {
        //default libsvt-av1 crf value is 35
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRateFactorSlider->setRange(0, 63);
        ui->videoRateFactorSlider->setValue(35);
        command.video_crf_qscale << command.video_crf_flag;
        this->set_crf_slider_interface(video_codec);
    }
    else
    {
        return;
    }
}

void VideoCRFInterface::set_crf_slider_interface(const QString &video_codec)
{
    if(video_codec.contains("Theora", Qt::CaseInsensitive))
    {
        ui->videoCRFRadio->setText(tr("Variable Bit Rate"));
        ui->videoLQLabel->setText(tr("High Quality |"));
        ui->videoHQLabel->setText(tr("| Low Quality"));
    }
    else if(video_codec.contains("MPEG-2",Qt::CaseInsensitive))
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

void VideoCRFInterface::get_video_crf_qscale_selection()
{
    this->process_video_crf_qscale_selection();
}

void VideoCRFInterface::process_video_crf_qscale_selection()
{
    this->selection.video_crf_selection_list.clear();
    this->selection.video_crf_selection_list << command.video_crf_qscale
                                             << this->selection.video_crf_selection;

    Q_EMIT this->send_video_crf_qscale_selection(this->selection.video_crf_selection_list);
}
