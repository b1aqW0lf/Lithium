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


#include "abstract_timer.h"


AbstractTimer::AbstractTimer(QChronoTimer *parent)
    : QChronoTimer{parent}
{
    //create the timer
    timer = new QChronoTimer(std::chrono::seconds(1), this);//every second
    this->timer->setTimerType(Qt::PreciseTimer);

    connect(this->timer, &QChronoTimer::timeout, this, &AbstractTimer::process_duration_timer);
}

AbstractTimer::~AbstractTimer()
{
    delete timer;
}

void AbstractTimer::process_duration_timer()
{
    const int timeout{0};
    //--------------------------//
    static const int val{01};
    static const int max{59};
    static const int hour_max{23};
    static const int reset{00};

    //duration
    this->Duration.second = this->Duration.second + val;
    if(this->Duration.second > max)
    {
        //increment seconds back to 00 once seconds pass 59
        //-->00 is the 60th second
        this->Duration.second = reset;
        this->Duration.minute = this->Duration.minute + val;
    }
    if(this->Duration.minute > max)
    {
        //increment minutes back to 00 once minutes pass 59
        //-->00 is the 60th minute
        this->Duration.minute = reset;
        this->Duration.hour = this->Duration.hour + val;
    }
    if(this->Duration.hour > hour_max)
    {
        //increment hours back to 00 once hours pass 23
        //-->00 is the 24th hour
        this->Duration.hour = reset;
    }
    this->duration_time.setHMS(this->Duration.hour, this->Duration.minute, this->Duration.second);
    Q_EMIT this->show_duration_timer("| " + this->duration_time.toString(), timeout);
}

void AbstractTimer::start_timer()
{
    //start the timer
    this->timer->start();
}

void AbstractTimer::stop_timer()
{
    //stop the timer
    this->timer->stop();
}

void AbstractTimer::reset_duration_time()
{
    //resetting the duration_timer hours, minutes, and seconds back to 00:00:00
    static const int reset{00};
    this->Duration.second = reset;
    this->Duration.minute = reset;
    this->Duration.hour = reset;
}

QString AbstractTimer::current_capture_time()
{
    this->current_time = QTime::currentTime();//take the current time
    return current_time.toString();
}
