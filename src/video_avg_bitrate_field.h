#ifndef VIDEO_AVG_BITRATE_FIELD_H
#define VIDEO_AVG_BITRATE_FIELD_H

#include <QWidget>

namespace Ui {
class VideoAVGBitrateField;
}

class VideoAVGBitrateField : public QWidget
{
    Q_OBJECT

public:
    explicit VideoAVGBitrateField(QWidget *parent = nullptr);
    ~VideoAVGBitrateField();

private:
    Ui::VideoAVGBitrateField *ui;
};

#endif // VIDEO_AVG_BITRATE_FIELD_H
