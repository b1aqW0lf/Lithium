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


#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QScrollBar>

#include "app_location.h"
#include "detect_storage.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "save_as_ui.h"
#include "ui_save_as_ui.h"
#include "select_source_ui.h"
#include "ui_select_source_ui.h"
#include "video_ui.h"
#include "ui_video_ui.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    application_connections_setup();

    //display avaialble storage
    DetectStorage dsx{this};
    QLabel *statusBarLabel1, *statusBarLabel2;
    statusBarLabel1 = new QLabel(this);
    statusBarLabel2 = new QLabel(this);
    statusBarLabel1->setPixmap(tr(":/images/resources/hd_16px.png"));
    statusBarLabel1->setToolTip(tr("Available storage on main disk").toUtf8());
    statusBarLabel2->setText(dsx.stor_avail);
    ui->statusbar->addPermanentWidget(statusBarLabel1);
    ui->statusbar->addPermanentWidget(statusBarLabel2);

    //set tab1 as default avTabWidget tab
    ui->avTabWidget->setCurrentIndex(0);
    ui->avTabWidget->setTabText(0, "AV Options");
    ui->avTabWidget->setTabText(1, "Output");

    //Input groupbox
    ui->inputGroupBox->setTitle(tr("Select Sources "));
    ui->inputGroupBox->setAlignment(Qt::AlignLeft);

    //saveAS groupbox
    ui->outputGroupBox->setTitle(tr("Select Destination "));
    ui->outputGroupBox->setAlignment(Qt::AlignLeft);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::application_connections_setup()
{
    connect(ui->SelectSourceWidget, &SelectSourceUI::current_source_extension,
            ui->VideoUIWidget, &VideoUI::receive_source_data);

    connect(ui->VideoUIWidget, &VideoUI::send_output_extension,
            ui->SaveASWidget, &SaveAsUI::receive_output_extension);
}

