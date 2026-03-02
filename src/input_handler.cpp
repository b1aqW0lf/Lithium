#include "input_handler.h"

InputHandler::InputHandler(QWidget *parent)
    : QWidget{parent}{/*constructor*/}

InputHandler::~InputHandler(){/*destructor*/}

void InputHandler::send_input_selections_request()
{
    //request the user input selections/parameters
    Q_EMIT request_input_selections();
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

void InputHandler::process_selected_inputs()
{
    input_selections.clear();

    input_selections << list.video_selection
                     << list.crf_qscale_selection
                     << list.preset_selection
                     << list.options_selections
                     << list.audio_selections
                     << list.metadata_command;

    Q_EMIT send_selected_input_parameters(input_selections);
}
