#include "video_encoder_options.h"
#include "ui_video_encoder_options.h"

VideoEncoderOptions::VideoEncoderOptions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoEncoderOptions)
{
    ui->setupUi(this);

    connect(ui->videoEncoderProfileBox, &QComboBox::textActivated,
            this, &VideoEncoderOptions::select_encoder_profile);
    connect(ui->videoEncoderLevelBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoEncoderOptions::select_encoder_level);
}

VideoEncoderOptions::~VideoEncoderOptions()
{
    delete ui;
}

void VideoEncoderOptions::receive_selected_video_codec_name(const QString &video_codec)
{
    this->video_codec = video_codec;
    this->initialize_video_encoder_options(video_codec);
}

void VideoEncoderOptions::receive_source_video_codec_profile(const QString &codec_profile)
{
    this->codec_profile = codec_profile;
}

void VideoEncoderOptions::initialize_video_encoder_options(const QString &video_codec)
{
    this->initialize_encoder_profiles(video_codec);
}

void VideoEncoderOptions::initialize_encoder_profiles(const QString &video_codec)
{
    //initialize videoEncoderProfileBox with the profiles of the selected codec
    ui->videoEncoderProfileBox->clear();
    const int index0{0};
    const int separator{1};

    if(video_codec.contains("H264", Qt::CaseInsensitive) ||
        video_codec.contains("H.264", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.h264_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else if(video_codec.contains("x264 10-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.x26410bit_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else if(video_codec.contains("HEVC", Qt::CaseInsensitive) ||
               video_codec.contains("H265", Qt::CaseInsensitive) ||
               video_codec.contains("H.265", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.hevc_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else if(video_codec.contains("x265 10-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.x26510bit_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else if(video_codec.contains("x265 12-bit", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.x26512bit_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else if(video_codec.contains("VP9", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.vp9_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else if(video_codec.contains("Xvid", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.xvid_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else if(video_codec.contains("Theora", Qt::CaseInsensitive))
    {
        //theora does not have have video codec profiles
        return;
    }
    else if(video_codec.contains("MPEG-2", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.mpeg2_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else if(video_codec.contains("AV1", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.av1_profiles);
        ui->videoEncoderProfileBox->insertSeparator(separator);
    }
    else
    {
        return;
    }
}

void VideoEncoderOptions::select_encoder_profile(const QString &codec_profile)
{
    const int timeout{0};
    const int index0{0};

    if(codec_profile.contains("Source"))
    {
        //Set the User Role item data to the value of source file's profile
        ui->videoEncoderProfileBox->setItemData(index0, this->codec_profile, Qt::UserRole);
        this->selection.encoder_profile_selection << selection.encoder_profile_command
                                                  << ui->videoEncoderProfileBox->itemData(index0, Qt::UserRole).toString().toLower();
        Q_EMIT this->send_statusbar_message(ui->videoEncoderProfileBox->itemData(index0, Qt::UserRole).toString(), timeout);
    }
    else
    {
        this->selection.encoder_profile_selection << selection.encoder_profile_command
                                                  << codec_profile;
        Q_EMIT this->send_statusbar_message(codec_profile, timeout);
    }

    //set tooltip
    ui->videoEncoderProfileBox->setToolTip(ui->videoEncoderProfileBox->currentText());
}

void VideoEncoderOptions::select_encoder_level(const int &index)
{

}
