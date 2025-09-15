#ifndef AUDIO_INTERFACE_H
#define AUDIO_INTERFACE_H

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


#include "audio_commands.h"
#include "audio_interface_data.h"

#include <QWidget>


namespace Ui {
class AudioInterface;
}

class AudioInterface : public QWidget
{
    Q_OBJECT

public:
    explicit AudioInterface(QWidget *parent = nullptr);
    ~AudioInterface();

Q_SIGNALS:
    void send_audio_statusbar_message(const QString &message, const int &timeout);
    void send_selected_audio_extension(const QString &extension);
    void send_audio_interface_selections(const QStringList &audio_selections);

public Q_SLOTS:
    void receive_source_file_audio_data(const QString &audio_codec, const QString &audio_bitrate,
                                        const QString &audio_samplerate, const QString &audio_channels);
    void get_audio_interface_selections();

private:
    Ui::AudioInterface *ui;
    AudioInterfaceData audiodata;
    AudioCommands command;

private Q_SLOTS:
    void enable_copy_source_audio();
    void select_audio_codec(const int &index);
    void select_audio_bitrate(const int &index);
    void select_audio_samplerate(const int &index);
    void select_audio_channels(const int &index);
    void select_audio_container(const int &index);

private:
    //functions
    void initialize_audio_interface_data();
    void process_source_file_audio_data(const QString &audio_codec, const QString &audio_bitrate,
                                        const QString &audio_samplerate, const QString &audio_channels);
    void process_audio_interface_selections();
    void setup_audio_mono_stereo_channel(const int &index, const int &message_timeout, const QString &audio_channel, Qt::ItemDataRole role);

    //struct
    struct
    {
        QStringList copy_audio_command{};
        QStringList audio_codec_selection{};
        QStringList audio_bitrate_selection{};
        QStringList audio_samplerate_selection{};
        QStringList audio_channel_selection{};
        QString audio_container_selection{};
        //-----------------------------------------//
        QStringList audio_selection_list{};
    }selection;
};

#endif // AUDIO_INTERFACE_H
