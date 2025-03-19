#ifndef STATUSBAR_UI_H
#define STATUSBAR_UI_H

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


#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class StatusBarUI;
}
QT_END_NAMESPACE

class StatusBarUI : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBarUI(QWidget *parent = nullptr);
    ~StatusBarUI();

public Q_SLOTS:
    void receive_video_frame_count(const QString &nb_frames);
    void parse_transcode_output(const QString &data);

private:
    struct TrancodeStatus
    {
        QString frame_num{};
        QString frame_fps{};
        QString q_num{};
        QString size{};
        QString frame_time{};
        QString proc_bitrate{};
        QString proc_speed{};
    };

private:
    Ui::StatusBarUI *ui;

    //variable
    QString nb_frames{};

    //progressbar
    void start_progressbar_process(const QString &frames);

    //struct instances
    TrancodeStatus status;
};

#endif // STATUSBAR_UI_H
