#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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


//#include "ffmpeg_commands_list.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDial>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QProcess>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QStatusBar>
#include <QStorageInfo>
#include <QTextEdit>
#include <QToolBar>



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
