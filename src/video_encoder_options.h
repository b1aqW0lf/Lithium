#ifndef VIDEO_ENCODER_OPTIONS_H
#define VIDEO_ENCODER_OPTIONS_H

#include "encoder_level_data.h"
#include "encoder_profile_data.h"

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

Q_SIGNALS:
    void send_statusbar_message(const QString &message, const int &timeout);

public Q_SLOTS:
    void receive_selected_video_codec_name(const QString &video_codec);
    void receive_source_video_codec_profile(const QString &codec_profile);

private Q_SLOTS:
    void select_encoder_profile(const int &index);
    void select_encoder_level(const int &index);

private:
    Ui::VideoEncoderOptions *ui;
    VideoEncoderLevelData level_data;
    VideoEncoderProfileData profile_data;

    //variables
    QString video_codec{};
    QString codec_profile{};
};

#endif // VIDEO_ENCODER_OPTIONS_H
