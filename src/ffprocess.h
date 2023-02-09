#ifndef FFPROCESS_H
#define FFPROCESS_H

#include <QProcess>
#include <QWidget>


class FFprocess : public QProcess
{
    Q_OBJECT

    friend class OutputDisplayUI;

public:
    explicit FFprocess(QProcess *parent = nullptr);
    ~FFprocess();

public:
    //ffmpeg executables as processes
    QProcess *ffmpeg; //transcodes, encodes, records, and streams files
    QProcess *ffprobe;//examines input and output files
    QProcess *ffplay; //play input and output files

Q_SIGNALS:
    void ffmpeg_read_output(const QString &output);
    void ffprobe_read_output(const QString &output);
    void ffmpeg_found(const QString &status);
    void ffprobe_found(const QString &status);
    void ffplay_found(const QString &status);
    void ffmpeg_ready_status(const QString &status, const int &num);
    void ffmpeg_started();
    void ffprobe_started();
    void ffmpeg_finished();
    void ffprobe_finished();

    //not impelemented yet
    /*void ffmpeg_cancelled();
    void ffprobe_cancelled();*/

private Q_SLOTS:
    void ffmpegReadStandardOutput();//ffmpeg QProcess function
    void ffprobeReadStandardOutput();//ffprobe QProcess function
    void ffmpeg_process_started();
    void ffmpeg_process_finished();
    void ffprobe_process_started();
    void ffprobe_process_finished();

    //not impelemented yet
    /*void ffmpeg_process_cancelled();
    void ffprobe_process_cancelled();*/

private:
    //ffmpeg status functions
    void ffmpeg_location_setup();
    void ffprobe_location_setup();
    void ffplay_location_setup();

    //ffplay output strings
    QString ffplay_output{};

    //ffmpeg argument
    QStringList args{};

    //ffmpeg file path -> path to ffmpeg
    QString ffmpeg_path{};
    QString ffprobe_path{};
    QString ffplay_path{};
};

#endif // FFPROCESS_H
