#include "video_encoder_options.h"
#include "ui_video_encoder_options.h"

VideoEncoderOptions::VideoEncoderOptions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoEncoderOptions)
{
    ui->setupUi(this);
}

VideoEncoderOptions::~VideoEncoderOptions()
{
    delete ui;
}
