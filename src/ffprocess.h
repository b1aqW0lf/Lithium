#ifndef FFPROCESS_H
#define FFPROCESS_H

/******************************************************************************
 Copyright (c) 2020-2022 b1aqW0lf
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
