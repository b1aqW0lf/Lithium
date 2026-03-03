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


#include "subtitles_checkbox.h"
#include "src/ui_subtitles_checkbox.h"


SubtitlesCheckBox::SubtitlesCheckBox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubtitlesCheckBox)
{
    ui->setupUi(this);

    connect(ui->subtitlesCheckBox, &QCheckBox::clicked, this, &SubtitlesCheckBox::enable_copy_subtitles);
    connect(ui->subtitlesCheckBox, &QCheckBox::clicked, this, &SubtitlesCheckBox::send_subtitles_checkbox_message);
}

SubtitlesCheckBox::~SubtitlesCheckBox()
{
    delete ui;
}

void SubtitlesCheckBox::enable_copy_subtitles()
{
    QStringList subtitles_commands_list{};

    if(ui->subtitlesCheckBox->isChecked())
    {
        subtitles_commands_list << "-codec:s" << "copy";
        //ffmpeg command to copy subtitles
        Q_EMIT this->send_copy_subtitles_command(subtitles_commands_list);
    }
    else
    {
        subtitles_commands_list << "-sn";
        //ffmpeg command to not copy subtitles
        Q_EMIT this->send_copy_subtitles_command(subtitles_commands_list);
    }
}

void SubtitlesCheckBox::send_subtitles_checkbox_message()
{
    const int timeout{0};

    if(ui->subtitlesCheckBox->isChecked())
    {
        //send subtitles checkbox statusbar message
        Q_EMIT send_subtitles_checkbox_statusbar_message("Copy Metadata Enabled", timeout);
    }
    else
    {
        //send blank metadata checkbox statusbar message
        Q_EMIT send_subtitles_checkbox_statusbar_message("", timeout);
    }
}

void SubtitlesCheckBox::get_copy_subtitles_selection()
{
    //send copy subtitles command
    enable_copy_subtitles();
}
