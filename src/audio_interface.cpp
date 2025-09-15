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


#include "audio_interface.h"
#include "src/ui_audio_interface.h"

#include <QAbstractItemModel>
#include <QListView>
#include <QStandardItemModel>
#include <QStringListModel>


AudioInterface::AudioInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AudioInterface)
{
    ui->setupUi(this);

    //connections - signals and slots
    connect(ui->copyAudioCheckBox, &QCheckBox::clicked,
            this, &AudioInterface::enable_copy_source_audio);
    connect(ui->audioCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &AudioInterface::select_audio_codec);
    connect(ui->audioBitrateBox, QOverload<int>::of(&QComboBox::activated),
            this, &AudioInterface::select_audio_bitrate);
    connect(ui->audioSamplerateBox, QOverload<int>::of(&QComboBox::activated),
            this, &AudioInterface::select_audio_samplerate);
    connect(ui->audioChannelBox, QOverload<int>::of(&QComboBox::activated),
            this, &AudioInterface::select_audio_channels);
    connect(ui->audioContainerBox, QOverload<int>::of(&QComboBox::activated),
            this, &AudioInterface::select_audio_container);

    this->initialize_audio_interface_data();
}

AudioInterface::~AudioInterface()
{
    delete ui;
}

void AudioInterface::initialize_audio_interface_data()
{
    ui->audioCodecBox->insertItems(0, audiodata.audioCodecList);
    ui->audioCodecBox->insertSeparator(1);
    ui->audioBitrateBox->insertItems(0, audiodata.audioBitrateList);
    ui->audioBitrateBox->insertSeparator(1);
    ui->audioSamplerateBox->insertItems(0, audiodata.audioSamplerateList);
    ui->audioSamplerateBox->insertSeparator(1);
    ui->audioChannelBox->insertItems(0, audiodata.audioChannelList);
    ui->audioChannelBox->insertSeparator(1);
    ui->audioContainerBox->insertItems(0, audiodata.audioContainerList);
}

void AudioInterface::enable_copy_source_audio()
{
    const int timeout{0};
    this->selection.copy_audio_command.clear();

    if(ui->copyAudioCheckBox->isChecked() == true)
    {
        //send command to copy the source audio stream
        this->selection.copy_audio_command << command.audio_codec_flag << command.copy_command;
        Q_EMIT this->send_audio_statusbar_message("Copy Source Audio Enabled", timeout);
    }
    else
    {
        //send regular command to transcode audio stream
        this->selection.copy_audio_command << command.audio_codec_flag;
        Q_EMIT this->send_audio_statusbar_message("", timeout);//clear the message
    }
}

void AudioInterface::receive_source_file_audio_data(const QString &audio_codec, const QString &audio_bitrate,
                                                    const QString &audio_samplerate, const QString &audio_channels)
{
    //receive source file audio data
    this->process_source_file_audio_data(audio_codec, audio_bitrate, audio_samplerate, audio_channels);
}

void AudioInterface::process_source_file_audio_data(const QString &audio_codec, const QString &audio_bitrate,
                                                    const QString &audio_samplerate, const QString &audio_channels)
{
    //used for the UserRole of the "Source" DisplayRole
    const int index{0};//first index for the comboboxes
    this->ui->audioCodecBox->setItemData(index, audio_codec, Qt::UserRole);
    this->ui->audioBitrateBox->setItemData(index, audio_bitrate, Qt::UserRole);
    this->ui->audioSamplerateBox->setItemData(index, audio_samplerate, Qt::UserRole);
    this->ui->audioChannelBox->setItemData(index, audio_channels, Qt::UserRole);
}

