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


#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QScrollBar>

#include "detect_storage.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/audio_ui.h"
#include "src/output_display_ui.h"
#include "src/process_mode_ui.h"
#include "src/save_as_ui.h"
#include "src/select_source_ui.h"
#include "src/statusbar_ui.h"
#include "src/transcode.h"
#include "src/video_ui.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //enable application connections
    local_connections_setup();
    application_connections_setup();
    transcoder_connections_setup();

    //enable action bar
    ui->toolbar->addAction(ui->actionEncode);
    ui->toolbar->addAction(ui->actionCancel);
    ui->toolbar->addSeparator();
    ui->actionEncode->setCheckable(true);
    ui->actionEncode->setText(tr("Encode"));
    ui->actionCancel->setText(tr("Cancel"));
    ui->actionEncode->setIconText(tr("Encode"));
    ui->actionCancel->setIconText(tr("Cancel"));
    ui->actionEncode->setShortcut(Qt::CTRL+Qt::Key_E);
    ui->actionCancel->setShortcut(Qt::CTRL+Qt::Key_X);
    ui->actionEncode->setToolTip(tr("Start Encoding"));
    ui->actionCancel->setToolTip(tr("Cancel Encoding"));


    //display progress bar and statusbar
    StatusBarUI *statProgressBar = new StatusBarUI;
    ui->statusbar->addPermanentWidget(statProgressBar);

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

void MainWindow::local_connections_setup()
{
    connect(ui->actionEncode, &QAction::triggered,
            this, &MainWindow::start_action_encode);
}

void MainWindow::application_connections_setup()
{
    connect(ui->SelectSourceWidget, &SelectSourceUI::current_vid_source_extension,
            ui->VideoUIWidget, &VideoUI::receive_vid_source_extension);

    connect(ui->VideoUIWidget, &VideoUI::send_output_vid_extension,
            ui->SaveASWidget, &SaveAsUI::receive_output_extension);

    connect(ui->SelectSourceWidget, &SelectSourceUI::current_audio_source_extension,
            ui->AudioUIWidget, &AudioUI::receive_audio_source_extension);

    connect(ui->AudioUIWidget, &AudioUI::send_output_audio_extension,
            ui->SaveASWidget, &SaveAsUI::receive_output_extension);

    /*connect(ui->SelectSourceWidget, &SelectSourceUI::current_vid_source_file,
            &inputProbe, &InputSourceProbe::receive_vid_file_path);*/

    /*connect(ui->SelectSourceWidget, &SelectSourceUI::current_audio_source_file,
            &inputProbe, &InputSourceProbe::receive_audio_file_path);*/

    connect(ui->SelectSourceWidget, &SelectSourceUI::current_vid_source_file,
            &inputProbe, &InputSourceProbe::start_probe_process);

    //temp connection-for testing only-----------------------------//
    connect(&inputProbe, &InputSourceProbe::show_vid_data,
            ui->statusbar, &QStatusBar::showMessage);

    /*connect(&inputProbe, &InputSourceProbe::show_audio_data,
            ui->statusbar, &QStatusBar::showMessage);*/
    //-------------------------------------------------------------//

    /*connect(&process, &FFprocess::ffmpeg_ready_status,
            ui->statusbar, &QStatusBar::showMessage);*/

    connect(&process, &FFprocess::ffmpeg_read_output,
            ui->OuputDisplayWidget, &OutputDisplayUI::textEdit_display_output);

    connect(&process, &FFprocess::ffprobe_read_output,
            ui->OuputDisplayWidget, &OutputDisplayUI::textEdit_display_output);

    //experimental
    connect(&inputProbe, &InputSourceProbe::ffprobe_started_message,
            ui->statusbar, &QStatusBar::showMessage);

    //temp connection-for testing only--------------------------------//

    connect(ui->VideoUIWidget, &VideoUI::send_vid_data,
            ui->statusbar, &QStatusBar::showMessage);

    connect(ui->AudioUIWidget, &AudioUI::send_audio_data,
            ui->statusbar, &QStatusBar::showMessage);

    //------------------------------------------------------------------//

    connect(&inputProbe, &InputSourceProbe::source_vid_resolution,
            ui->VideoUIWidget, &VideoUI::receive_vid_source_resolution);

    connect(&inputProbe, &InputSourceProbe::source_vid_codec_name,
            ui->VideoUIWidget, &VideoUI::receive_vid_source_codec);

    connect(&inputProbe, &InputSourceProbe::source_vid_frame_rate,
            ui->VideoUIWidget, &VideoUI::receive_vid_source_framerate);

    connect(&inputProbe, &InputSourceProbe::source_vid_bit_rate,
            ui->VideoUIWidget, &VideoUI::receive_vid_source_bitrate);

    connect(&inputProbe, &InputSourceProbe::source_vid_display_aspect_ratio,
            ui->VideoUIWidget, &VideoUI::receive_vid_source_display_aspect_ratio);

    //----------------------------------------------------------------------//

    connect(&inputProbe, &InputSourceProbe::source_audio_codec_name,
            ui->AudioUIWidget, &AudioUI::receive_audio_source_codec);

    connect(&inputProbe, &InputSourceProbe::source_audio_bitrate,
            ui->AudioUIWidget, &AudioUI::receive_audio_source_bitrate);

    connect(&inputProbe, &InputSourceProbe::source_audio_samplerate,
            ui->AudioUIWidget, &AudioUI::receive_audio_source_samplerate);

    connect(&inputProbe, &InputSourceProbe::source_audio_channels,
            ui->AudioUIWidget, &AudioUI::receive_audio_source_channels);

    //----------------------------------------------------------------------//

    connect(ui->SelectSourceWidget, &SelectSourceUI::clear_input1_data,
            ui->VideoUIWidget, &VideoUI::receive_clear_request);

    //----------------------------------------------------------------------//

    connect(ui->SelectSourceWidget, &SelectSourceUI::current_vid_source_file,
            &transcoder, &Transcode::receive_source_video_file);

    connect(&transcoder, &Transcode::source_vid_file_status,
            ui->statusbar, &QStatusBar::showMessage);

    connect(&transcoder, &Transcode::output_vid_file_status,
            ui->statusbar, &QStatusBar::showMessage);

    connect(ui->SaveASWidget, &SaveAsUI::send_output_file_path,
            &transcoder, &Transcode::receive_output_video_path);

    //------------------------------------------------------------------------//

    connect(ui->ProcessModeUIWidget, &ProcessModeUI::enable_normal_mode_processing,
            this, &MainWindow::normal_mode_enabled);

    connect(this, &MainWindow::start_normal_mode_process,
            &transcoder, &Transcode::start_normal_mode_transcode);

    //--------------------------------------------------------------------------//

    connect(ui->VideoUIWidget, &VideoUI::two_pass_encode_enabled,
            &transcoder, &Transcode::enable_two_pass_encode);

    connect(ui->VideoUIWidget, &VideoUI::send_average_bitrate_value,
            &transcoder, &Transcode::receive_vid_avg_bitrate);
}

