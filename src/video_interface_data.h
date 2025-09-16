#ifndef VIDEO_INTERFACE_DATA_H
#define VIDEO_INTERFACE_DATA_H

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


class VideoInterfaceData
{
    friend class VideoInterface;
    //list of the video interface's stringlists - to be used with their respective comboboxes
    private:
        /*!
         * \brief videoCodecList is to be used with videoCodecBox
         */
        //list of available video codecs
        const QStringList videoCodecList =  QStringList() << "Source" << "H264/libx264" << "x264 10-bit" << "HEVC/libx265" << "x265 10-bit"
                                                         << "x265 12-bit" << "Xvid" << "VP9" << "Theora" << "MPEG-2" << "AV1/SVT-AV1";

        /*!
         * \brief videoResolutionList is to be used with videoResolutionBox
         */
        //list of video resolution values
        const QStringList videoResolutionList = QStringList() << "Source" << "640x360" << "720x480" << "720x576" << "1024x768" << "1280x720"
                                                              << "1366x768" << "1600x900" << "1920x1080" << "1920x1200" << "2048x1080"
                                                              << "2048x1152" << "2560x1440" << "3840x2160" << "4096x2160";

        /*!
         * \brief videoFramerateList is to be used with videoFramerateBox
         */
        //list of video framerate values
        const QStringList videoFramerateList = QStringList() << "Source" << "23.976" << "24" << "25" << "29.97" << "30" << "60" << "120" << "144";

        /*!
         * \brief videoAspectRatioList is to be used with videoAspectRatioBox
         */
        //list of video display aspect ratio values
        const QStringList videoAspectRatioList = QStringList() << "Source" << "4:3" << "16:9" << "16:10" << "21:9" << "21:10";

        /*!
         * \brief videoContainerList is to be used with videoContainerBox
         */
        //list of available video containers/extensions
        const QStringList videoContainerList = QStringList() << "Source" << "MP4" << "MKV" << "WebM" << "TS" << "OGV";
};

#endif // VIDEO_INTERFACE_DATA_H
