#include "video_encoder_options.h"
#include "ui_video_encoder_options.h"

VideoEncoderOptions::VideoEncoderOptions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoEncoderOptions)
{
    ui->setupUi(this);

    connect(ui->videoEncoderProfileBox, QOverload<int>::of(&QComboBox::activated),
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

void VideoEncoderOptions::initialize_video_encoder_options(const QString &video_codec)
{
    this->initialize_encoder_profiles(video_codec);
}

void VideoEncoderOptions::initialize_encoder_profiles(const QString &video_codec)
{
    ui->videoEncoderProfileBox->clear();
    const int index0{0};

    if(video_codec.contains("H264", Qt::CaseInsensitive) ||
        video_codec.contains("H.264", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.h264_profiles);
    }
    else if(video_codec.contains("HEVC", Qt::CaseInsensitive) ||
               video_codec.contains("H265", Qt::CaseInsensitive) ||
               video_codec.contains("H.265", Qt::CaseInsensitive))
    {
        ui->videoEncoderProfileBox->insertItems(index0, profile_data.hevc_profiles);
    }
}

void VideoEncoderOptions::receive_source_video_codec_profile(const QString &codec_profile)
{
    this->codec_profile = codec_profile;
}

void VideoEncoderOptions::select_encoder_profile(const int &index)
{

}

void VideoEncoderOptions::select_encoder_level(const int &index)
{

}
