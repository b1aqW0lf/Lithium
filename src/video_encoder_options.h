#ifndef VIDEO_ENCODER_OPTIONS_H
#define VIDEO_ENCODER_OPTIONS_H

#include <QWidget>

namespace Ui {
class VideoEncoderOptions;
}

class VideoEncoderOptions : public QWidget
{
    Q_OBJECT

public:
    explicit VideoEncoderOptions(QWidget *parent = nullptr);
    ~VideoEncoderOptions();

private:
    Ui::VideoEncoderOptions *ui;
};

#endif // VIDEO_ENCODER_OPTIONS_H
