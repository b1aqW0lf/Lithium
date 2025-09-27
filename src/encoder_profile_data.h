#ifndef ENCODER_PROFILE_DATA_H
#define ENCODER_PROFILE_DATA_H

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


class VideoEncoderProfileData
{
    friend class VideoEncoderOptions;
    private:
        const QStringList h264_profiles = QStringList() << "Source" << "auto" << "baseline" << "main"
                                                        << "high" << "high10" << "high422" << "high444";

        const QStringList x26410bit_profiles = QStringList() << "Source" << "auto" << "high10" << "high422" << "high444";

        const QStringList hevc_profiles = QStringList() << "Source" << "auto" << "main" << "main-intra"
                                                        << "mainstillpicture" << "main444-8" << "main444-intra"
                                                        << "main444-stillpicture";

        const QStringList x26510bit_profiles = QStringList() << "Source" << "auto" << "main10" << "main10-intra"
                                                             << "main422-10" << "main422-10-intra" << "main444-10"
                                                             << "main444-10-intra";

        const QStringList x26512bit_profiles = QStringList() << "Source" << "auto" << "main12" << "main12-intra"
                                                             << "main422-12" << "main422-12-intra" << "main444-12"
                                                             << "main444-12-intra";

        const QStringList vp9_profiles = QStringList() << "Source" << "auto" << "0" << "1" << "2" << "3";

        const QStringList xvid_profiles = QStringList() << "Source" << "auto" << "1" << "2" << "3" << "4"
                                                        << "5" << "6" <<"7" << "8" << "9";

        const QStringList mpeg2_profiles = QStringList() << "Source" << "auto" << "main" << "high";

        const QStringList av1_profiles = QStringList() << "Source" << "auto" << "main";
};

#endif // ENCODER_PROFILE_DATA_H
