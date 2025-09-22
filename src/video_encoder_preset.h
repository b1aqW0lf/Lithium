#ifndef VIDEO_ENCODER_PRESET_H
#define VIDEO_ENCODER_PRESET_H

#include <QWidget>

namespace Ui {
class VideoEncoderPreset;
}

class VideoEncoderPreset : public QWidget
{
    Q_OBJECT

public:
    explicit VideoEncoderPreset(QWidget *parent = nullptr);
    ~VideoEncoderPreset();

private:
    Ui::VideoEncoderPreset *ui;
};

#endif // VIDEO_ENCODER_PRESET_H
