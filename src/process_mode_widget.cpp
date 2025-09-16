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


#include "process_mode_widget.h"
#include "src/ui_process_mode_widget.h"

#include <QButtonGroup>


ProcessModeWidget::ProcessModeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProcessModeWidget)
{
    ui->setupUi(this);

    //process mode default settings
    this->setup_process_mode_buttons();

    connect(ui->normalModeRadio, &QRadioButton::clicked, this, &ProcessModeWidget::select_process_mode);
    connect(ui->extractAudioRadio, &QRadioButton::clicked, this, &ProcessModeWidget::select_process_mode);
    connect(ui->mergeSourcesRadio, &QRadioButton::clicked, this, &ProcessModeWidget::select_process_mode);
}

ProcessModeWidget::~ProcessModeWidget()
{
    delete ui;
}

void ProcessModeWidget::setup_process_mode_buttons()
{
    //process mode default settings
    QButtonGroup *processButtons = new QButtonGroup(this);
    processButtons->addButton(ui->normalModeRadio);
    processButtons->addButton(ui->mergeSourcesRadio);
    processButtons->addButton(ui->extractAudioRadio);
    processButtons->setExclusive(true);/**/
    ui->normalModeRadio->setChecked(true);//default enabled button
    ui->normalModeRadio->setToolTip(tr("Normal mode processing"));
    ui->extractAudioRadio->setToolTip(tr("Extract audio from a video to save"));
    ui->mergeSourcesRadio->setToolTip(tr("Merge a video source with an audio "
                                         "source"));
}

void ProcessModeWidget::select_process_mode()
{
    const int timeout{0};

    this->get_current_process_mode();
    if(ui->normalModeRadio->isChecked())
    {
        Q_EMIT current_process_mode_status("Normal Processing Mode Enabled", timeout);
    }
    else if(ui->extractAudioRadio->isChecked())
    {
        Q_EMIT current_process_mode_status("Extract Audio Mode Enabled", timeout);
    }
    else if(ui->mergeSourcesRadio->isChecked())
    {
        Q_EMIT current_process_mode_status("Merge Sources Mode Enabled", timeout);
    }
    else
    {
        Q_EMIT current_process_mode_status("Something went wrong!!", timeout);
    }
}

void ProcessModeWidget::send_current_process_mode()
{
    this->get_current_process_mode();
}

void ProcessModeWidget::get_current_process_mode()
{
    if(ui->normalModeRadio->isChecked())
    {
        this->process_mode = ProcessMode::NormalMode;
        Q_EMIT current_process_mode(process_mode);
    }
    else if(ui->extractAudioRadio->isChecked())
    {
        this->process_mode = ProcessMode::ExtractMode;
        Q_EMIT current_process_mode(process_mode);
    }
    else if(ui->mergeSourcesRadio->isChecked())
    {
        this->process_mode = ProcessMode::MergeMode;
        Q_EMIT current_process_mode(process_mode);
    }
    else
    {
        return;
    }
}
