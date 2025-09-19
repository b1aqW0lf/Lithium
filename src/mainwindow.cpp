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


#include "mainwindow.h"
#include "src/ui_mainwindow.h"

#include "src/audio_interface.h"
#include "src/input_treeview.h"
#include "src/video_interface.h"

#include <QFileInfo>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_WINDOWS
    //set slider style
    ui->horizontalSlider_2->setStyle(QStyleFactory::create("windowsvista"));
#endif

    this->setup_toolbar_settings();
    this->setup_action_buttons();
    this->setup_radio_buttons();
    this->setup_checkboxes();

    connect(ui->actionOpenFile, &QAction::triggered, &openfile, &OpenFile::open_source_file);
    connect(ui->actionEncode, &QAction::triggered, &transcode, &TranscodeProcess::start_transcoding_process);
    connect(ui->actionCancel, &QAction::triggered, &transcode, &TranscodeProcess::cancel_transcoding_process);
    connect(ui->actionCancel, &QAction::triggered, &progressbar, &SimpleProgressbar::cancel_progressbar_process);
    connect(ui->actionCancel, &QAction::triggered, &datawidget, &ParsedDataWidget::clear_parsed_data_info);
    connect(&openfile, &OpenFile::send_source_video_file, this, &MainWindow::receive_source_file);
    connect(&openfile, &OpenFile::send_source_video_file, &transcode, &TranscodeProcess::receive_source_file);
    connect(&openfile, &OpenFile::send_source_video_file, &inputprobe, &InputProbe::receive_source_file);
    connect(&openfile, &OpenFile::send_source_video_file, ui->inputTreeWidget, &InputTreeView::receive_source_file);
    connect(&openfile, &OpenFile::send_source_video_file, &extension, &FileExtensionCheck::receive_source_video_file);
    connect(&openfile, &OpenFile::get_current_process_mode, &processModeWidget, &ProcessModeWidget::send_current_process_mode);//new
    connect(&extension, &FileExtensionCheck::send_source_video_file_extension,
            ui->videoInterfaceWidget, &VideoInterface::receive_source_video_file_extension);//new
    connect(&transcode, &TranscodeProcess::send_ffprobe_output, &progressbar, &SimpleProgressbar::receive_ffprobe_frames_value);
    connect(&transcode, &TranscodeProcess::send_ffmpeg_output, &parsedata, &ParseData::parse_file_data);
    connect(&parsedata, &ParseData::send_frame_num_update, &progressbar, &SimpleProgressbar::receive_frame_num_value);
    connect(&parsedata, &ParseData::send_parsed_data, &datawidget, &ParsedDataWidget::receive_parsed_data);
    connect(&transcode, &TranscodeProcess::send_transcode_process_message, ui->statusbar, &QStatusBar::showMessage);
    connect(&processModeWidget, &ProcessModeWidget::current_process_mode_status, ui->statusbar, &QStatusBar::showMessage);
    connect(&processModeWidget, &ProcessModeWidget::current_process_mode, &treeview, &InputTreeView::current_process_mode);//new
    connect(&processModeWidget, &ProcessModeWidget::current_process_mode, &openfile, &OpenFile::current_process_mode);//new
    connect(&inputprobe, &InputProbe::send_input_probe_data, this, &MainWindow::receive_input_probe_data);
    connect(&inputprobe, &InputProbe::send_input_probe_data, ui->inputTreeWidget, &InputTreeView::receive_input_probe_data);
    connect(&inputprobe, &InputProbe::send_source_file_audio_data, ui->audioInterfaceWidget, &AudioInterface::receive_source_file_audio_data);//new
    connect(&inputprobe, &InputProbe::send_source_file_video_data, ui->videoInterfaceWidget, &VideoInterface::receive_source_file_video_data);//new
    connect(ui->audioInterfaceWidget, &AudioInterface::send_audio_statusbar_message, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->videoInterfaceWidget, &VideoInterface::send_video_statusbar_message, ui->statusbar, &QStatusBar::showMessage);//new

    //statusbar widgets
    this->setup_statusbar_widgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup_toolbar_settings()
{
    //toolbar settings
    ui->toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolbar->setIconSize(QSize(28,28));
    ui->toolbar->setMovable(false);
    ui->toolbar->setFloatable(false);
    //--------------------------------------------//
    //enable action bar
    ui->toolbar->addAction(ui->actionOpenFile);
    ui->toolbar->addSeparator();
    ui->toolbar->addAction(ui->actionEncode);
    ui->toolbar->addAction(ui->actionCancel);
}

void MainWindow::setup_action_buttons()
{
    ui->actionOpenFile->setIcon(QIcon(":/images/resources/actionOpenFile.svg"));
    ui->actionEncode->setIcon(QIcon(":/images/resources/actionEncode.svg"));
    ui->actionCancel->setIcon(QIcon(":/images/resources/actionCancel.svg"));
    ui->actionEncode->setToolTip(tr("Start Encoding"));
    ui->actionCancel->setToolTip(tr("Cancel Encoding"));
}

void MainWindow::setup_radio_buttons()
{
    ui->toolbar->addSeparator();
    ui->toolbar->addWidget(&processModeWidget);
}

void MainWindow::setup_checkboxes()
{
    ui->toolbar->addSeparator();
    ui->toolbar->addWidget(&metadataCheckBox);
    ui->toolbar->addWidget(&subtitlesCheckBox);
}

void MainWindow::setup_statusbar_widgets()
{
    ui->statusbar->addPermanentWidget(&datawidget);
    ui->statusbar->addPermanentWidget(&progressbar);
    ui->statusbar->addPermanentWidget(&storage);
}

void MainWindow::receive_source_file(const QString &filename)
{
    this->filename = filename;
}

void MainWindow::receive_input_probe_data(const QString &video_codec, const QString &video_res,
                                          const QString &video_fps, const QString &video_dar,
                                          const QString &pixel_format, const QString &video_codec_type,
                                          const QString &audio_codec, const QString &audio_channels,
                                          const QString &audio_codec_type)
{

    QFileInfo file(filename);
    //display received data
    ui->sourceInfoLabel->setText("Source: " + file.completeBaseName() + ", "
                                 + video_codec.toUpper() + ", " + video_res + ", " + video_fps + " fps, "
                                 + video_dar + ", " + pixel_format + ", " + audio_codec.toUpper() + ", "
                                 + audio_channels + " channels");
}
