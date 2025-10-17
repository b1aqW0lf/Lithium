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
    connect(ui->videoAspectRatioBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoInterface::select_video_display_aspect_ratio);
    connect(ui->videoFramerateBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoInterface::select_video_framerate);
    connect(ui->videoColorspaceBox, QOverload<int>::of(&QComboBox::activated),
            this, &VideoInterface::select_video_colorspace);

    this->initialize_video_interface_data();
    this->initialize_video_ui_default_settings();
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
    ui->videoAspectRatioBox->insertItems(0, videodata.videoAspectRatioList);
    ui->videoAspectRatioBox->insertSeparator(1);
    ui->videoFramerateBox->insertItems(0, videodata.videoFramerateList);
    ui->videoFramerateBox->insertSeparator(1);
    ui->videoColorspaceBox->insertItems(0, videodata.videoColorspaceList);
    ui->videoColorspaceBox->insertSeparator(1);
    ui->videoColorspaceBox->insertSeparator(3);

}
void VideoInterface::initialize_video_ui_default_settings()
{
    ui->copyVideoCheckBox->setChecked(false);
    ui->videoCodecBox->setCurrentIndex(0);
    ui->videoResolutionBox->setCurrentIndex(0);
    ui->videoAspectRatioBox->setCurrentIndex(0);
    ui->videoFramerateBox->setCurrentIndex(0);
    ui->videoColorspaceBox->setCurrentIndex(2);
}

