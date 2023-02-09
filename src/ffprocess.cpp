#include "ffprocess.h"

FFprocess::FFprocess(QProcess *parent)
    : QProcess{parent}
{
    //create the processes to be used
    this->ffmpeg = new QProcess;
    this->ffplay = new QProcess;
    this->ffprobe = new QProcess;


    //connect signals and slots
    //connect(ffmpeg, SIGNAL(started()), this, SLOT(ffprocess_running()));
    connect(this->ffmpeg, &QProcess::readyReadStandardOutput,
            this, &FFprocess::ffmpegReadStandardOutput);

    //connect(ffprobe, SIGNAL(finished(int)), this, SLOT(ffprobe_finished()));
    connect(this->ffprobe, &QProcess::readyReadStandardOutput,
            this, &FFprocess::ffprobeReadStandardOutput);//ffprobe data

    connect(this->ffmpeg, &QProcess::started,
            this, &FFprocess::ffmpeg_process_started);

    connect(this->ffmpeg, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &FFprocess::ffmpeg_process_finished);

    connect(this->ffprobe, &QProcess::started,
            this, &FFprocess::ffprobe_process_started);

    connect(this->ffprobe, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &FFprocess::ffprobe_process_finished);

    //set process channel
    ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    ffplay->setProcessChannelMode(QProcess::MergedChannels);
    ffprobe->setProcessChannelMode(QProcess::MergedChannels);

    //location of ffmpeg, ffplay, and ffprobe
    ffmpeg_location_setup();
    ffprobe_location_setup();
    ffplay_location_setup();

    //loads ffmpeg on startup
    args << "-version";
    ffmpeg->start(ffmpeg_path, args);
    ffmpeg->waitForStarted();
    args.clear();

}

FFprocess::~FFprocess()
{
    delete ffmpeg;
    delete ffprobe;
    delete ffplay;
}

void FFprocess::ffmpegReadStandardOutput()
{
    QString ffmpeg_output{};
    //ffmpeg process' readyReadStandardOutput implementation
    ffmpeg_output = this->ffmpeg->readAllStandardOutput();//single line stats
    emit ffmpeg_read_output(ffmpeg_output);
}

void FFprocess::ffprobeReadStandardOutput()
{
    QString ffprobe_output{};
    //ffprobe process' readyReadStandardOutput implementation
    ffprobe_output = this->ffprobe->readAllStandardOutput();//single line stats
    emit ffprobe_read_output(ffprobe_output);
}

void FFprocess::ffmpeg_location_setup()
{

#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe") &&
            !QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        //Lithium root directory + ffmpeg executable
        this->ffmpeg_path = application_dir+"/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
    }
    else if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe") &&
            !QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        //Lithium root directory + "ffmpeg" sub directory + ffmpeg executable
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
    else if(QFile::exists(application_dir+"/ffmpeg.exe") &&
             QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        //If ffmpeg is found in both root directory and subdirectory
        //use ffmpeg found in user-created "ffmpeg" subdirectory
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
        //ui->statusbar->showMessage(tr("Using ffmpeg found in "+(application_dir+"/ffmpeg").toUtf8()));
        emit ffmpeg_found("Using ffmpeg found in "+(application_dir+"/ffmpeg").toUtf8()));
    }
    else
    {
        /*ui->ffProcWindow->setText(tr("FFmpeg executables not detected"));
        ui->statusbar->showMessage(tr("FFmpeg executables not detected"));*/
        emit ffmpeg_found("FFmpeg executables not detected");
    }
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif
    int message_timeout{0};//status bar message timeout value
    emit ffmpeg_ready_status("Ready", message_timeout);

}

