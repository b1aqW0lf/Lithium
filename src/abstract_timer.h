#ifndef ABSTRACT_TIMER_H
#define ABSTRACT_TIMER_H


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


#include <QChronoTimer>
#include <QObject>
#include <QTime>


class AbstractTimer : public QChronoTimer
{
    Q_OBJECT

public:
    explicit AbstractTimer(QChronoTimer *parent = nullptr);
    ~AbstractTimer();

Q_SIGNALS:
    void show_duration_timer(const QString &timer_message, const int &timeout);

public:
    //functions
    void start_timer();
    void stop_timer();
    void reset_duration_time();
    QString capture_current_time();

private Q_SLOTS:
    void process_duration_timer();

private:
    //time
    QTime current_time{};//used to check the current time
    QTime start_time = QTime(00,00,00);
    QChronoTimer *timer;

    struct DurationTime
    {
        int second{00};
        int minute{00};
        int hour{00};

        /*void reset_duration_time()
        {
            second = 00;
            minute = 00;
            hour = 00;
        }*/
    }Duration;
};

#endif // ABSTRACT_TIMER_H
