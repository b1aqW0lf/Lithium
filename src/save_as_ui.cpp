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


#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "save_as_ui.h"
#include "ui_save_as_ui.h"

SaveAsUI::SaveAsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveAsUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->saveASButton, SIGNAL(clicked()), this, SLOT(select_output()));

    //saveAS label
    ui->saveASLabel->setText(tr("Save As: "));
}

SaveAsUI::~SaveAsUI()
{
    delete ui;
}

//select the output file to convert to
void SaveAsUI::select_output()
{
    QString output_file{};
    output_file =
            QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                "C:/output_file",
                tr(""));
    if(!output_file.isEmpty())
    {
        ui->saveASEdit->setText(output_file);
    }
}

void SaveAsUI::receive_output_vid_extension(const QString &text)
{
    QString saveAs_line_data{ui->saveASEdit->text()};
    saveAs_line_data = saveAs_line_data.left(saveAs_line_data.lastIndexOf("."));
    ui->saveASEdit->setText(saveAs_line_data+text);
}
