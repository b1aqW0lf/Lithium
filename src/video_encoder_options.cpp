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

void VideoEncoderOptions::select_encoder_profile(const int &index)
{

}

void VideoEncoderOptions::select_encoder_level(const int &index)
{

}

void VideoEncoderOptions::receive_selected_video_codec_name(const QString &video_codec)
{
    this->video_codec = video_codec;
}