void MainWindow::transcoder_connections_setup()
{
    //transcoding connections----------------------------------------------------//
    //video transcoding
    connect(ui->VideoUIWidget, &VideoUI::send_video_codec_name,
            &transcoder, &Transcode::receive_video_codec_name);

    connect(ui->VideoUIWidget, &VideoUI::send_video_crf_val,
            &transcoder, &Transcode::receive_video_crf_val);

    connect(ui->VideoUIWidget, &VideoUI::send_video_qscale_val,
            &transcoder, &Transcode::receive_video_qscale_val);

    connect(ui->VideoUIWidget, &VideoUI::send_video_resolution_value,
            &transcoder, &Transcode::receive_video_res_value);

    connect(ui->VideoUIWidget, &VideoUI::send_vid_display_aspect_ratio_val,
            &transcoder, &Transcode::receive_video_dar_value);

    connect(ui->VideoUIWidget, &VideoUI::send_video_framerate_value,
            &transcoder, &Transcode::receive_video_framerate_val);

    //audio transcoding
    connect(ui->AudioUIWidget, &AudioUI::send_audio_codec_name,
            &transcoder, &Transcode::receive_audio_codec_name);

    connect(ui->AudioUIWidget, &AudioUI::send_audio_channels_val,
            &transcoder, &Transcode::receive_audio_channels_val);
}

void MainWindow::normal_mode_enabled(const bool &value)
{
    this->normal_mode_val = value;
}

void MainWindow::start_action_encode()
{
    if(this->normal_mode_val == true)
    {
        Q_EMIT start_normal_mode_process();
    }
}