void VideoInterface::enable_copy_source_video()
{
    const int timeout{0};

    if(ui->copyVideoCheckBox->isChecked() == true)
    {
        //send command to copy the source video stream
        this->selection.copy_video_enabled = true;
        Q_EMIT this->send_video_statusbar_message("Copy Source Video Enabled", timeout);
    }
    else
    {
        //send regular command to transcode video stream
        this->selection.copy_video_enabled = false;
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
    //first index for the comboboxes
    const int index{0};

    //placing data in their respective variables for when the user does not select an option - defaults to "Source"
    this->selection.video_codec_selection.clear();
    this->selection.video_resolution_selection.clear();
    this->selection.video_display_aspect_ratio_selection.clear();
    this->selection.video_framerate_selection.clear();

    this->selection.video_codec_selection << video_codec;
    this->selection.video_resolution_selection = video_resolution;
    this->selection.video_display_aspect_ratio_selection = video_aspect_ratio;
    this->selection.video_framerate_selection << video_framerate;

    //used for the UserRole of the "Source" DisplayRole -> when the user manually selects "Source"
    this->ui->videoCodecBox->setItemData(index, video_codec, Qt::UserRole);
    Q_EMIT this->send_selected_video_codec_name(video_codec);//send the codec name to set the initial ui settings
    this->ui->videoResolutionBox->setItemData(index, video_resolution, Qt::UserRole);
    this->ui->videoAspectRatioBox->setItemData(index, video_aspect_ratio, Qt::UserRole);
    this->ui->videoFramerateBox->setItemData(index, video_framerate, Qt::UserRole);
}

void VideoInterface::receive_source_video_colorspace_data(const QString &stream_colorspace, const QString &color_space,
                                                          const QString &color_transfer, const QString &color_primaries)
{
    this->process_source_video_colorspace_data(stream_colorspace, color_space, color_transfer, color_primaries);
}

void VideoInterface::process_source_video_colorspace_data(const QString &stream_colorspace, const QString &color_space,
                                                          const QString &color_transfer, const QString &color_primaries)
{
    const int index0{0};
    this->selection.video_colorspace_selection.clear();
    if(stream_colorspace.isEmpty() || stream_colorspace.length() == 0)
    {
        //set itemdata to Unknown to signify the stream colorspace is unknown
        //unknown is an actual value in ffmpeg colorspace -->https://trac.ffmpeg.org/wiki/colorspace
        ui->videoColorspaceBox->setItemData(index0, "Unknown", Qt::UserRole);
    }
    else
    {
        ui->videoColorspaceBox->setItemData(index0, stream_colorspace, Qt::UserRole);
    }

    this->selection.video_color_space = color_space;
    this->selection.video_color_transfer = color_transfer;
    this->selection.video_color_primaries = color_primaries;

    this->selection.video_colorspace_selection << command.colorspace_flag << color_space
                                               << command.color_transfer_flag << color_transfer
                                               << command.color_primaries_flag << color_primaries;/**/
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

    //send the name of the selected codec
    if(index == 0)
    {
        //send codec assigned to "Source" via the Qt::UserRole
        Q_EMIT this->send_selected_video_codec_name(ui->videoCodecBox->itemData(index, Qt::UserRole).toString());
    }
    if(index > 0)
    {
        Q_EMIT this->send_selected_video_codec_name(ui->videoCodecBox->currentText());
    }
}

void VideoInterface::select_video_resolution(const int &index)
{
    const int message_timeout{0};
    this->selection.video_resolution_selection.clear();

    if(index == 0)//source
    {
        //clicking "Source" will set the source file resolution as the selected resolution
        this->selection.video_resolution_selection = ui->videoResolutionBox->itemData(index, Qt::UserRole).toString();/**/
        Q_EMIT this->send_video_statusbar_message(ui->videoResolutionBox->itemData(index, Qt::UserRole).toString(), message_timeout);
    }
    else if(index == 1)//separator
    {
        //option one (1) cannot be selected by the user - it is the separator
        return;
    }
    else if(index >= 2 && index <= videodata.videoResolutionList.size())
    {
        this->selection.video_resolution_selection = ui->videoResolutionBox->currentText();
        Q_EMIT this->send_video_statusbar_message(ui->videoResolutionBox->currentText(), message_timeout);
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
            this->selection.video_display_aspect_ratio_selection = ui->videoAspectRatioBox->itemData(index, Qt::UserRole).toString();
            Q_EMIT this->send_video_statusbar_message(ui->videoAspectRatioBox->itemData(index, Qt::UserRole).toString(), message_timeout);
        }
        else
        {
            //clicking "Source" will set the source display aspect ratio as the selected display aspect ratio
            this->selection.video_display_aspect_ratio_selection = ui->videoAspectRatioBox->itemData(index, Qt::UserRole).toString();
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
        this->selection.video_display_aspect_ratio_selection = ui->videoAspectRatioBox->currentText();
        Q_EMIT this->send_video_statusbar_message(ui->videoAspectRatioBox->currentText(), message_timeout);
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
        this->selection.video_framerate_selection << ui->videoFramerateBox->itemData(index, Qt::UserRole).toString();
        Q_EMIT this->send_video_statusbar_message(ui->videoFramerateBox->itemData(index, Qt::UserRole).toString(), message_timeout);
    }
    else if(index == 1)//separator
    {
        //option one (1) cannot be selected by the user - it is the separator
        return;
    }
    else if(index >= 2 && index <= videodata.videoFramerateList.size())
    {
        this->selection.video_framerate_selection << ui->videoFramerateBox->currentText();
        Q_EMIT this->send_video_statusbar_message(ui->videoFramerateBox->currentText(), message_timeout);
    }
    else
    {
        return;
    }
}

void VideoInterface::select_video_colorspace(const int &index)
{
    const int message_timeout{0};
    this->selection.video_colorspace_selection.clear();

    if(index == 0)
    {
        //clicking "Source" will set the source cocolorspace values as the selected values
        this->selection.video_colorspace_selection << command.colorspace_flag /*<< command.bt709_val*/<< this->selection.video_color_space
                                                   << command.color_primaries_flag /*<< command.bt709_val*/<< this->selection.video_color_primaries
                                                   << command.color_transfer_flag /*<< command.bt709_val*/<< this->selection.video_color_transfer;
        Q_EMIT this->send_video_statusbar_message(ui->videoColorspaceBox->itemData(index, Qt::UserRole).toString(), message_timeout);
    }
    else if(index == 1)//separator
    {
        //option one (1) cannot be selected by the user - it is the separator
        return;
    }
    else if(index == 2)//"Default"
    {
        //to force ffmpeg to decide on the colorspace values, set the colorspace value to an empty string
        this->selection.video_colorspace_selection << "";
        Q_EMIT this->send_video_statusbar_message(ui->videoColorspaceBox->currentText(), message_timeout);
    }
    else if(index == 3)//separator
    {
        //option three (3) cannot be selected by the user - it is the separator
        return;
    }
    else if(index == 4)//sRGB
    {
        //-colorspace bt709 -color_trc srgb
        //If srgb does not work, try the more academic name iec61966-2-1 -->https://trac.ffmpeg.org/wiki/colorspace
        //defaulting to iec61966-2-1
        this->selection.video_colorspace_selection << command.colorspace_flag << command.bt709_val
                                                   << command.color_primaries_flag << command.bt709_val
                                                   << command.color_transfer_flag << command.iec61966_2_1_val;
        Q_EMIT this->send_video_statusbar_message(ui->videoColorspaceBox->currentText(), message_timeout);
    }
    else if(index == 5)//BT601
    {
        //-colorspace smpte170m -color_primaries smpte170m -color_trc smpte170m
        this->selection.video_colorspace_selection << command.colorspace_flag << command.smpte170m_val
                                                   << command.color_primaries_flag << command.smpte170m_val
                                                   << command.color_transfer_flag << command.smpte170m_val;
        Q_EMIT this->send_video_statusbar_message(ui->videoColorspaceBox->currentText(), message_timeout);
    }
    else if(index == 6)//BT709
    {
        //-color_primaries bt709 -color_trc bt709 -colorspace bt709 is the same as -color_primaries 1 -color_trc 1 -colorspace 1
        this->selection.video_colorspace_selection << command.colorspace_flag << command.bt709_val
                                                   << command.color_primaries_flag << command.bt709_val
                                                   << command.color_transfer_flag << command.bt709_val;
        Q_EMIT this->send_video_statusbar_message(ui->videoColorspaceBox->currentText(), message_timeout);
    }
    else if(index == 7)//BT2020
    {
        //-color_primaries bt2020 -color_trc bt2020-10/-color_trc smpte2084 for HDR10/-color_trc arib-std-b67 for HLG
        //-colorspace bt2020nc/-colorspace bt2020ncl -->ncl = non-constant luminance
        this->selection.video_colorspace_selection << command.colorspace_flag << command.bt2020_ncl_val
                                                   << command.color_primaries_flag << command.bt2020_val
                                                   << command.color_transfer_flag << command.bt2020_10bit;
        Q_EMIT this->send_video_statusbar_message(ui->videoColorspaceBox->currentText(), message_timeout);
    }
    else if(index == 8)//BT2100 (PQ)
    {
        //-color_primaries bt2020, -color_trc smpte2084, -colorspace bt2020nc
        this->selection.video_colorspace_selection << command.colorspace_flag << command.bt2020_ncl_val
                                                   << command.color_primaries_flag << command.bt2020_val
                                                   << command.color_transfer_flag << command.smpte2084_val;
        Q_EMIT this->send_video_statusbar_message(ui->videoColorspaceBox->currentText(), message_timeout);

    }
    else if(index == 9)//BT2100 (HLG)
    {
        //-color_primaries bt2020 -color_trc arib-std-b67 -colorspace bt2020nc
        this->selection.video_colorspace_selection << command.colorspace_flag << command.bt2020_ncl_val
                                                   << command.color_primaries_flag << command.bt2020_val
                                                   << command.color_transfer_flag << command.arib_std_b67_val;
        Q_EMIT this->send_video_statusbar_message(ui->videoColorspaceBox->currentText(), message_timeout);
    }
    else
    {
        return;
    }
}

void VideoInterface::get_video_interface_selections()
{
    this->process_video_interface_selections();
}

void VideoInterface::process_video_interface_selections()
{
    this->selection.video_selection_list.clear();

    if(selection.copy_video_enabled == true)
    {
        //send the copy video stream command
        this->selection.video_selection_list << command.video_codec_flag << command.copy_command;
    }
    if(selection.copy_video_enabled == false)
    {
        const int index0{0};
        if(ui->videoAspectRatioBox->itemText(index0).contains("setdar=", Qt::CaseInsensitive))
        {
            //check if the display aspect ratio command has the setdar= command and add it to the
            //resolution command-> -filter:v scale=(resolution),setdar=(display_aspect_ratio) if it does -
            //adding the selections to the video selection list
            this->selection.video_selection_list << command.video_codec_flag
                                                 << this->selection.video_codec_selection
                                                 << command.video_filter_flag
                                                 << command.video_scale+this->selection.video_resolution_selection+","+
                                                        this->selection.video_display_aspect_ratio_selection//add setdar= to -filter:v
                                                 << command.video_fps_flag
                                                 << this->selection.video_framerate_selection
                                                 << this->selection.video_colorspace_selection;
        }
        else
        {
            //send the transcode video stream command along with the video options
            //adding the selections to the video selection list
            this->selection.video_selection_list << command.video_codec_flag
                                                 << this->selection.video_codec_selection
                                                 << command.video_filter_flag
                                                 << command.video_scale+this->selection.video_resolution_selection
                                                 << command.video_dar_flag
                                                 << this->selection.video_display_aspect_ratio_selection
                                                 << command.video_fps_flag
                                                 << this->selection.video_framerate_selection
                                                 << this->selection.video_colorspace_selection;
                                                 /*<< command.colorspace_flag << this->selection.video_color_space
                                                 << command.color_primaries_flag << this->selection.video_color_primaries
                                                 << command.color_transfer_flag << this->selection.video_color_transfer;*/
        }
    }

    //send the video selections
    Q_EMIT this->send_video_interface_selections(this->selection.video_selection_list);
}
