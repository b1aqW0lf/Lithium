#ifndef ENCODER_AVAILABILITY_H
#define ENCODER_AVAILABILITY_H

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
***********************************************************************************/


#include "encoder_process.h"

#include <QObject>


class EncoderAvailability : public QObject
{
    Q_OBJECT

public:
    explicit EncoderAvailability(QObject *parent = nullptr);
    ~EncoderAvailability();

Q_SIGNALS:
    void send_encoder_availability_message(const QString &message, const int &timeout);

public:
    void send_encoder_ready_status();
    void set_encoder_availablity();

private:
    EncoderProcess enc_process;

    //functions
    void get_encoder_availability();
    void check_encoder_path();
    void send_availability_signal();
    void make_encoder_available();

    //structs
    struct EncoderFound{
        bool ffmpeg_found{false};
        bool ffprobe_found{false};
    }encoder;
};

#endif // ENCODER_AVAILABILITY_H
