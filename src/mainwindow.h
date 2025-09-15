#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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


#include "file_extension_check.h"
#include "input_probe.h"
#include "input_treeview.h"
#include "metadata_checkbox.h"
#include "open_file.h"
#include "parse_data.h"
#include "parsed_data_widget.h"
#include "process_mode_widget.h"
#include "simple_progressbar.h"
#include "storage_data.h"
#include "subtitles_checkbox.h"
#include "transcode_process.h"

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public Q_SLOTS:
    void receive_source_file(const QString &filename);
    void receive_input_probe_data(const QString &video_codec, const QString &video_res,
                                  const QString &video_fps, const QString &video_dar,
                                  const QString &pixel_format, const QString &video_codec_type,
                                  const QString &audio_codec, const QString &audio_channels,
                                  const QString &audio_codec_type);

private:
    Ui::MainWindow *ui;
    FileExtensionCheck extension;
    InputProbe inputprobe;
    InputTreeView treeview;
    TranscodeProcess transcode;
    OpenFile openfile;
    ParseData parsedata;

    //widgets
    ParsedDataWidget datawidget;
    SimpleProgressbar progressbar;
    StorageData storage;
    ProcessModeWidget processModeWidget;
    MetadataCheckBox metadataCheckBox;
    SubtitlesCheckBox subtitlesCheckBox;

    //functions
    void setup_toolbar_settings();
    void setup_action_buttons();
    void setup_radio_buttons();
    void setup_checkboxes();
    void setup_statusbar_widgets();

    //variables
    QString filename{};
};
#endif // MAINWINDOW_H
