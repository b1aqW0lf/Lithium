/******************************************************************************
 Copyright (c) 2020-2022 b1aqW0lf
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


#include "process_mode_ui.h"
#include "ui_process_mode_ui.h"

#include <QGroupBox>
#include <QRadioButton>

ProcessModeUI::ProcessModeUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessModeUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->mergeSourcesRadio, &QRadioButton::clicked,
            this, &ProcessModeUI::merge_sources_mode);
    connect(ui->extractAudioRadio, &QRadioButton::clicked,
            this, &ProcessModeUI::extract_audio_mode);/**/
    connect(ui->normalModeRadio, &QRadioButton::clicked,
            this, &ProcessModeUI::normal_processing_mode);
    //--------------------------------------------------------------------------

    //Initialize interface widgets
    //radio buttons defaults
    processButtons = new QButtonGroup(this);
    processButtons->addButton(ui->normalModeRadio);
    processButtons->addButton(ui->mergeSourcesRadio);
    processButtons->addButton(ui->extractAudioRadio);
    processButtons->setExclusive(true);
    ui->normalModeRadio->setChecked(true);
    ui->normalModeRadio->setToolTip(tr("Normal processing"));
    ui->extractAudioRadio->setToolTip(tr("Extract audio from video to save"));
    ui->mergeSourcesRadio->setToolTip(tr("Merge a video source with an audio "
                                  "source"));
    ui->modeGroupBox->setTitle(tr("Process Mode"));
    ui->modeGroupBox->setAlignment(Qt::AlignLeft);
    //--------------------------------------------------------------------------
}

ProcessModeUI::~ProcessModeUI()
{
    delete ui;
}

void ProcessModeUI::merge_sources_mode()
{
    int timeout{0};

    //merging sources settings in select_source
    Q_EMIT current_process_mode_status("Merge Sources Mode Enabled", timeout);
    Q_EMIT enable_merge_sources_settings();
}

void ProcessModeUI::extract_audio_mode()
{
    int timeout{0};

    //audio extract settings in select_source
    Q_EMIT current_process_mode_status("Extract Audio Mode Enabled", timeout);
    Q_EMIT enable_extract_audio_settings();
}

void ProcessModeUI::normal_processing_mode()
{
    int timeout{0};

    //normal mode settings in select_source
    Q_EMIT current_process_mode_status("Normal Processing Mode Enabled", timeout);
    Q_EMIT enable_normal_mode_settings();
}

void ProcessModeUI::current_process_mode()
{
    //mode variables
    bool normal_mode{};
    bool merge_sources{};
    bool extract_audio{};

    if(ui->normalModeRadio->isChecked() == true)
    {
        normal_mode = true;
    }
    if(ui->mergeSourcesRadio->isChecked() == true)
    {
        merge_sources = true;
    }
    if(ui->extractAudioRadio->isChecked() == true)
    {
        extract_audio = true;
    }
    Q_EMIT current_process_mode_state(normal_mode, merge_sources, extract_audio);
}
