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


#include "simple_progressbar.h"
#include "src/ui_simple_progressbar.h"

#include <QProgressBar>


SimpleProgressbar::SimpleProgressbar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SimpleProgressbar)
{
    ui->setupUi(this);

    //progressbar settings
    ui->progressbar->setValue(0);
    ui->progressbar->setFixedHeight(17);
    ui->progressbar->setFixedWidth(160);
    this->show_progressbar(false); //hide the progressbar on first launch
}

SimpleProgressbar::~SimpleProgressbar()
{
    delete ui;
}

void SimpleProgressbar::receive_ffprobe_frames_value(const QString &nb_frames)
{
    //receive the counted/total frame number of the source file
    //use the total frame number as the progressbar max value
    this->set_progressbar_range(nb_frames.toInt());
}
void SimpleProgressbar::set_progressbar_range(const int &maximum)
{
    const int minimum{0};
    //set the lower and upper bounds of the progressbar
    ui->progressbar->setRange(minimum, maximum);
}

void SimpleProgressbar::receive_frame_num_value(const QString &frame_num)
{
    //check if progressbar is invisible
    if(ui->progressbar->isVisible() == false && frame_num.toInt() > 0)
    {
        this->show_progressbar(true);
    }/**///maybe a better a beter check would be to check when transcode starts<--!!!

    //update the progressbar with new frame_num value
    this->update_progressbar(frame_num);
}

void SimpleProgressbar::update_progressbar(const QString &frame_num)
{
    //progressbar's progress is based on the frames being processed
    ui->progressbar->setValue(frame_num.toInt());
}

void SimpleProgressbar::cancel_progressbar_process()
{
    ui->progressbar->reset();
    this->show_progressbar(false);
}

void SimpleProgressbar::show_progressbar(const bool &visible)
{
    if(ui->progressbar->value() == 0)
    {
        ui->progressbar->setVisible(false);
        ui->progressLabel->setVisible(false);
        ui->line->setVisible(false);
    }
    else
    {
        ui->progressbar->setVisible(visible);
        ui->progressLabel->setVisible(visible);
        ui->line->setVisible(visible);
    }
}

void SimpleProgressbar::set_progressbar_theme()
{
    //default or dark theme
    this->set_theme();
}

void SimpleProgressbar::set_theme()
{
    ui->progressbar->setStyleSheet("QProgressBar { text-align: center }"
                                   "QProgressBar::chunk { background-color: #70A8E0; width: 10px; margin: 0.75px; }");/**/
}
