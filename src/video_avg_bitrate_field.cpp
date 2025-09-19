#include "video_avg_bitrate_field.h"
#include "src/ui_video_avg_bitrate_field.h"

VideoAVGBitrateField::VideoAVGBitrateField(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoAVGBitrateField)
{
    ui->setupUi(this);
}

VideoAVGBitrateField::~VideoAVGBitrateField()
{
    delete ui;
}
