/******************************************************************************
 Copyright (c) 2020-2025 b1aqW0lf
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/


#include "video_interface.h"
#include "src/ui_video_interface.h"


VideoInterface::VideoInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoInterface)
{
    ui->setupUi(this);

    //connections - signals and slots
    connect(ui->copyVideoCheckBox, &QCheckBox::clicked,
            this, &VideoInterface::enable_copy_source_video);
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoInterface::select_video_codec);
    connect(ui->videoResolutionBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoInterface::select_video_resolution);
    connect(ui->videoFramerateBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoInterface::select_video_framerate);
    connect(ui->videoAspectRatioBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoInterface::select_video_display_aspect_ratio);
    connect(ui->videoContainerBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoInterface::select_video_container);

    this->initialize_video_interface_data();
}

VideoInterface::~VideoInterface()
{
    delete ui;
}

void VideoInterface::initialize_video_interface_data()
{
    ui->videoCodecBox->insertItems(0, videodata.videoCodecList);
    ui->videoCodecBox->insertSeparator(1);
    ui->videoResolutionBox->insertItems(0, videodata.videoResolutionList);
    ui->videoResolutionBox->insertSeparator(1);
    ui->videoFramerateBox->insertItems(0, videodata.videoFramerateList);
    ui->videoFramerateBox->insertSeparator(1);
    ui->videoAspectRatioBox->insertItems(0, videodata.videoAspectRatioList);
    ui->videoAspectRatioBox->insertSeparator(1);
    ui->videoContainerBox->insertItems(0, videodata.videoContainerList);
    ui->videoContainerBox->insertSeparator(1);
}

void VideoInterface::enable_copy_source_video()
{
    const int timeout{0};
    this->selection.copy_video_command.clear();

    if(ui->copyVideoCheckBox->isChecked() == true)
    {
        //send command to copy the source video stream
        this->selection.copy_video_command << command.video_codec_flag << command.copy_command;
        Q_EMIT this->send_video_statusbar_message("Copy Source Video Enabled", timeout);
    }
    else
    {
        //send regular command to transcode video stream
        this->selection.copy_video_command << command.video_codec_flag;
        Q_EMIT this->send_video_statusbar_message("", timeout);//clear the message
    }
}

void VideoInterface::receive_source_file_video_data(const QString &video_codec, const QString &video_resolution,
                                                    const QString &video_framerate, const QString &video_aspect_ratio)
{
    //receive source file audio data
    this->process_source_file_video_data(video_codec, video_resolution, video_framerate, video_aspect_ratio);
}

void VideoInterface::process_source_file_video_data(const QString &video_codec, const QString &video_resolution,
                                                    const QString &video_framerate, const QString &video_aspect_ratio)
{
    //used for the UserRole of the "Source" DisplayRole
    const int index{0};//first index for the comboboxes
    this->ui->videoCodecBox->setItemData(index, video_codec, Qt::UserRole);
    this->ui->videoResolutionBox->setItemData(index, video_resolution, Qt::UserRole);
    this->ui->videoFramerateBox->setItemData(index, video_framerate, Qt::UserRole);
    this->ui->videoAspectRatioBox->setItemData(index, video_aspect_ratio, Qt::UserRole);
}

void VideoInterface::select_video_codec(const int &index)
{
    const int message_timeout{0};
    this->selection.video_codec_selection.clear();

    if(index == 0)//source
    {
        //clicking "Source" will set the source file codec as the selected codec
        this->selection.video_codec_selection << ui->videoCodecBox->itemData(index, Qt::UserRole).toString();
    }
    else if(index == 1)//separator
    {
        //option one (1) cannot be selected by the user - it is the separator
        return;
    }
    else if(index == 2)//h264/libx264
    {
        ui->videoCodecBox->setItemData(index, "libx264", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString()
                                              << command.pixel_format_flag << command.pixel_yuv420p;
    }
    else if(index == 3)//h264 10bit /libx264 10bit
    {
        ui->videoCodecBox->setItemData(index, "libx264", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString()
                                              << command.pixel_format_flag << command.pixel_yuv420p10bit;
    }
    else if(index == 4)//hevc/libx265
    {
        ui->videoCodecBox->setItemData(index, "libx265", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString()
                                              << command.pixel_format_flag << command.pixel_yuv420p;
    }
    else if(index == 5)//hevc 10bit/libx265 10bit
    {
        ui->videoCodecBox->setItemData(index, "libx265", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString()
                                              << command.pixel_format_flag << command.pixel_yuv444p10bit;
    }
    else if(index == 6)//hevc 12bit/libx265 12bit
    {
        ui->videoCodecBox->setItemData(index, "libx265", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString()
                                              << command.pixel_format_flag << command.pixel_yuv444p12bit;
    }
    else if(index == 7)//xvid/libxvid
    {
        ui->videoCodecBox->setItemData(index, "libxvid", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString();
    }
    else if(index == 8)//vp9/libvpx-vp9
    {
        ui->videoCodecBox->setItemData(index, "libvpx-vp9", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString();
    }
    else if(index == 9)//theora/libtheora
    {
        ui->videoCodecBox->setItemData(index, "libtheora", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString();
    }
    else if(index == 10)//mpeg2/mpeg2video
    {
        ui->videoCodecBox->setItemData(index, "mpeg2video", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString();
    }
    else if(index == 11)//av1/libsvt-av1
    {
        ui->videoCodecBox->setItemData(index, "libsvt-av1", Qt::UserRole);
        this->selection.video_codec_selection <<  ui->videoCodecBox->itemData(index, Qt::UserRole).toString();
    }
    else
    {
        Q_EMIT this->send_video_statusbar_message("Something went wrong", message_timeout);
    }

    Q_EMIT this->send_video_statusbar_message(ui->videoCodecBox->itemData(index, Qt::UserRole).toString(), message_timeout);
}

void VideoInterface::select_video_resolution(const int &index)
{
    const int message_timeout{0};
    this->selection.video_resolution_selection.clear();

    if(index == 0)//source
    {
        //clicking "Source" will set the source file resolution as the selected resolution
        this->selection.video_resolution_selection << command.video_filter_flag
                                                   << command.video_scale+ui->videoResolutionBox->itemData(index, Qt::UserRole).toString();
        Q_EMIT this->send_video_statusbar_message(ui->videoResolutionBox->itemData(index, Qt::UserRole).toString(), message_timeout);
    }
    else if(index == 1)//separator
    {
        //option one (1) cannot be selected by the user - it is the separator
        return;
    }
    else if(index >= 2 && index <= videodata.videoResolutionList.size())
    {
        this->selection.video_resolution_selection << command.video_filter_flag
                                                   << command.video_scale+ui->videoResolutionBox->currentText();
        Q_EMIT this->send_video_statusbar_message(ui->videoResolutionBox->currentText(), message_timeout);
    }
    else
    {
        return;
    }
}

void VideoInterface::select_video_framerate(const int &index)
{
    const int message_timeout{0};
    this->selection.video_framerate_selection.clear();

    if(index == 0)
    {
        //clicking "Source" will set the source framerate as the selected framerate
        this->selection.video_framerate_selection << command.video_fps_flag
                                                  << ui->videoFramerateBox->itemData(index, Qt::UserRole).toString();
        Q_EMIT this->send_video_statusbar_message(ui->videoFramerateBox->itemData(index, Qt::UserRole).toString(), message_timeout);
    }
    else if(index == 1)//separator
    {
        //option one (1) cannot be selected by the user - it is the separator
        return;
    }
    else if(index >= 2 && index <= videodata.videoFramerateList.size())
    {
        this->selection.video_framerate_selection << command.video_fps_flag << ui->videoFramerateBox->currentText();
        Q_EMIT this->send_video_statusbar_message(ui->videoFramerateBox->currentText(), message_timeout);
    }
    else
    {
        return;
    }
}

void VideoInterface::select_video_display_aspect_ratio(const int &index)
{
    const int message_timeout{0};
    this->selection.video_display_aspect_ratio_selection.clear();

    if(index == 0)
    {
        if(ui->videoAspectRatioBox->itemText(index).contains("setdar=", Qt::CaseInsensitive))
        {
            //keep the "setdar=" value - set it as the display aspect ratio command
            this->selection.video_display_aspect_ratio_selection << ui->videoAspectRatioBox->itemData(index, Qt::UserRole).toString();
            Q_EMIT this->send_video_statusbar_message(ui->videoAspectRatioBox->itemData(index, Qt::UserRole).toString(), message_timeout);
        }
        else
        {
            //clicking "Source" will set the source display aspect ratio as the selected display aspect ratio
            this->selection.video_display_aspect_ratio_selection << command.video_dar_flag
                                                                 << ui->videoAspectRatioBox->itemData(index, Qt::UserRole).toString();
            Q_EMIT this->send_video_statusbar_message(ui->videoAspectRatioBox->itemData(index, Qt::UserRole).toString(), message_timeout);
        }
    }
    else if(index == 1)//separator
    {
        //option one (1) cannot be selected by the user - it is the separator
        return;
    }
    else if(index >= 2 && index <= videodata.videoAspectRatioList.size())
    {
        this->selection.video_display_aspect_ratio_selection << command.video_dar_flag
                                                             << ui->videoAspectRatioBox->currentText();
        Q_EMIT this->send_video_statusbar_message(ui->videoAspectRatioBox->currentText(), message_timeout);
    }
    else
    {
        return;
    }
}

void VideoInterface::select_video_container(const int &index)
{
    const int message_timeout{0};
    this->selection.video_container_selection.clear();

    if(index == 0)
    {
        //clicking "Source" will set the source container as the selected container
        this->selection.video_container_selection = "."+ui->videoContainerBox->itemData(index, Qt::UserRole).toString();
        Q_EMIT this->send_video_statusbar_message(ui->videoContainerBox->itemData(index, Qt::UserRole).toString().toUpper(), message_timeout);
    }
    else if(index == 1)//separator
    {
        //option one (1) cannot be selected by the user - it is the separator
        return;
    }
    else if(index >= 2 && index <= videodata.videoContainerList.size())
    {
        this->selection.video_container_selection = "."+ui->videoContainerBox->currentText().toLower();
        Q_EMIT this->send_video_statusbar_message(ui->videoContainerBox->currentText(), message_timeout);
    }
    else
    {
        return;
    }

    Q_EMIT this->send_selected_video_extension(this->selection.video_container_selection);
}

void VideoInterface::receive_source_video_file_extension(const QString &extension)
{
    //receive the source video file's extension
    this->process_source_video_file_extension(extension);
}

void VideoInterface::process_source_video_file_extension(const QString &extension)
{
    const int index{0};
    //set the "Source" option to the source video file's extension
    this->ui->videoContainerBox->setItemData(index, extension, Qt::UserRole);
}

void VideoInterface::get_video_interface_selections()
{
    this->process_video_interface_selections();
}

void VideoInterface::process_video_interface_selections()
{
    this->selection.video_selection_list.clear();

    if(ui->copyVideoCheckBox->isChecked() == true)
    {
        //send the copy video stream command
        this->selection.video_selection_list << this->selection.copy_video_command;
    }
    if(ui->copyVideoCheckBox->isChecked() == false)
    {
        const int index0{0};
        if(ui->videoAspectRatioBox->itemText(index0).contains("setdar=", Qt::CaseInsensitive))
        {
            //check if the display aspect ratio command has the setdar= command and add it to the
            //resolution command-> -filter:v scale=(resolution),setdar=(display_aspect_ratio) if it does -
            //adding the selections to the video selection list
            this->selection.video_selection_list << this->selection.copy_video_command
                                                 << this->selection.video_codec_selection
                                                 << this->selection.video_resolution_selection[1]+","+
                                                        this->selection.video_display_aspect_ratio_selection[0]//add setdar= to -filter:v
                                                 << this->selection.video_framerate_selection;
        }
        else
        {
            //send the transcode video stream command along with the video options
            //adding the selections to the video selection list
            this->selection.video_selection_list << this->selection.copy_video_command
                                                 << this->selection.video_codec_selection
                                                 << this->selection.video_resolution_selection
                                                 << this->selection.video_display_aspect_ratio_selection
                                                 << this->selection.video_framerate_selection;
        }
    }

    //send the video selections
    Q_EMIT this->send_video_interface_selections(this->selection.video_selection_list);
}
