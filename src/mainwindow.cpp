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
#include "src/video_avg_bitrate_field.h"
#include "src/video_interface.h"
#include "src/video_crf_interface.h"
#include "src/video_encoder_preset.h"

#include <QButtonGroup>
#include <QFileInfo>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setup_toolbar_settings();
    this->setup_action_buttons();
    this->setup_radio_buttons();
    this->setup_checkboxes();

    connect(ui->actionOpenFile, &QAction::triggered, &openfile, &OpenFile::open_source_file);
    connect(ui->actionCancel, &QAction::triggered, &transcode, &TranscodeProcess::cancel_transcoding_process);
    connect(ui->actionCancel, &QAction::triggered, &progressbar, &SimpleProgressbar::cancel_progressbar_process);
    connect(ui->actionCancel, &QAction::triggered, &datawidget, &ParsedDataWidget::clear_parsed_data_info);
    connect(&openfile, &OpenFile::send_source_video_file, this, &MainWindow::receive_source_file);
    connect(&openfile, &OpenFile::send_source_video_file, &transcode, &TranscodeProcess::receive_source_file);
    connect(&openfile, &OpenFile::send_source_video_file, &inputprobe, &InputProbe::receive_source_file);
    connect(&openfile, &OpenFile::send_source_video_file, ui->inputTreeWidget, &InputTreeView::receive_source_file);
    connect(&openfile, &OpenFile::send_source_video_file, ui->saveAsWidget, &SaveAsField::receive_input_file_name);
    connect(&openfile, &OpenFile::get_current_process_mode, &processModeWidget, &ProcessModeWidget::send_current_process_mode);//new
    connect(&transcode, &TranscodeProcess::send_ffprobe_output, &progressbar, &SimpleProgressbar::receive_ffprobe_frames_value);
    connect(&transcode, &TranscodeProcess::send_ffmpeg_output, &parsedata, &ParseData::parse_file_data);
    connect(&parsedata, &ParseData::send_frame_num_update, &progressbar, &SimpleProgressbar::receive_frame_num_value);
    connect(&parsedata, &ParseData::send_parsed_data, &datawidget, &ParsedDataWidget::receive_parsed_data);
    connect(&transcode, &TranscodeProcess::send_transcode_process_message, ui->statusbar, &QStatusBar::showMessage);
    connect(&processModeWidget, &ProcessModeWidget::current_process_mode_status, ui->statusbar, &QStatusBar::showMessage);
    connect(&processModeWidget, &ProcessModeWidget::current_process_mode, &treeview, &InputTreeView::current_process_mode);//new
    connect(&processModeWidget, &ProcessModeWidget::current_process_mode, &openfile, &OpenFile::current_process_mode);//new
    connect(&processModeWidget, &ProcessModeWidget::current_process_mode, ui->audioInterfaceWidget, &AudioInterface::current_process_mode);
    connect(&processModeWidget, &ProcessModeWidget::current_process_mode, ui->saveAsWidget, &SaveAsField::current_process_mode);//new
    connect(&inputprobe, &InputProbe::send_input_probe_data, this, &MainWindow::receive_input_probe_data);
    connect(&inputprobe, &InputProbe::send_input_probe_data, ui->inputTreeWidget, &InputTreeView::receive_input_probe_data);
    connect(&inputprobe, &InputProbe::send_source_file_audio_data, ui->audioInterfaceWidget, &AudioInterface::receive_source_file_audio_data);//new
    connect(&inputprobe, &InputProbe::send_source_file_video_data, ui->videoInterfaceWidget, &VideoInterface::receive_source_file_video_data);//new
    connect(&inputprobe, &InputProbe::send_source_video_colorspace_data,
            ui->videoInterfaceWidget, &VideoInterface::receive_source_video_colorspace_data);
    connect(&inputprobe, &InputProbe::send_source_video_bitrate, ui->videoAVGBitrateWidget, &VideoAVGBitrateField::receive_source_video_bitrate);//new
    connect(&inputprobe, &InputProbe::send_video_codec_profile_level,
            ui->videoOptionsWidget, &VideoEncoderOptions::receive_video_codec_profile_level);//new
    connect(ui->audioInterfaceWidget, &AudioInterface::send_audio_statusbar_message, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->videoInterfaceWidget, &VideoInterface::send_video_statusbar_message, ui->statusbar, &QStatusBar::showMessage);//new
    connect(ui->videoInterfaceWidget, &VideoInterface::send_selected_video_codec_name,
            ui->videoCRFWidget, &VideoCRFInterface::receive_selected_video_codec_name);//new
    connect(ui->videoCRFWidget, &VideoCRFInterface::send_statusbar_message, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->videoInterfaceWidget, &VideoInterface::send_selected_video_codec_name,
            ui->videoPresetWidget, &VideoEncoderPreset::receive_selected_video_codec_name);
    connect(ui->videoInterfaceWidget, &VideoInterface::send_selected_video_codec_name,
            ui->videoOptionsWidget, &VideoEncoderOptions::receive_selected_video_codec_name);
    connect(ui->videoPresetWidget, &VideoEncoderPreset::send_statusbar_message, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->videoAVGBitrateWidget, &VideoAVGBitrateField::send_avg_bitrate_statusbar_message, ui->statusbar, &QStatusBar::showMessage);//new
    connect(ui->videoOptionsWidget, &VideoEncoderOptions::send_statusbar_message, ui->statusbar, &QStatusBar::showMessage);
    connect(this, &MainWindow::button_group_signal, ui->videoAVGBitrateWidget, &VideoAVGBitrateField::set_avg_bitrate_button_mode);//new
    connect(this, &MainWindow::button_group_signal, ui->videoCRFWidget, &VideoCRFInterface::set_crf_button_mode);//new
    connect(ui->saveAsWidget, &SaveAsField::send_save_field_statusbar_message, ui->statusbar, &QStatusBar::showMessage);//new
    connect(&encoderAvail, &EncoderAvailability::send_encoder_availability_message, ui->statusbar, &QStatusBar::showMessage);//new
    connect(&metadataCheckBox, &MetadataCheckBox::send_metadata_checkbox_statusbar_message, ui->statusbar, &QStatusBar::showMessage);
    connect(&subtitlesCheckBox, &SubtitlesCheckBox::send_subtitles_checkbox_statusbar_message, ui->statusbar, &QStatusBar::showMessage);

    connect(ui->actionEncode, &QAction::triggered, &inputHandler, &InputHandler::send_input_selections_request);
    connect(&inputHandler, &InputHandler::request_input_selections, &subtitlesCheckBox, &SubtitlesCheckBox::get_copy_subtitles_selection);
    connect(&inputHandler, &InputHandler::request_input_selections, ui->videoInterfaceWidget, &VideoInterface::get_video_interface_selections);
    connect(&inputHandler, &InputHandler::request_input_selections, ui->videoCRFWidget, &VideoCRFInterface::get_video_crf_qscale_selection);
    connect(&inputHandler, &InputHandler::request_input_selections, ui->videoPresetWidget, &VideoEncoderPreset::get_video_encoder_preset_selection);
    connect(&inputHandler, &InputHandler::request_input_selections, ui->videoOptionsWidget, &VideoEncoderOptions::get_video_encoder_options_selections);
    connect(&inputHandler, &InputHandler::request_input_selections, ui->videoAVGBitrateWidget, &VideoAVGBitrateField::get_video_bitrate_selections);
    connect(&inputHandler, &InputHandler::request_input_selections, ui->audioInterfaceWidget, &AudioInterface::get_audio_interface_selections);
    connect(&inputHandler, &InputHandler::request_input_selections, &metadataCheckBox, &MetadataCheckBox::get_copy_metadata_selection);
    connect(&inputHandler, &InputHandler::request_input_selections, ui->saveAsWidget, &SaveAsField::send_selected_output_path);

    connect(&subtitlesCheckBox, &SubtitlesCheckBox::send_copy_subtitles_command,
            &inputHandler, &InputHandler::receive_copy_subtitles_command);
    connect(ui->videoInterfaceWidget, &VideoInterface::send_video_interface_selections,
            &inputHandler, &InputHandler::receive_input_video_selection);
    connect(ui->videoCRFWidget, &VideoCRFInterface::send_video_crf_qscale_selection,
            &inputHandler, &InputHandler::receive_input_crf_qscale_selection);
    connect(ui->videoPresetWidget, &VideoEncoderPreset::send_video_preset_selection,
            &inputHandler, &InputHandler::receive_input_video_preset_selection);
    connect(ui->videoOptionsWidget, &VideoEncoderOptions::send_video_options_selections,
            &inputHandler, &InputHandler::receive_video_options_selections);
    connect(ui->videoAVGBitrateWidget, &VideoAVGBitrateField::send_video_avg_bitrate_selections,
            &inputHandler, &InputHandler::receive_video_avg_bitrate_selections);
    connect(ui->audioInterfaceWidget, &AudioInterface::send_audio_interface_selections,
            &inputHandler, &InputHandler::receive_input_audio_selections);
    connect(&metadataCheckBox, &MetadataCheckBox::send_copy_metadata_command,
            &inputHandler, &InputHandler::receive_copy_metadata_command);
    connect(ui->saveAsWidget, &SaveAsField::send_output_file_path,
            &transcode, &TranscodeProcess::receive_output_file_path);

    connect(ui->actionEncode, &QAction::triggered, &inputHandler, &InputHandler::send_received_selected_inputs);
    connect(&inputHandler, &InputHandler::send_selected_input_parameters, &transcode, &TranscodeProcess::start_transcoding_process);

    //statusbar widgets
    this->setup_statusbar_widgets();
    this->setup_button_group();

    //ready status
    this->setup_ready_status();
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

void MainWindow::setup_button_group()
{
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(ui->videoCRFWidget->video_crf_button_widget());
    buttonGroup->addButton(ui->videoAVGBitrateWidget->video_avg_bitrate_bttn_widget());

    ui->videoCRFWidget->video_crf_button_widget()->setChecked(true);
    connect(buttonGroup, &QButtonGroup::buttonClicked, this, &MainWindow::group_button_clicked);
}

void MainWindow::setup_ready_status()
{
    encoderAvail.set_encoder_availablity();
    encoderAvail.send_encoder_ready_status();
}

void MainWindow::group_button_clicked()
{
    Q_EMIT this->button_group_signal();
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
