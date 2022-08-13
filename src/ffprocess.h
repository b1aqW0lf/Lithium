#ifndef FFPROCESS_H
#define FFPROCESS_H

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
