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


#include "input_treeview.h"
#include "src/ui_input_treeview.h"

#include <QFileInfo>


InputTreeView::InputTreeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InputTreeView)
{
    ui->setupUi(this);

    ui->inputTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->inputTreeView->setAlternatingRowColors(true);//<-----
    const QStringList headers{"File Name", "Codec", "Codec Type", "File Size", "Directory"};

    tree_model.set_header_labels(headers);
    tree_model.set_column_count(headers);
    tree_model.generate_model_items(headers);
    tree_model.set_model_items(headers);
    tree_model.set_treeview_model(ui->inputTreeView);
    ui->inputTreeView->setColumnWidth(0, 415);
}

InputTreeView::~InputTreeView()
{
    delete ui;
}

void InputTreeView::receive_source_file(const QString &source_file)
{
    this->source_file = source_file;
}

void InputTreeView::current_process_mode(ProcessMode mode)
{
    this->process_mode = mode;
}

void InputTreeView::receive_input_probe_data(const QString &video_codec, const QString &video_res,
                                             const QString &video_fps, const QString &video_dar,
                                             const QString &pixel_format, const QString &video_codec_type,
                                             const QString &audio_codec, const QString &audio_channels,
                                             const QString &audio_codec_type)
{
    //display received data
    this->display_input_probe_data(video_codec, video_codec_type,
                                   audio_codec, audio_codec_type);
}

void InputTreeView::display_input_probe_data(const QString &video_codec, const QString &video_codec_type,
                                             const QString &audio_codec, const QString &audio_codec_type)
{
    //display received data
    QFileInfo file(this->source_file);

    tree_model.model_item_list[0]->setData(file.completeBaseName(), Qt::DisplayRole);
    tree_model.model_item_list[0]->setData(this->source_file, Qt::UserRole);//used when double-clicking the entry to begin transcoding - not implemented
    tree_model.model_item_list[0]->setToolTip(this->source_file);
    tree_model.model_item_list[1]->setData(video_codec.toUpper(), Qt::DisplayRole);
    tree_model.model_item_list[2]->setData(video_codec_type, Qt::DisplayRole);
    tree_model.model_item_list[3]->setData(QLocale().formattedDataSize(file.size(), 2, QLocale::DataSizeSIFormat), Qt::DisplayRole);
    tree_model.model_item_list[4]->setData(file.absolutePath(), Qt::DisplayRole);

    tree_model.generate_child_items(3);
    tree_model.set_model_child_items(3);
    tree_model.child_item_list[0]->setData(file.completeBaseName(), Qt::DisplayRole);
    tree_model.child_item_list[1]->setData(audio_codec.toUpper(), Qt::DisplayRole);
    tree_model.child_item_list[2]->setData(audio_codec_type, Qt::DisplayRole);

    this->set_model_items_icons();
    this->codec_type_uppercase_check();
}

void InputTreeView::set_model_items_icons()
{
#ifdef Q_OS_WINDOWS
    tree_model.model_item_list[0]->setIcon(QIcon("C:/users/Kraken/Documents/Source_Code/Projects/Yttrium/resources/video.svg"));
    tree_model.child_item_list[0]->setIcon(QIcon("C:/users/Kraken/Documents/Source_Code/Projects/Yttrium/resources/audio.svg"));
#else
    tree_model.model_item_list[0]->setIcon(QIcon("/home/Ultron/Documents/Source_Code/Projects/Yttrium/resources/video.svg"));
    tree_model.child_item_list[0]->setIcon(QIcon("/home/Ultron/Documents/Source_Code/Projects/Yttrium/resources/audio.svg"));
#endif
}

void InputTreeView::codec_type_uppercase_check()
{
    if(tree_model.model_item_list[2]->data(Qt::DisplayRole).toString() == "video")
    {
        tree_model.model_item_list[2]->setData("Video", Qt::DisplayRole);
    }

    if(tree_model.child_item_list[2]->data(Qt::DisplayRole).toString() == "audio")
    {
        tree_model.child_item_list[2]->setData("Audio", Qt::DisplayRole);
    }
}
