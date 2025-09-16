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


#include "parse_data.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>


namespace Analyze
{
    const char progress[] = "frame\\s*=\\s*([\\d]+)\\s*fps\\s*=\\s*([\\d]+)\\s*q\\s*=\\s*(-?[\\d]+.[\\d]+)\\s+[L]?size\\s*=\\s*([N/A]*[\\d]*[KiB]*)\\s+time=\\s*([N/A]*[\\d:\\d:\\d.\\d]*)\\s+bitrate=\\s*([N/A]*[\\d.\\d]*[kbits/s]*)\\s+speed\\s*=\\s*([N/A]*[\\d.\\dx]*)";
}

ParseData::ParseData(QWidget *parent)
    : QWidget{parent}
{
    //empty constructor
}

ParseData::~ParseData()
{
    //empty deconstructor
}

void ParseData::parse_file_data(QString &data)
{
    //parse the received data
    QRegularExpression regx_parse(Analyze::progress);
    QRegularExpressionMatchIterator itr = regx_parse.globalMatch(data);

    while (itr.hasNext())
    {
        QRegularExpressionMatch match = itr.next();
        this->Status.frame_num = match.captured(1);
        Q_EMIT send_frame_num_update(this->Status.frame_num);//sent to progressbar
        this->Status.frame_fps = match.captured(2);
        this->Status.q_num     = match.captured(3);
        this->Status.size      = match.captured(4);
        this->Status.frame_time = match.captured(5);
        this->Status.proc_bitrate = match.captured(6);
        this->Status.proc_speed = match.captured(7);
        Q_EMIT send_parsed_data("frame=" + this->Status.frame_num + " " +
                                "fps=" + this->Status.frame_fps + " " +
                                "q=" + this->Status.q_num + " " +
                                "size=" + this->Status.size + " " +
                                "time=" + this->Status.frame_time + " " +
                                "bitrate=" + this->Status.proc_bitrate + " " +
                                "speed=" + this->Status.proc_speed);
    }
}
