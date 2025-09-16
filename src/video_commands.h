#ifndef VIDEO_COMMANDS_H
#define VIDEO_COMMANDS_H

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


#include <QStringList>


class VideoCommands
{
    friend class VideoInterface;

    //list of ffmpeg video transcode commands
private:
    const QString copy_command = "copy";
    const QString video_codec_flag = "-codec:v";
    const QString pixel_format_flag = "-pix_fmt";
    const QString pixel_yuv420p = "yuv420p";//8bit
    const QString pixel_yuv420p10bit = "yuv420p10le";
    const QString pixel_yuv444p10bit = "yuv444p10le";
    const QString pixel_yuv444p12bit = "yuv444p12le";
    const QString video_filter_flag = "-filter:v";
    const QString video_scale       = "scale=";
    const QString video_fps_flag = "-r";
    const QString video_dar_flag = "-aspect";//display aspect ratio flag
};

#endif // VIDEO_COMMANDS_H
