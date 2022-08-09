#ifndef FFPROCESS_H
#define FFPROCESS_H

#include <QObject>
#include <QProcess>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class FFprocess;
}
QT_END_NAMESPACE

class FFprocess : public QWidget
{
    Q_OBJECT

public:
    explicit FFprocess(QWidget *parent = nullptr);
    ~FFprocess();

private:
    Ui::FFprocess *ui;

private Q_SLOTS:
    void ffmpeg_proc_output();
    void ffprobe_proc_output();
    /*void display_ffmpeg_output();
    void display_ffprobe_output();*/
    //encoding_finished()

private:
    //location of ffmpeg, ffplay, and ffprobe
    QString ffmpeg_path{};
    QString ffplay_path{};
    QString ffprobe_path{};
    QStringList args{};
    //ffmpeg executables as processes
    QProcess *ffmpeg;
    QProcess *ffprobe;
    QProcess *ffplay;
    //------------------------
    QString ff_output{};
    QString ffprobe_output{};
};

#endif // FFPROCESS_H