void FFprocess::ffprobe_location_setup()
{

#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffprobe.exe") &&
            !QFile::exists(application_dir+"/ffmpeg/ffprobe.exe"))
    {
        //Lithium root directory + ffprobe executable
        this->ffprobe_path = application_dir+"/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir);
    }
    else if(QFile::exists(application_dir+"/ffmpeg/ffprobe.exe") &&
            !QFile::exists(application_dir+"/ffprobe.exe"))
    {
        //Lithium root directory + "ffmpeg" sub directory + ffprobe executable
        this->ffprobe_path = application_dir+"/ffmpeg/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir+"/ffmpeg");
    }
    else if(QFile::exists(application_dir+"/ffprobe.exe") &&
             QFile::exists(application_dir+"/ffmpeg/ffprobe.exe"))
    {
        //If ffprobe is found in both root directory and subdirectory
        //use ffprobe found in user-created "ffmpeg" subdirectory
        this->ffprobe_path = application_dir+"/ffmpeg/ffprobe.exe";
        this->ffprobe->setWorkingDirectory(application_dir+"/ffmpeg");
        //ui->statusbar->showMessage(tr("Using ffprobe found in "+(application_dir+"/ffmpeg").toUtf8()));
        emit ffprobe_found("Using ffprobe found in "+(application_dir+"/ffmpeg").toUtf8()));
    }
    else
    {
        /*ui->ffProcWindow->setText(tr("FFprobe executable not detected"));
        ui->statusbar->showMessage(tr("FFprobe executable not detected"));*/
        emit ffprobe_found("FFprobe executable not detected");
    }
#elif defined Q_OS_LINUX
    this->ffprobe_path = "ffprobe";
#endif

}

void FFprocess::ffplay_location_setup()
{

#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffplay.exe") &&
            !QFile::exists(application_dir+"/ffmpeg/ffplay.exe"))
    {
        //Lithium root directory + ffplay executable
        this->ffplay_path = application_dir+"/ffplay.exe";
        this->ffplay->setWorkingDirectory(application_dir);
    }
    else if(QFile::exists(application_dir+"/ffmpeg/ffplay.exe") &&
            !QFile::exists(application_dir+"/ffplay.exe"))
    {
        //Lithium root directory + "ffmpeg" sub directory + ffplay executable
        this->ffplay_path = application_dir+"/ffmpeg/ffplay.exe";
        this->ffplay->setWorkingDirectory(application_dir+"/ffmpeg");
    }
    else if(QFile::exists(application_dir+"/ffplay.exe") &&
             QFile::exists(application_dir+"/ffmpeg/ffplay.exe"))
    {
        //If ffplay is found in both root directory and subdirectory
        //use ffplay found in user-created "ffmpeg" subdirectory
        this->ffplay_path = application_dir+"/ffmpeg/ffplay.exe";
        this->ffplay->setWorkingDirectory(application_dir+"/ffmpeg");
        //ui->statusbar->showMessage(tr("Using ffplay found in "+(application_dir+"/ffmpeg").toUtf8()));
        emit ffmpeg_found("Using ffplay found in "+(application_dir+"/ffmpeg").toUtf8()));
    }
    else
    {
        /*ui->ffProcWindow->setText(tr("FFplay executable not detected"));
        ui->statusbar->showMessage(tr("FFplay executable not detected"));*/
        emit ffplay_found("FFplay executable not detected");
    }
#elif defined Q_OS_LINUX
    this->ffplay_path = "ffplay";
#endif

}



void FFprocess::ffmpeg_process_started()
{
    ffmpeg->waitForStarted();
    //check if ffmpeg process has started
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        emit ffmpeg_started();
    }
}

void FFprocess::ffmpeg_process_finished()
{
    ffmpeg->waitForFinished();
    if(this->ffmpeg->QProcess::state() == QProcess::NotRunning)
    {
        emit ffmpeg_finished();

        //close write channel after process finishes
        ffmpeg->closeWriteChannel();
    }
}

void FFprocess::ffprobe_process_started()
{
    ffprobe->waitForStarted();
    //check if ffmpeg process has started
    if(this->ffprobe->QProcess::state() == QProcess::Running)
    {
        emit ffprobe_started();
    }
}

void FFprocess::ffprobe_process_finished()
{
    ffprobe->waitForFinished();
    if(this->ffprobe->QProcess::state() == QProcess::NotRunning)
    {
        emit ffprobe_finished();

        //close write channel after process finishes
        ffprobe->closeWriteChannel();
    }
}
