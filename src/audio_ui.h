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


#include <QWidget>

#include "ui_audio_ui.h"

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

Q_SIGNALS:
    void send_output_audio_extension(const QString &text);

    //send current audio options
    void send_current_audio_options(const QString &codec, const QString &channels,
                                    const QString &samplerate, const QString &bitrate);

    //for testing only!
    void send_audio_data(const QString &data, const int &timeout);
    //-----------------------------------------------------------------//

public Q_SLOTS:
    void receive_audio_source_extension(const QString &extension);
    void receive_audio_source_codec(const QString &codec);
    void receive_audio_source_bitrate(const QString &bitrate);
    void receive_audio_source_samplerate(const QString &samplerate);
    void receive_audio_source_channels(const QString &channels);
    void receive_process_mode_state(const bool &normal, const bool &merge, const bool &extract);

    //experimental
    void get_selected_audio_options();

private:
    Ui::AudioUI *ui;

private Q_SLOTS:
    void select_aud_codec(const int index);
    void select_aud_bitrate();
    void select_samplerate();
    void select_channels();
    void select_aud_container();

private:
    //audio variables
    QString audio_source_ext{};
    QString audio_container{};
    //-----------------------------------
    QString audio_codec{};
    QString audio_bitrate{};
    QString audio_samplerate{};
    QString audio_channels{};
    //-----------------------------------
    QString source_codec{};
    QString source_bitrate{};
    QString source_samplerate{};
    QString source_channels{};
    //------------------------------------
    //process mode state
    bool normal_mode{};
    bool merge_mode{};
    bool extract_mode{};
    //------------------------------------
    //functions
    void enable_extract_audio_settings(const bool &extract);
    void default_options_check();
    //------------------------------------
    //Interface String Lists
    QStringList audioCodecList{};
    QStringList audioBitrateList{};
    QStringList audioSampleList{};
    QStringList audioChannelList{};
    QStringList audioContainerList{};

};

#endif // AUDIO_UI_H
