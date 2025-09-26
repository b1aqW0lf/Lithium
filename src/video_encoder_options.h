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

private Q_SLOTS:
    void select_encoder_profile(const int &index);
    void select_encoder_level(const int &index);

private:
    Ui::VideoEncoderOptions *ui;
};

#endif // VIDEO_ENCODER_OPTIONS_H
