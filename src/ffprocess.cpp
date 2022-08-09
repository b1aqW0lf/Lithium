#include "ffprocess.h"
#include "mainwindow.h"
#include "ui_ffprocess.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextEdit>
#include <QScrollBar>
#include <QToolBar>


FFprocess::FFprocess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FFprocess)
{
    ui->setupUi(this);

    ffmpeg = new QProcess{this};
    ffplay = new QProcess{this};
    ffprobe = new QProcess{this};

    //connect signals and slots
    //connect(ffmpeg, SIGNAL(started()), this, SLOT(ffprocess_running()));
    //
    /*-->>connect(ffmpeg, SIGNAL(finished(int)), this,SLOT(encoding_finished()));<<--*/
    connect(ffmpeg, &QProcess::readyReadStandardOutput,
            this, &FFprocess::ffmpeg_proc_output);
    //connect(ffprobe, SIGNAL(finished(int)), this, SLOT(ffprobe_finished()));//<-
    connect(ffprobe, &QProcess::readyReadStandardOutput,
            this, &FFprocess::ffprobe_proc_output);//ffprobe data
    //-------------------------------------------------------------------------

    //set channel mode
    ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    ffplay->setProcessChannelMode(QProcess::MergedChannels);
    ffprobe->setProcessChannelMode(QProcess::MergedChannels);


    //location of ffmpeg, ffplay, and ffprobe
    args << "-version";
    QString application_path = QCoreApplication::applicationDirPath();
    QString application_dir = QDir(application_path).absolutePath();

#ifdef Q_OS_WIN
    if(QFile::exists(application_dir+"/ffmpeg.exe") &&
            !QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        //Elysium root directory + ffmpeg executable
        ffmpeg_path = application_dir+"/ffmpeg.exe";
        ffmpeg->setWorkingDirectory(application_dir);
    }
    else if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe") &&
            !QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        //Elysium root directory + "ffmpeg" sub directory + ffmpeg executable
        ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
    else if(QFile::exists(application_dir+"/ffmpeg.exe") &&
            QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        //If ffmpeg is found in both root directory and subdirectory
        //use ffmpeg found in user-created "ffmpeg" subdirectory
        ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
        /*ui->statusbar->showMessage(tr("Using ffmpeg found in "+
                                      (application_dir+"/ffmpeg").toUtf8()));*/
    }
    else
    {
        //ui->ffprocessOutputView->setText(tr("FFmpeg executables not detected"));
        //ui->statusbar->showMessage(tr("FFmpeg executables not detected"));
    }
    ffplay_path = "ffplay";//location of ffplay
    ffplay->setWorkingDirectory("");
    ffprobe_path = "ffprobe";//location of ffprobe
    ffprobe->setWorkingDirectory("");
#elif defined Q_OS_LINUX
    ffmpeg_path = "ffmpeg";
    ffplay_path = "ffplay";
    ffprobe_path = "ffprobe";
#endif
    ffmpeg->start(ffmpeg_path, args);/**/
    ffmpeg->waitForStarted();
    //ui->ffprocessOutputView->setText(ff_output);
    args.clear();
    /*MainWindow mwui{this};
    mwui.ui->statusbar->showMessage(tr("Ready"));*/


    //ffprocessOutputView default settings
    ui->ffprocessOutputView->setReadOnly(true);

}

FFprocess::~FFprocess()
{
    delete ui;
    delete ffmpeg;
    delete ffprobe;
    delete ffplay;
}

void FFprocess::ffmpeg_proc_output()//complete!
{
    //ffmpeg process' readyReadStandardOutput implementation
    ff_output = (ffmpeg->readAllStandardOutput());//single line stats
    //ff_output.append(ffmpeg->readAllStandardOutput());
    ui->ffprocessOutputView->setText(ff_output);

    //put scrollbar slider at the right of the textEdit
    ui->ffprocessOutputView->verticalScrollBar()->setSliderPosition(
                ui->ffprocessOutputView->verticalScrollBar()->maximum());
}

void FFprocess::ffprobe_proc_output()
{
    //ffprobe process' readyReadStandardOutput implementation
    ffprobe_output.append(ffprobe->readAllStandardOutput());

    //ff_output.append(ffprobe_output);
    ui->ffprocessOutputView->setText(ffprobe_output);

    //put scrollbar slider at the right of the textEdit
    ui->ffprocessOutputView->verticalScrollBar()->setSliderPosition(
                ui->ffprocessOutputView->verticalScrollBar()->maximum());
}
