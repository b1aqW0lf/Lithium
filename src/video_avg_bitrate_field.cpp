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


#include "video_avg_bitrate_field.h"
#include "src/ui_video_avg_bitrate_field.h"

#include <QRegularExpressionMatch>
#include <QRegularExpressionValidator>


VideoAVGBitrateField::VideoAVGBitrateField(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoAVGBitrateField)
{
    ui->setupUi(this);

    this->setup_avg_bitrate_field_settings();
    this->setup_avg_bitrate_field_validator();

    connect(ui->videoAVGBitrateRadio, &QRadioButton::clicked,
            this, &VideoAVGBitrateField::enable_average_bitrate_field);
    connect(ui->videoAVGBitrateRadio, &QRadioButton::clicked,
            this, &VideoAVGBitrateField::get_vid_bitrate_field_data);
    connect(ui->videoAVGBitrateField, &QLineEdit::returnPressed,
            this, &VideoAVGBitrateField::get_vid_bitrate_field_data);
    connect(ui->twoPassCheckBox, &QCheckBox::clicked,
            this, &VideoAVGBitrateField::enable_two_pass_encode);
}

VideoAVGBitrateField::~VideoAVGBitrateField()
{
    delete ui;
}

void VideoAVGBitrateField::setup_avg_bitrate_field_settings()
{
    ui->videoAVGBitrateField->setDisabled(true);
    ui->videoAVGBitrateLabel->setDisabled(true);
    ui->videoAVGBitrateField->setToolTip("Enter the bitrate for the output video stream");
    ui->twoPassCheckBox->setDisabled(true);
    ui->twoPassCheckBox->setToolTip(tr("Two-pass encoding"));
    ui->videoAVGBitrateLabel->setToolTip("Average bitrate of source video stream");
    ui->videoAVGBitrateGroupBox->setTitle("Average Bitrate");
    ui->videoAVGBitrateRadio->setText(tr("Bitrate (Kbps)"));
    ui->videoAVGBitrateLabel->setText("Kbps");
    ui->videoAVGBitrateField->setFixedWidth(115);
    /*ui->videoAVGBitField->setFixedHeight(25);*/
    ui->videoAVGBitrateField->setPlaceholderText("6000");
}

void VideoAVGBitrateField::setup_avg_bitrate_field_validator()
{
    QRegularExpression rx("^[1-9]+[0]*[.]?\\d*[\\s]?[kKmM]?$");//first digit cannot be zero
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->videoAVGBitrateField->setValidator(validator);
}


void VideoAVGBitrateField::receive_source_video_bitrate(const QString &bitrate)
{
    //receive source video bitrate
    this->process_source_video_bitrate(bitrate);
}

void VideoAVGBitrateField::process_source_video_bitrate(const QString &bitrate)
{
    //show source video bitrate
    ui->videoAVGBitrateLabel->setText(bitrate);
}

void VideoAVGBitrateField::enable_average_bitrate_field()
{
    if(ui->videoAVGBitrateRadio->isChecked() == true)
    {
        ui->videoAVGBitrateField->setEnabled(true);
        ui->videoAVGBitrateLabel->setEnabled(true);
        ui->twoPassCheckBox->setEnabled(true);
    }
    if(ui->videoAVGBitrateRadio->isChecked() == false)
    {
        ui->videoAVGBitrateField->setEnabled(false);
        ui->videoAVGBitrateLabel->setEnabled(false);
        ui->twoPassCheckBox->setEnabled(false);
    }
}
void VideoAVGBitrateField::get_vid_bitrate_field_data()
{
    const int timeout{0};
    if(ui->videoAVGBitrateRadio->isChecked() == true)
    {
        //report that the average bitrate field is enabled
        Q_EMIT average_bitrate_encode_enabled(true);

        if(!ui->videoAVGBitrateField->text().isEmpty())
        {
            //videoAVGBitField data ends with a "k", "m", "K", or "M"
            if(ui->videoAVGBitrateField->text().endsWith("K", Qt::CaseInsensitive) ||
                ui->videoAVGBitrateField->text().endsWith("M", Qt::CaseInsensitive))
            {
                ui->videoAVGBitrateField->setText(ui->videoAVGBitrateField->text().toUpper().remove(" "));
            }
            else
            {
                //videoAVGBitField data with numbers only
                ui->videoAVGBitrateField->setText(ui->videoAVGBitrateField->text().remove(" "));
                //Q_EMIT send_average_bitrate_value(video_bitrate);
            }
        }
        if(ui->videoAVGBitrateField->text().isEmpty())
        {
            //default to 6000k
            ui->videoAVGBitrateField->setText("6000");
            //send the 6000 value
        }
    }
    else
    {
        //report that the average bitrate field is disabled
        Q_EMIT average_bitrate_encode_enabled(false);
    }
}

void VideoAVGBitrateField::enable_two_pass_encode()
{
    const int &timeout{0};

    if(ui->videoAVGBitrateRadio->isChecked() == true)
    {
        if(ui->twoPassCheckBox->isChecked() == true)
        {
            //send signal that two pass encoding option is enabled
            Q_EMIT two_pass_encode_enabled(true);
            this->send_avg_bitrate_statusbar_message("Two-Pass Encode Enabled", timeout);
        }
        else
        {
            //send signal that two pass encoding option is disabled
            Q_EMIT two_pass_encode_enabled(false);
            this->send_avg_bitrate_statusbar_message("", timeout);
        }
    }
}
void VideoAVGBitrateField::decrease_vid_bitrate_field_value()
{

}
void VideoAVGBitrateField::increase_vid_bitrate_field_value()
{

}
