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


#include "save_as_field.h"
#include "ui_save_as_field.h"

#include <QAction>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


SaveAsField::SaveAsField(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveAsField)
{
    ui->setupUi(this);

    this->setup_clear_button();

    //connect signals and slots
    connect(ui->saveAsBrowseButton, &QPushButton::clicked, this, &SaveAsField::select_save_destination);
}

SaveAsField::~SaveAsField()
{
    delete ui;
}

void SaveAsField::setup_clear_button()
{
    //enable clear button
    ui->saveAsLineEdit->setClearButtonEnabled(true);
    QAction *clearButton = ui->saveAsLineEdit->findChild<QAction *>();
    clearButton->setIcon(QIcon(":/images/resources/clear_icon.png"));
    clearButton->setToolTip("clear");
}

//select the output file to convert to
void SaveAsField::select_save_destination()
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
        ui->saveAsLineEdit->setText(output_file);
    }
    Q_EMIT send_output_file_path(ui->saveAsLineEdit->text());
}

void SaveAsField::receive_output_extension(const QString &ext)
{
    QString save_as_line_data{ui->saveAsLineEdit->text()};

    //based on code from qtffmpeg
    save_as_line_data = save_as_line_data.left(save_as_line_data.lastIndexOf("."));
    if(!ui->saveAsLineEdit->text().isEmpty())
    {
        ui->saveAsLineEdit->setText(save_as_line_data+ext);
    }
    this->send_selected_output_path();
}

void SaveAsField::send_selected_output_path()
{
    Q_EMIT send_output_file_path(ui->saveAsLineEdit->text());
}
