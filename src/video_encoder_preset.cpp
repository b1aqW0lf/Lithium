#include "video_encoder_preset.h"
#include "ui_video_encoder_preset.h"

VideoEncoderPreset::VideoEncoderPreset(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoEncoderPreset)
{
    ui->setupUi(this);
}

VideoEncoderPreset::~VideoEncoderPreset()
{
    delete ui;
}
