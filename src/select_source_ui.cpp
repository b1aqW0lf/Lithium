/******************************************************************************
 Copyright (c) 2020-2023 b1aqW0lf
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


#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>

#include "select_source_ui.h"
#include "ui_select_source_ui.h"

SelectSourceUI::SelectSourceUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectSourceUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->sourceBrowseBtn1, &QPushButton::clicked,
            this, &SelectSourceUI::select_input1);
    connect(ui->sourceBrowseBtn2, &QPushButton::clicked,
            this, &SelectSourceUI::select_input2);
    connect(ui->sourceBrowseBtn1, &QPushButton::clicked,
            this, &SelectSourceUI::send_video_source_data);
    connect(ui->sourceBrowseBtn2, &QPushButton::clicked,
            this, &SelectSourceUI::send_audio_source_data);

    //interface buttons initialization
    ui->sourceBrowseBtn1->setText(tr("..."));
    ui->sourceBrowseBtn2->setText(tr("..."));

    //source1 input field default settings
    ui->sourceInput1Edit->setToolTip(tr("Select video file to process"));
    ui->sourceInput1Edit->setPlaceholderText(tr("Select video file to process"));
    ui->source1Label->setText(tr("Source 1:"));//use HTML formating to bold text
    ui->sourceInput2Edit->setPlaceholderText(tr("Select audio file"));
    ui->source2Label->setText(tr("Source 2:"));

    //default mode
    normal_mode_settings_enabled();

    //enable clear button
    ui->sourceInput1Edit->setClearButtonEnabled(true);
    ui->sourceInput2Edit->setClearButtonEnabled(true);
    QToolButton *clearButton1 = ui->sourceInput1Edit->findChild<QToolButton *>();
    QToolButton *clearButton2 = ui->sourceInput2Edit->findChild<QToolButton *>();
    clearButton1->setIcon(QIcon(":/images/resources/clear_icon.png"));
    clearButton2->setIcon(QIcon(":/images/resources/clear_icon.png"));
    clearButton1->setToolTip("clear");
    clearButton2->setToolTip("clear");

    connect(clearButton1, &QToolButton::clicked,
            this, &SelectSourceUI::clear_input1_data_request);
}

SelectSourceUI::~SelectSourceUI()
{
    delete ui;
}

void SelectSourceUI::clear_input1_data_request()
{
    Q_EMIT clear_input1_data();
}

void SelectSourceUI::send_video_source_data()
{
    //get current sourceInput1Edit file and extension
    QString source_text{};
    QString source_ext{};
    source_text = ui->sourceInput1Edit->text();
    //Q_EMIT current_vid_source_file(source_text);

    //based on code from qtffmpeg
    source_ext = source_text.mid(source_text.lastIndexOf("."));
    Q_EMIT current_vid_source_extension(source_ext);
}

//send audio source input source extension
void SelectSourceUI::send_audio_source_data()
{
    //get current sourceInput2Edit file and extension
    QString source_audio_text{};
    QString source_audio_ext{};
    source_audio_text = ui->sourceInput2Edit->text();
    //Q_EMIT current_audio_source_file(source_audio_text);

    //based on code from qtffmpeg
    source_audio_ext = source_audio_text.mid(source_audio_text.lastIndexOf("."));
    Q_EMIT current_audio_source_extension(source_audio_ext);
}

//select first input file to convert
void SelectSourceUI::select_input1()//complete!
{
    QString input_file1 =
            QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C:/input_file1",
                tr("Videos (*.mp4 *.mov *.avi *.mkv *.webm *.ogv *.mpeg *.mpg"
                   " *.vp9 *.vp8)"));
    if(!input_file1.isEmpty())
    {
        ui->sourceInput1Edit->setText(input_file1);
        Q_EMIT current_vid_source_file(input_file1);
    }
}

//select optional second input, an audio file to use
void SelectSourceUI::select_input2()
{
    QString input_file2 =
            QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C:/input_file2",
                tr("Audio (*.wav *.mp4a *.m4a *.flac *.mp3 *.ogg *.3gp *.mka"
                   " *.aac *.eac *.oga)"));
    if(!input_file2.isEmpty())
    {
        ui->sourceInput2Edit->setText(input_file2);
        Q_EMIT current_audio_source_file(input_file2);
    }
}

void SelectSourceUI::merge_sources_settings_enabled()
{
    //merge sources settings
    ui->sourceInput2Edit->setEnabled(true);
    ui->sourceBrowseBtn2->setEnabled(true);
    ui->source2Label->setEnabled(true);
    ui->sourceInput2Edit->setToolTip(tr("Select audio file"));
}

void SelectSourceUI::normal_mode_settings_enabled()
{
    //normal mode settings
    ui->sourceInput2Edit->setEnabled(false);
    ui->sourceBrowseBtn2->setEnabled(false);
    ui->source2Label->setEnabled(false);
    ui->sourceInput2Edit->setToolTip(tr(""));
}

void SelectSourceUI::extract_audio_settings_enabled()
{
    //extract audio settings
    ui->sourceInput2Edit->setEnabled(false);
    ui->sourceBrowseBtn2->setEnabled(false);
    ui->source2Label->setEnabled(false);
    ui->sourceInput2Edit->setToolTip(tr(""));
}
