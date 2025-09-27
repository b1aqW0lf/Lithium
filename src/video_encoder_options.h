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
    void send_video_options_selections(const QStringList &options_selections);

public Q_SLOTS:
    void receive_selected_video_codec_name(const QString &video_codec);
    void receive_source_video_codec_profile(const QString &codec_profile);
    void get_video_encoder_options_selections();

private Q_SLOTS:
    void select_encoder_profile(const QString &codec_profile);
    void select_encoder_level(const QString &codec_level);

private:
    Ui::VideoEncoderOptions *ui;
    VideoEncoderLevelData level_data;
    VideoEncoderProfileData profile_data;

    //functions
    void initialize_video_encoder_options(const QString &video_codec);
    void initialize_encoder_profiles(const QString &video_codec);
    void initialize_encoder_levels(const QString &video_codec);
    void process_video_options_selections();

    //variables
    QString codec_profile{};

    struct
    {
        QString encoder_profile_command{"-profile:v"};
        QString encoder_level_command{"-level:v"};
        QStringList encoder_profile_selection{};
        QStringList encoder_level_selection{};
        QStringList video_options_selections{};
    }selection;
};

#endif // VIDEO_ENCODER_OPTIONS_H
