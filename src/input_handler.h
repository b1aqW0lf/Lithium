#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

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
    void receive_input_video_selection(const QStringList &video_selection);
    void receive_input_crf_qscale_selection(const QStringList &crf_qscale_selection);
    void receive_input_video_preset_selection(const QStringList &preset_selection);
    void receive_video_options_selections(const QStringList &options_selections);
    void send_received_selected_inputs();

private:
    //functions
    void process_selected_inputs();

    //struct
    struct SelectionList{
        QStringList crf_qscale_selection{};
        QStringList video_selection{};
        QStringList preset_selection{};
        QStringList options_selections{};
    }list;

    QStringList input_selections{};
};

#endif // INPUT_HANDLER_H
