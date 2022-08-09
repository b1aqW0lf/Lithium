#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
