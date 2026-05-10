#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

/******************************************************************************
 Copyright (c) 2020-2026 b1aqW0lf
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


#include "process_mode_flags.h"

#include <QWidget>


class InputHandler : public QWidget
{
    Q_OBJECT

public:
    explicit InputHandler(QWidget *parent = nullptr);
    ~InputHandler();

Q_SIGNALS:
    void send_selected_input_parameters(const QStringList &input_selections);
    void request_input_selections();

public Q_SLOTS:
    void send_input_selections_request();
    void receive_copy_subtitles_command(const QStringList &subtitles_command);
    void receive_input_video_selection(const QStringList &video_selection);
    void receive_input_crf_qscale_selection(const QStringList &crf_qscale_selection);
    void receive_video_avg_bitrate_selections(const QStringList &bitrate_selections);
    void receive_input_video_preset_selection(const QStringList &preset_selection);
    void receive_video_options_selections(const QStringList &options_selections);
    void receive_input_audio_selections(const QStringList &audio_selections);
    void receive_copy_metadata_command(const QStringList &metadata_command);
    void send_received_selected_inputs();
    void receive_current_process_mode(ProcessMode process_mode);

private:
    //functions
    void process_selected_inputs();

    //variables
    ProcessMode process_mode{ProcessMode::NormalMode};

    //struct
    struct SelectionList{
        QStringList subtitles_command{};
        QStringList video_selection{};
        QStringList crf_qscale_selection{};
        QStringList bitrate_selections{};
        QStringList preset_selection{};
        QStringList options_selections{};
        QStringList audio_selections{};
        QStringList metadata_command{};
        QStringList input_selections{};
    }list;
};

#endif // INPUT_HANDLER_H
