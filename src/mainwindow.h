#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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


//#include "ffmpeg_commands_list.h"

#include <QMainWindow>
#include <QWidget>

#include "ffprocess.h"
#include "input_source_probe.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    InputSourceProbe inputProbe;
    FFprocess process;

    //function
    void application_connections_setup();

/*private Q_SLOTS:
    //ffmpeg process data processing
    void ffmpegReadStandardOutput();//ffmpeg QProcess function
    void ffprobeReadStandardOutput();//ffprobe QProcess function
    //-------------------------------------------------------
    //encoding slot methods
    void encoding_started();
    void encoding_finished();
    void encoding_cancelled();
    //-------------------------------------------------------
    //mainwindow ui slot methods
    //encoding, input and output
    void encoder_preset();//encoder
    void select_input1();//inputs
    void select_input2();
    void select_output();//output/save as
    void select_crf();//slider selections
    void select_qscale();
    //-------------------------------------------------------
    //audio selection
    void select_aud_codec();
    void select_bitrate();
    void select_samplerate();
    void select_channels();
    //--------------------------------------------------------
    //video selection
    void select_vid_codec();
    void vid_codec_interface();
    void select_vid_res();
    void select_container();
    void select_aspect_rat();
    void select_vid_fps();
    void select_motion_est();
    //---------------------------------------------------------
    //process mode ui slot methods
    void merge_sources();
    void extract_audio();
    void normal_mode();
    void two_pass_encode();
    //---------------------------------------------------------
    //playback methods
    void play_input1();
    void play_input2();
    void play_output();*/

};
#endif // MAINWINDOW_H
