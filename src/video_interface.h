#ifndef VIDEO_INTERFACE_H
#define VIDEO_INTERFACE_H

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


#include "video_interface_data.h"
#include "video_commands.h"

#include <QWidget>

namespace Ui {
class VideoInterface;
}

class VideoInterface : public QWidget
{
    Q_OBJECT

public:
    explicit VideoInterface(QWidget *parent = nullptr);
    ~VideoInterface();

Q_SIGNALS:
    void send_video_statusbar_message(const QString &message, const int &timeout);
    void send_selected_video_extension(const QString &extension);
    void send_video_interface_selections(const QStringList &video_selections);
    void send_selected_video_codec_name(const QString &video_codec);

public Q_SLOTS:
    void receive_source_file_video_data(const QString &video_codec, const QString &video_resolution,
                                        const QString &video_framerate, const QString &video_aspect_ratio);
    void receive_source_video_file_extension(const QString &extension);
    void get_video_interface_selections();

private:
    Ui::VideoInterface *ui;
    VideoInterfaceData videodata;
    VideoCommands command;

private Q_SLOTS:
    void enable_copy_source_video();
    void select_video_codec(const int &index);
    void select_video_resolution(const int &index);
    void select_video_framerate(const int &index);
    void select_video_display_aspect_ratio(const int &index);
    void select_video_container(const int &index);

private:
    //functions
    void initialize_video_interface_data();
    void process_source_file_video_data(const QString &video_codec, const QString &video_resolution,
                                        const QString &video_framerate, const QString &video_aspect_ratio);
    void process_source_video_file_extension(const QString &extension);
    void process_video_interface_selections();

    //struct
    struct
    {
        QStringList copy_video_command{};
        QStringList video_codec_selection{};
        QStringList video_resolution_selection{};
        QStringList video_framerate_selection{};
        QStringList video_display_aspect_ratio_selection{};
        QString video_container_selection{};
        //--------------------------------------
        QStringList video_selection_list{};
    }selection;
};

#endif // VIDEO_INTERFACE_H
