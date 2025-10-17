#include "input_handler.h"

InputHandler::InputHandler(QWidget *parent)
    : QWidget{parent}{/*constructor*/}

InputHandler::~InputHandler(){/*destructor*/}

void InputHandler::send_input_selections_request()
{
    //request the user input selections/parameters
    this->request_input_selections();
}

void InputHandler::receive_input_video_selection(const QStringList &video_selection)
{
    this->list.video_selection = video_selection;
}

void InputHandler::receive_input_crf_qscale_selection(const QStringList &crf_qscale_selection)
{
    this->list.crf_qscale_selection = crf_qscale_selection;
}

void InputHandler::send_received_selected_inputs()
{
    this->process_selected_inputs();
}

void InputHandler::process_selected_inputs()
{
    this->input_selections.clear();

    this->input_selections << this->list.video_selection
                           << this->list.crf_qscale_selection;

    Q_EMIT this->send_selected_input_parameters(this->input_selections);
}
