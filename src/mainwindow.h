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


//#include "ffmpeg_commands_list.h"

#include <QLabel>
#include <QMainWindow>
#include <QWidget>

#include "detect_ffmpeg.h"
#include "input_source_probe.h"
#include "statusbar_ui.h"
#include "transcode.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

Q_SIGNALS:
    void start_encode_process();
    //test
    void encode_mode_check();
    void cancel_encode_process();

public Q_SLOTS:
    void enable_encode_button();

private Q_SLOTS:
    void start_action_encode();
    void cancel_action_encode();

private:
    Ui::MainWindow *ui;
    QLabel *statusBarLabel1, *statusBarLabel2;
    StatusBarUI statProgressBar;
    InputSourceProbe inputProbe;
    DetectFFmpeg detectFFmpeg;
    Transcode transcoder;

    //variables
    bool normal_mode_val{};

    //function
    void local_connections_setup();
    void application_connections_setup();
    void transcoder_connections_setup();
};
#endif // MAINWINDOW_H
