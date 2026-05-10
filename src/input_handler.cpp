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


#include "input_handler.h"


InputHandler::InputHandler(QWidget *parent)
    : QWidget{parent}{/*constructor*/}

InputHandler::~InputHandler(){/*destructor*/}

void InputHandler::send_input_selections_request()
{
    //request the user input selections/parameters
    Q_EMIT request_input_selections();
}

void InputHandler::receive_copy_subtitles_command(const QStringList &subtitles_command)
{
    list.subtitles_command = subtitles_command;
}

void InputHandler::receive_input_video_selection(const QStringList &video_selection)
{
    list.video_selection = video_selection;
}

void InputHandler::receive_input_crf_qscale_selection(const QStringList &crf_qscale_selection)
{
    list.crf_qscale_selection = crf_qscale_selection;
}

void InputHandler::receive_video_avg_bitrate_selections(const QStringList &bitrate_selections)
{
    list.bitrate_selections = bitrate_selections;
}

void InputHandler::receive_input_video_preset_selection(const QStringList &preset_selection)
{
    list.preset_selection = preset_selection;
}

void InputHandler::receive_video_options_selections(const QStringList &options_selections)
{
    list.options_selections = options_selections;
}

void InputHandler::receive_input_audio_selections(const QStringList &audio_selections)
{
    list.audio_selections = audio_selections;
}

void InputHandler::receive_copy_metadata_command(const QStringList &metadata_command)
{
    list.metadata_command = metadata_command;
}

void InputHandler::send_received_selected_inputs()
{
    process_selected_inputs();
}

void InputHandler::receive_current_process_mode(ProcessMode process_mode)
{
    this->process_mode = process_mode;
}

void InputHandler::process_selected_inputs()
{
    list.input_selections.clear();

    if(process_mode == ProcessMode::NormalMode || process_mode == ProcessMode::MergeMode)
    {
        list.input_selections << list.subtitles_command
                              << list.video_selection
                              << list.crf_qscale_selection
                              << list.preset_selection
                              << list.options_selections
                              << list.audio_selections
                              << list.metadata_command;
    }
    else if(process_mode == ProcessMode::ExtractMode)
    {
        list.input_selections << list.video_selection
                              << list.audio_selections
                              << list.metadata_command;
    }
    else
    {
        return;
    }

    Q_EMIT send_selected_input_parameters(list.input_selections);
}
