/******************************************************************************
 Copyright (c) 2020-2022 b1aqW0lf

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/


#include "save_as_ui.h"
#include "ui_save_as_ui.h"

SaveAsUI::SaveAsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveAsUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->saveASButton, SIGNAL(clicked()), this, SLOT(select_output()));

    //saveAS line edit
    //ui->saveASEdit;

    //saveAS label
    ui->saveASLabel->setText(tr("Save As: "));

    //saveAS groupbox
    ui->OutputGroupBox->setTitle(tr("Select Destination "));
    ui->OutputGroupBox->setAlignment(Qt::AlignLeft);
}

SaveAsUI::~SaveAsUI()
{
    delete ui;
}

//select the output file to convert to
void SaveAsUI::select_output()
{
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
    emit saveas_value();
}