void AudioInterface::select_audio_codec(const int &index)
{
    const int message_timeout{0};
    this->selection.audio_codec_selection.clear();

    if(index == 0)//source
    {
        //clicking "Source" will set the source file codec as the selected codec
        this->selection.audio_codec_selection << ui->audioCodecBox->itemData(index, Qt::UserRole).toString();
        Q_EMIT this->send_audio_statusbar_message(ui->audioCodecBox->itemData(index, Qt::UserRole).toString().toUpper(), message_timeout);
    }
    else if(index > 0 && index <= audiodata.audioCodecList.size())
    {
#ifdef Q_OS_WIN
        if(index == 2)//AAC
        {
            //set libfdk_aac as the aac ffmpeg audio codec on Windows
            this->selection.audio_codec_selection << ui->audioCodecBox->setItemData(index, "libfdk_aac", Qt::UserRole);
            Q_EMIT this->send_audio_statusbar_message(ui->audioCodecBox->itemData(index, Qt::UserRole).toString().toLower();
        }
        else
        {
            //set the selected codec as the desired audio codec
            this->selection.audio_codec_selection << ui->audioCodecBox->itemData(index, Qt::DisplayRole).toString().toLower();
            Q_EMIT this->send_audio_statusbar_message(ui->audioCodecBox->itemData(index, Qt::DisplayRole).toString().toLower(), message_timeout);
        }
#else
        //set the selected codec as the desired audio codec
        this->selection.audio_codec_selection << ui->audioCodecBox->itemData(index, Qt::DisplayRole).toString().toLower();
        Q_EMIT this->send_audio_statusbar_message(ui->audioCodecBox->itemData(index, Qt::DisplayRole).toString(), message_timeout);
#endif
    }
    else
    {
        Q_EMIT this->send_audio_statusbar_message("Something went wrong", message_timeout);
    }
}

void AudioInterface::select_audio_bitrate(const int &index)
{
    const int message_timeout{0};
    this->selection.audio_bitrate_selection.clear();

    if(index == 0)//source
    {
        //clicking "Source" will set the source file audio bitrate as the selected bitrate
        this->selection.audio_bitrate_selection << command.audio_bitrate_flag
                                                << ui->audioBitrateBox->itemData(index, Qt::UserRole).toString()+"k";
        Q_EMIT this->send_audio_statusbar_message(ui->audioBitrateBox->itemData(index, Qt::UserRole).toString(), message_timeout);
    }
    else if(index > 0 && index <= audiodata.audioBitrateList.size())
    {
        //set the selected audio bitrate as the desired bitrate
        this->selection.audio_codec_selection << command.audio_bitrate_flag
                                              << ui->audioBitrateBox->itemData(index, Qt::DisplayRole).toString()+"k";
        Q_EMIT this->send_audio_statusbar_message(ui->audioBitrateBox->itemData(index, Qt::DisplayRole).toString(), message_timeout);
    }
    else
    {
        Q_EMIT this->send_audio_statusbar_message("Something went wrong", message_timeout);
    }
}

void AudioInterface::select_audio_samplerate(const int &index)
{
    const int message_timeout{0};
    this->selection.audio_samplerate_selection.clear();

    if(index == 0)//source
    {
        //clicking "Source" will set the source file audio samplerate as the selected samplerate
        this->selection.audio_samplerate_selection << command.audio_samplerate_flag
                                                   << ui->audioSamplerateBox->itemData(index, Qt::UserRole).toString();
        Q_EMIT this->send_audio_statusbar_message(ui->audioSamplerateBox->itemData(index, Qt::UserRole).toString(), message_timeout);
    }
    else if(index > 0 && index <= audiodata.audioSamplerateList.size())
    {
        //set the selected audio samplerate as the desired samplerate
        this->selection.audio_samplerate_selection << command.audio_samplerate_flag
                                                   << ui->audioSamplerateBox->itemData(index, Qt::DisplayRole).toString();
        Q_EMIT this->send_audio_statusbar_message(ui->audioSamplerateBox->itemData(index, Qt::DisplayRole).toString(), message_timeout);
    }
    else
    {
        Q_EMIT this->send_audio_statusbar_message("Something went wrong", message_timeout);
    }
}

void AudioInterface::select_audio_channels(const int &index)
{
    const int message_timeout{0};
    this->selection.audio_channel_selection.clear();

    if(index == 0)//source
    {
        //clicking "Source" will set the source file audio channel as the selected channel
        if(ui->audioChannelBox->itemData(index, Qt::UserRole).toString().contains("Mono", Qt::CaseInsensitive))
        {
            //set the audio_chanel_selection value to "1" for mono
            this->setup_audio_mono_stereo_channel(index, message_timeout, "1", Qt::UserRole);
        }
        else if(ui->audioChannelBox->itemData(index, Qt::UserRole).toString().contains("Stereo", Qt::CaseInsensitive))
        {
            //set the audio_chanel_selection value to "2" for stereo
            this->setup_audio_mono_stereo_channel(index, message_timeout, "2", Qt::UserRole);
        }
        else
        {
            this->selection.audio_channel_selection << command.audio_channels_flag
                                                    << ui->audioChannelBox->itemData(index, Qt::UserRole).toString();
            Q_EMIT this->send_audio_statusbar_message(ui->audioChannelBox->itemData(index, Qt::UserRole).toString(), message_timeout);
            //this->setup_audio_mono_stereo_channel(index, message_timeout);
        }
    }
    else if(index == 2)//mono
    {
        //set the mono user role to "1"
         this->setup_audio_mono_stereo_channel(index, message_timeout, "1", Qt::UserRole);
    }
    else if(index == 3)//stereo
    {
        //set the stereo user role to "2"
        this->setup_audio_mono_stereo_channel(index, message_timeout, "2", Qt::UserRole);
    }
    else if(index > 3 && index <= audiodata.audioChannelList.size())
    {
        //set the selected audio channel as the desired channel
        this->selection.audio_channel_selection << command.audio_channels_flag
                                                << ui->audioChannelBox->currentText().toLower();
        Q_EMIT this->send_audio_statusbar_message(ui->audioChannelBox->currentText(), message_timeout);
    }
    else
    {
        Q_EMIT this->send_audio_statusbar_message("Something went wrong", message_timeout);
    }
}
void AudioInterface::setup_audio_mono_stereo_channel(const int &index, const int &message_timeout,
                                                     const QString &audio_channel, Qt::ItemDataRole role)
{
    //note: role should be either Qt::DisplayRole or Qt::UserRole
    ui->audioChannelBox->setItemData(index, audio_channel, role);
    this->selection.audio_channel_selection << command.audio_channels_flag
                                            << ui->audioChannelBox->itemData(index, role).toString();
    Q_EMIT this->send_audio_statusbar_message(ui->audioChannelBox->itemData(index, role).toString(), message_timeout);
}

void AudioInterface::select_audio_container(const int &index)
{
    const int message_timeout{0};
    this->selection.audio_container_selection.clear();

    //set the selected audio extension as the desired extension
    this->selection.audio_container_selection = "."+ui->audioContainerBox->itemData(index, Qt::DisplayRole).toString().toLower();
    Q_EMIT this->send_audio_statusbar_message(ui->audioContainerBox->itemData(index, Qt::DisplayRole).toString(), message_timeout);
    Q_EMIT this->send_selected_audio_extension(this->selection.audio_container_selection);
}

void AudioInterface::get_audio_interface_selections()
{
    this->process_audio_interface_selections();
}

void AudioInterface::process_audio_interface_selections()
{
    this->selection.audio_selection_list.clear();

    if(ui->copyAudioCheckBox->isChecked() == true)
    {
        //send the copy audio stream command
        this->selection.audio_selection_list << this->selection.copy_audio_command;//-codec:a copy
    }
    if(ui->copyAudioCheckBox->isChecked() == false)
    {
        const int index0{0};
        //send the transcode audio stream command along with the audio options
        //adding the selections to the audio selection list
        this->selection.audio_selection_list << this->selection.copy_audio_command//-codec:a
                                             << this->selection.audio_codec_selection
                                             << this->selection.audio_bitrate_selection
                                             << this->selection.audio_samplerate_selection
                                             << this->selection.audio_channel_selection;
    }

    //send the audio selections
    Q_EMIT this->send_audio_interface_selections(this->selection.audio_selection_list);
}
