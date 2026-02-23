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
#include <QFileInfo>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#if !defined(TIMEOUT) || !defined(INDEX0)
#define TIMEOUT 0
#define INDEX0 0
#endif


SaveAsField::SaveAsField(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveAsField)
{
    ui->setupUi(this);

    setup_clear_button();
    setup_default_extensions();
    setup_ui_tooltips();

    //connect signals and slots
    connect(ui->saveAsBrowseButton, &QPushButton::clicked, this, &SaveAsField::select_save_destination);
    connect(ui->saveAsContainerBox, QOverload<int>::of(&QComboBox::activated),
            this, &SaveAsField::select_output_file_container);
    connect(ui->saveAsLineEdit, &QLineEdit::returnPressed, this, &SaveAsField::send_selected_output_path);
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

void SaveAsField::setup_default_extensions()
{
    int index{0};

    if(process_mode == ProcessMode::NormalMode ||
        process_mode == ProcessMode::MergeMode)
    {
        ui->saveAsContainerBox->insertItems(index, extensions.videoContainerList);
        //insert separator
        index = 1;
        ui->saveAsContainerBox->insertSeparator(index);
    }
}

void SaveAsField::setup_ui_tooltips()
{
    ui->saveAsContainerBox->setToolTip(tr("Select the destination file's extension."));
    ui->saveAsBrowseButton->setToolTip(tr("Click to choose the location to save the output."));
}

void SaveAsField::receive_input_file_name(const QString &file_name)
{
    process_input_file_name(file_name);
}

void SaveAsField::process_input_file_name(const QString &file_name)
{
    //load the input source file with the current selected extension
    ui->saveAsLineEdit->setText(file_name);
    get_input_file_extension(file_name);
    input_file_name = file_name;
}

void SaveAsField::get_input_file_extension(const QString &file_name)
{
    //get the extension of the input file
    QString input_ext = file_name.mid(file_name.lastIndexOf("."));
    ui->saveAsContainerBox->setItemData(INDEX0, input_ext, Qt::UserRole);
}

void SaveAsField::current_process_mode(ProcessMode process_mode)
{
    initalize_output_extensions(process_mode);
    process_mode = process_mode;
}

void SaveAsField::initalize_output_extensions(ProcessMode process_mode)
{
    ui->saveAsContainerBox->clear();

    if(process_mode == ProcessMode::NormalMode || process_mode == ProcessMode::MergeMode)
    {
        const int separator{1};
        //load the video container list
        ui->saveAsContainerBox->insertItems(INDEX0, extensions.videoContainerList);
        ui->saveAsContainerBox->insertSeparator(separator);
        ui->saveAsContainerBox->setCurrentIndex(3);
    }
    if(process_mode == ProcessMode::ExtractMode)
    {
        //load the audio container list
        ui->saveAsContainerBox->insertItems(INDEX0, extensions.audioContainerList);
    }

    //get input file extension again after process mode changes
    get_input_file_extension(input_file_name);
}

//select the output file path to convert to
void SaveAsField::select_save_destination()
{
    QString output_file = ui->saveAsLineEdit->text();
    if(ui->saveAsLineEdit->text().isEmpty() == false)
    {
        output_file =
            QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                output_file,
                tr(""));
        if(!output_file.isEmpty())
        {
            ui->saveAsLineEdit->setText(output_file);
        }
    }
    if(ui->saveAsLineEdit->text().isEmpty() == true)
    {
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
    }
}

void SaveAsField::select_output_file_container(const int &index)
{
    output_ext.clear();

    if(process_mode == ProcessMode::NormalMode || process_mode == ProcessMode::MergeMode)
    {
        //utilizing the videoContainerList
        if(index == 0)
        {
            //clicking "Source" will set the source container as the selected container
            output_ext = ui->saveAsContainerBox->itemData(index, Qt::UserRole).toString().toLower();
            Q_EMIT send_save_field_statusbar_message(ui->saveAsContainerBox->itemData(index,
                                            Qt::UserRole).toString().remove(".").toUpper(), TIMEOUT);
        }
        else if(index == 1)//separator
        {
            //option one (1) cannot be selected by the user - it is the separator
            return;
        }
        else if(index >= 2 && index <= extensions.videoContainerList.size())
        {
            output_ext = "."+ui->saveAsContainerBox->currentText().toLower();
            Q_EMIT send_save_field_statusbar_message(ui->saveAsContainerBox->currentText().toUpper(), TIMEOUT);
        }
        else
        {
            return;
        }
    }
    if(process_mode == ProcessMode::ExtractMode)
    {
        //utilizing the audioContainerList
        output_ext = "."+ui->saveAsContainerBox->itemData(index, Qt::DisplayRole).toString().toLower();
        Q_EMIT send_save_field_statusbar_message(ui->saveAsContainerBox->currentText().toUpper(), TIMEOUT);
    }

    set_output_file_extension(output_ext);
}

void SaveAsField::set_output_file_extension(const QString &output_ext)
{
    QString lineedit_text = ui->saveAsLineEdit->text();
    lineedit_text = lineedit_text.left(lineedit_text.lastIndexOf("."));
    if(!output_ext.isEmpty() && !lineedit_text.isEmpty())
    {
        //set the output file extension
        ui->saveAsLineEdit->setText(lineedit_text+output_ext);
    }
    else if(output_ext.isEmpty())
    {
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Please select an input file first"));
    }
    else if(lineedit_text.isEmpty() || lineedit_text == "")
    {
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Output file path not specified"));
    }
    else
    {
        return;
    }
}

void SaveAsField::send_selected_output_path()
{
    if(ui->saveAsLineEdit->text() == input_file_name)
    {
        QFileInfo file(ui->saveAsLineEdit->text());
        QString base_name = file.completeBaseName();
        QString path = file.absolutePath();
        QString ext = file.suffix();
        QString str{"_1"};

        ui->saveAsLineEdit->setText(path + "/" + base_name + str + "." + ext);
    }

    Q_EMIT send_output_file_path(ui->saveAsLineEdit->text());
}
