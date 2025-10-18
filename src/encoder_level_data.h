#ifndef ENCODER_LEVEL_DATA_H
#define ENCODER_LEVEL_DATA_H

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


class VideoEncoderLevelData
{
    friend class VideoEncoderOptions;
    private:
        const QStringList h264_levels = QStringList() << "Source" << "auto" << "1" << "1b" << "1.1" << "1.2" << "1.3" << "2"
                                                      << "2.1" << "2.2" << "3" << "3.1" << "3.2" << "4" << "4.1"
                                                      << "4.2" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";

        const QStringList x26410bit_levels = QStringList() << "Source" << "auto" << "1" << "1b" << "1.1" << "1.2" << "1.3" << "2"
                                                           << "2.1" << "2.2" << "3" << "3.1" << "3.2" << "4" << "4.1"
                                                           << "4.2" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";

        const QStringList hevc_levels = QStringList() << "Source" << "auto" << "1" << "2" << "2.1" << "3" << "3.1" << "4"
                                                      << "4.1" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";

        const QStringList x26510bit_levels = QStringList() << "Source" << "auto" << "1" << "2" << "2.1" << "3" << "3.1" << "4"
                                                           << "4.1" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";

        const QStringList x26512bit_levels = QStringList() << "Source" << "auto" << "1" << "2" << "2.1" << "3" << "3.1" << "4"
                                                           << "4.1" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";

        const QStringList vp9_levels = QStringList() << "Source" << "auto" << "1" << "1.1" << "2" << "2.1" << "3" << "3.1"
                                                     << "4" << "4.1" << "5" << "5.1" << "5.2" << "6" << "6.1" << "6.2";

        const QStringList xvid_levels = QStringList() << "Source" << "auto" << "0" << "1" << "2" << "3" << "4";

        const QStringList mpeg2_levels = QStringList() << "Source" << "auto" << "low" << "main" << "high1440" << "high";

        const QStringList av1_levels = QStringList() << "Source" << "auto" << "2.0" << "2.1" << "3.0" << "3.1" << "4.0" << "4.1"
                                                     << "5.0" << "5.1" << "5.2" << " 5.3" << "6.0" << " 6.1" << "6.2" << "6.3";
};

#endif // ENCODER_LEVEL_DATA_H
