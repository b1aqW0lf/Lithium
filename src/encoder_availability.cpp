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


#include "encoder_availability.h"

#ifndef TIMEOUT
#define TIMEOUT 0
#endif


EncoderAvailability::EncoderAvailability(QObject *parent)
    : QObject{parent}{/*constructor*/}

EncoderAvailability::~EncoderAvailability(){/*destructor*/}

void EncoderAvailability::send_encoder_ready_status()
{
    this->get_encoder_availability();
}

void EncoderAvailability::get_encoder_availability()
{
    //check if encoder paths are empty
    this->check_encoder_path();

    //send signal in regards to availability
    this->send_availability_signal();
}

void EncoderAvailability::check_encoder_path()
{
    if(enc_process.get_encoder_path("ffmpeg").isEmpty() == false)
    {
        this->encoder.ffmpeg_found = true;
    }
    if(enc_process.get_encoder_path("ffmpeg").isEmpty() == true)
    {
        this->encoder.ffmpeg_found = false;
    }
    if(enc_process.get_encoder_path("ffprobe").isEmpty() == false)
    {
        this->encoder.ffprobe_found = true;
    }
    if(enc_process.get_encoder_path("ffprobe").isEmpty() == true)
    {
        this->encoder.ffprobe_found = false;
    }
}

void EncoderAvailability::send_availability_signal()
{
    if(this->encoder.ffmpeg_found == true && this->encoder.ffprobe_found == true)
    {
        Q_EMIT this->send_encoder_availability_message("Ready", TIMEOUT);
    }
    else if(this->encoder.ffmpeg_found == false && this->encoder.ffprobe_found == true)
    {
        Q_EMIT this->send_encoder_availability_message("Not Ready - ffmpeg not found", TIMEOUT);
    }
    else if(this->encoder.ffmpeg_found == true && this->encoder.ffprobe_found == false)
    {
        Q_EMIT this->send_encoder_availability_message("Not Ready - ffprobe not found", TIMEOUT);
    }
    else if(this->encoder.ffmpeg_found == false && this->encoder.ffprobe_found == false)
    {
        Q_EMIT this->send_encoder_availability_message("Not Ready - ffmpeg and ffprobe not found", TIMEOUT);
    }
    else
    {
        return;
    }
}

void EncoderAvailability::set_encoder_availablity()
{
    this->make_encoder_available();
}

void EncoderAvailability::make_encoder_available()
{
    enc_process.set_encoder_path("ffmpeg");
    enc_process.set_encoder_path("ffprobe");
}
