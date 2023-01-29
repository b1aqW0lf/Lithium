#ifndef AUDIO_UI_H
#define AUDIO_UI_H

/******************************************************************************
 Copyright (c) 2020-2023 b1aqW0lf
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


#include "ui_audio_ui.h"

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class AudioUI;
}
QT_END_NAMESPACE

class AudioUI : public QWidget
{
    Q_OBJECT

public:
    explicit AudioUI(QWidget *parent = nullptr);
    ~AudioUI();

private:
    Ui::AudioUI *ui;

private Q_SLOTS:
    void select_aud_bitrate();
    void select_aud_codec();
    void select_samplerate();
    void select_channels();

private:
    QString audio_br_value{};
    QString audio_codec{};
    QString audio_sr_value{};
    QString audio_ac_value{};
    //-----------------------------------
    //Interface String Lists
    QStringList audioCodecList{};
    QStringList audioBitrateList{};
    QStringList audioSampleList{};
    QStringList audioChannelList{};

};

#endif // AUDIO_UI_H
