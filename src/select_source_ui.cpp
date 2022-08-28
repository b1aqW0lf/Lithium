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


#include "select_source_ui.h"

SelectSourceUI::SelectSourceUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectSourceUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->sourceBrowse1, &QPushButton::clicked,
            this, &SelectSourceUI::select_input1);
    connect(ui->sourceBrowse2, &QPushButton::clicked,
            this, &SelectSourceUI::select_input2);

    //interface buttons initialization
    ui->sourceBrowse1 = new QPushButton(tr("..."));
    ui->sourceBrowse2 = new QPushButton(tr("..."));

    //source1 input field default settings
    ui->sourceInput1Edit->setToolTip(tr("Select video file to process"));
    ui->sourceInput1Edit->setPlaceholderText(tr("Select video file to process"));
    //ui->source1Label->setTextFormat(Qt::RichText);//it works
    ui->source1Label->setText(tr("Source 1:"));//use HTML formating to bold text

    //source2 input field default settings
    ui->sourceInput2Edit->setEnabled(false);
    ui->sourceBrowse2->setEnabled(false);
    ui->source2Label->setEnabled(false);
    ui->source2Label->setText(tr("Source 2:"));
}

SelectSourceUI::~SelectSourceUI()
{
    delete ui;
}

//select first input file to convert
void SelectSourceUI::select_input1()//complete!
{
    input_file1 =
            QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C:/input_file1",
                tr("Videos (*.mp4 *.mov *.avi *.mkv *.webm *.ogv *.mpeg)"));
    if(!input_file1.isEmpty())
    {
        ui->sourceInput1Edit->setText(input_file1);
    }
}

//select optional second input, an audio file to use
void SelectSourceUI::select_input2()
{
    input_file2 =
            QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C:/input_file2",
                tr("Audio (*.wav *.mp4a *.m4a *.flac *.mp3 *.ogg *.3gp)"));
    if(!input_file2.isEmpty())
    {
        ui->sourceInput2Edit->setText(input_file2);
    }
}

