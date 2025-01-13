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


#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QStandardItem>
#include <QStandardItemModel>

#include "audio_ui.h"


namespace AudioStandardItem
{
    QStandardItem *audioCodecBoxItem{};
    QStandardItem *audioBitrateBoxItem{};
    QStandardItem *audioSampleBoxItem{};
    QStandardItem *audioChannelBoxItem{};
    QStandardItem *audioContainerBoxItem{};
}

AudioUI::AudioUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioUI)
{
    ui->setupUi(this);

    //connect signals and slots
    //experimental------------------------------------------------------------------//
    connect(ui->audioCodecBox, QOverload<int>::of(&QComboBox::activated),
            this, &AudioUI::select_audio_codec);
    connect(ui->audioBitrateBox, QOverload<int>::of(&QComboBox::activated),
            this, &AudioUI::select_audio_bitrate);
    connect(ui->audioSampleBox, &QComboBox::textActivated,
            this, &AudioUI::select_samplerate);
    connect(ui->audioChannelBox, &QComboBox::textActivated,
            this, &AudioUI::select_channels);
    connect(ui->audioContainerBox, &QComboBox::textActivated,
            this, &AudioUI::select_audio_container);
    //------------------------------------------------------------------------------//
    /*connect(ui->audioCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_aud_codec);*/
    /*connect(ui->audioBitrateBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_aud_bitrate);*/
    /*connect(ui->audioSampleBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_samplerate);*/
    /*connect(ui->audioChannelBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_channels);*/
    /*connect(ui->audioContainerBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_aud_container);*/
    //-------------------------------------------------------------------------


    //Initialize audio interface widgets
    //audio codec
    AudioStandardItem::audioCodecBoxItem = new QStandardItem();
    AudioStandardItem::audioCodecBoxItem->setData("Source", Qt::DisplayRole);
    QStandardItemModel *audioCodecBoxModel = new QStandardItemModel(this);
    audioCodecBoxModel->setItem(0, AudioStandardItem::audioCodecBoxItem);
    ui->audioCodecBox->setModel(audioCodecBoxModel);

    ui->audioCodecBox->insertSeparator(1);
    audioCodecList << "AAC" << "AC3" << "AC3_Fixed" << "EAC3" << "FLAC" <<"MP3"
                   << "WAV" << "Vorbis" << "Opus" << "PCM_s16le" << "PCM_s24le"
                   << "PCM_s32le" << "PCM_s16be" << "PCM_s24be" << "PCM_s32be"
                   << "PCM_alaw" << "PCM_mulaw" << "PCM_u8";
    ui->audioCodecBox->insertItems(2, audioCodecList);

    //audio bitrate
    AudioStandardItem::audioBitrateBoxItem = new QStandardItem();
    AudioStandardItem::audioBitrateBoxItem->setData("Source", Qt::DisplayRole);
    QStandardItemModel *audioBitrateBoxModel = new QStandardItemModel(this);
    audioBitrateBoxModel->setItem(0, AudioStandardItem::audioBitrateBoxItem);
    ui->audioBitrateBox->setModel(audioBitrateBoxModel);

    ui->audioBitrateBox->insertSeparator(1);
    audioBitrateList << "64" << "80" << "96" << "112" << "128" << "160" <<"192"
                     << "224" << "256" << "320" << "384" << "448" << "512"
                     << "576" << "640" << "1536";
    ui->audioBitrateBox->insertItems(2, audioBitrateList);

    //audio sample rate
    ui->audioSampleBox->insertItem(0, "Source");
    ui->audioSampleBox->insertSeparator(1);
    audioSampleList << "8000" << "11025" << "12000" << "16000" << "22050"
                    << "24000" << "32000" << "44100" << "48000";
    ui->audioSampleBox->insertItems(2, audioSampleList);

    //audio channel
    AudioStandardItem::audioChannelBoxItem = new QStandardItem();
    AudioStandardItem::audioChannelBoxItem->setData("Source", Qt::DisplayRole);
    QStandardItemModel *audioChannelBoxModel = new QStandardItemModel(this);
    audioChannelBoxModel->setItem(0, AudioStandardItem::audioChannelBoxItem);
    ui->audioChannelBox->setModel(audioChannelBoxModel);

    ui->audioChannelBox->insertSeparator(1);
    audioChannelList << "Mono" << "Stereo" << "3" << "4" << "5" << "6" << "7"
                     << "8";
    ui->audioChannelBox->insertItems(2, audioChannelList);

    //audio extension/container
    ui->audioContainerBox->insertItem(0, "Source");
    ui->audioContainerBox->insertSeparator(1);
    audioContainerList << "M4A" << "FLAC" << "MP3" << "WAV" << "OGG" << "OGA"
                       << "AIFF" << "PCM";
    ui->audioContainerBox->insertItems(2, audioContainerList);
    ui->audioContainerLabel->setText("Audio Container");

    //experimental
    //audioContainerBox should be disabled in Normal Mode
    //audioContainerBox should only be used in Extract Audio Mode
    ui->audioContainerBox->setDisabled(true);
    ui->audioContainerLabel->setDisabled(true);

    //group box
    ui->audioGroupBox->setTitle(tr("Audio"));
    ui->audioGroupBox->setAlignment(Qt::AlignLeft);

}

AudioUI::~AudioUI()
{
    delete ui;
}

void AudioUI::receive_audio_source_codec(const QString &codec)
{
    this->source_codec = codec;
    AudioStandardItem::audioCodecBoxItem->setData(this->source_codec, Qt::UserRole);
}

void AudioUI::select_audio_codec(const int index)
{
    switch(index) {
    case 0:
        //copy from source
        this->audio_codec = ui->audioCodecBox->itemData(0).toString();
        break;
    case 1:
        //separator - cannot be selected by user
        break;
    case 2:
        //AAC
#ifdef Q_OS_WIN
        this->audio_codec = "libfdk_aac";//create test to verify user has libfdk_aac
#else
        this->audio_codec = "aac";
#endif
        break;
    case 3:
        //AC3
        this->audio_codec = ui->audioCodecBox->currentText().toLower();
        break;
    case 4:
        //AC3_FIXED
        this->audio_codec = ui->audioCodecBox->currentText().toLower();
        break;
    case 5:
        //EAC3
        this->audio_codec = ui->audioCodecBox->currentText().toLower();
        break;
    case 6:
        //FLAC
        this->audio_codec = ui->audioCodecBox->currentText().toLower();
        break;
    case 7:
        //MP3
        this->audio_codec = "libmp3lame";
        break;
    case 8:
        //WAV
        this->audio_codec = "wavpack";
        break;
    case 9:
        //VORBIS
        this->audio_codec = "libvorbis";
        break;
    case 10:
        //OPUS
        this->audio_codec = "libopus";
        break;
    case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19:
        //PCM_s16le, PCM_s24le, PCM_s32le, PCM_s16be, PCM_s24be, PCM_s32be
        //PCM_alaw, PCM_mulaw, PCM_u8
        this->audio_codec = ui->audioCodecBox->currentText().toLower();
        break;
    }Q_EMIT send_audio_data(audio_codec, 0);
    //Q_EMIT send_audio_codec_name(audio_codec);
}

void AudioUI::receive_audio_source_bitrate(const QString &bitrate)
{
    this->source_bitrate = bitrate;
    AudioStandardItem::audioBitrateBoxItem->setData(this->source_bitrate, Qt::UserRole);
}

void AudioUI::select_audio_bitrate(const int index)
{
    int timeout{0};
    switch(index) {
    case 0:
        //copy from source
        if(source_bitrate.contains("N/A", Qt::CaseInsensitive))
        {
            //assign a default value
            this->audio_bitrate = "128k";
        }
        else
        {
            //audio_bitrate = source;
            this->audio_bitrate = ui->audioBitrateBox->itemData(0).toString()+"k";
        }
        break;
    case 1:
        //separator - cannot be selected by user
        break;
    case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
    case 11: case 12: case 13: case 14: case 15: case 16: case 17:
        this->audio_bitrate = ui->audioBitrateBox->currentText()+"k";
        break;
    }
    Q_EMIT send_audio_data(audio_bitrate, timeout);
}

void AudioUI::receive_audio_source_samplerate(const QString &samplerate)
{
    this->source_samplerate = samplerate;
}

void AudioUI::select_samplerate()
{
    this-> audio_samplerate = this->source_samplerate;

    if(ui->audioSampleBox->currentIndex() == 0)
    {
        //audio_samplerate = "copy";
        audio_samplerate = this->source_samplerate;
    }
    else
    {
        audio_samplerate = ui->audioSampleBox->currentText();
    }
    //Q_EMIT send_audio_samplerate_val(audio_samplerate);
}
void AudioUI::receive_audio_source_channels(const QString &channels)
{
    //check for "mono" or "stereo"
    if(channels.contains("Mono", Qt::CaseInsensitive))
    {
        //use numeric value for mono
        this->source_channels = "1";
    }
    else if(channels.contains("1 channels", Qt::CaseInsensitive))
    {
        this->source_channels = "1";
    }
    else if(channels.contains("Stereo", Qt::CaseInsensitive))
    {
        //use numeric value for stereo
        this->source_channels = "2";
    }
    else
    {
        this->source_channels = channels;
    }
    AudioStandardItem::audioChannelBoxItem->setData(this->source_channels, Qt::UserRole);
}

void AudioUI::select_channels()
{
    this->audio_channels = this->source_channels;

    if(ui->audioChannelBox->currentIndex() == 0)
    {
        //audio_ac_value = "copy";
        audio_channels = this->source_channels;
    }
    else if(ui->audioChannelBox->currentIndex() == 2)
    {
        //value for mono
        audio_channels = "1";
    }
    else if(ui->audioChannelBox->currentIndex() == 3)
    {
        //value for stereo
        audio_channels = "2";
    }
    else
    {
        audio_channels = ui->audioChannelBox->currentText();
    }
    Q_EMIT send_audio_data(audio_channels,0);
}

//receive audio extension
void AudioUI::receive_audio_source_extension(const QString &extension)
{
    this->audio_source_ext = extension;
}

void AudioUI::select_audio_container()
{
    //source file extension from receive_audio_source_data(const QString &text)
    QString audio_ext{this->audio_source_ext};
    if(ui->audioContainerBox->currentIndex() == 0)
    {
        //source file extension
        //copy of the inputLine2Edit source extension
        audio_ext = this->audio_source_ext;
        Q_EMIT send_output_audio_extension(audio_ext);
    }
    if(ui->audioContainerBox->currentIndex() == 2)
    {
        //M4A
        audio_ext = ui->audioContainerBox->currentText().toLower();
        Q_EMIT send_output_audio_extension("."+audio_ext);
    }
    if(ui->audioContainerBox->currentIndex() == 3)
    {
        //FLAC
        audio_ext = ui->audioContainerBox->currentText().toLower();
        Q_EMIT send_output_audio_extension("."+audio_ext);
    }
    if(ui->audioContainerBox->currentIndex() == 4)
    {
        //MP3
        audio_ext = ui->audioContainerBox->currentText().toLower();
        Q_EMIT send_output_audio_extension("."+audio_ext);
    }
    if(ui->audioContainerBox->currentIndex() == 5)
    {
        //WAV
        audio_ext = ui->audioContainerBox->currentText().toLower();
        Q_EMIT send_output_audio_extension("."+audio_ext);
    }
    if(ui->audioContainerBox->currentIndex() == 6)
    {
        //OGG
        audio_ext = ui->audioContainerBox->currentText().toLower();
        Q_EMIT send_output_audio_extension("."+audio_ext);
    }
    if(ui->audioContainerBox->currentIndex() == 7)
    {
        //OGA
        audio_ext = ui->audioContainerBox->currentText().toLower();
        Q_EMIT send_output_audio_extension("."+audio_ext);
    }
    if(ui->audioContainerBox->currentIndex() == 8)
    {
        //AIFF
        audio_ext = ui->audioContainerBox->currentText().toLower();
        Q_EMIT send_output_audio_extension("."+audio_ext);
    }
    if(ui->audioContainerBox->currentIndex() == 9)
    {
        //PCM
        audio_ext = ui->audioContainerBox->currentText().toLower();
        Q_EMIT send_output_audio_extension("."+audio_ext);
    }
}

//experimental
void AudioUI::get_selected_audio_options()
{
    default_options_check();

    //emit the current selected audio options
    Q_EMIT send_current_audio_options(audio_codec, audio_channels,
                                      audio_samplerate, audio_bitrate);
}

void AudioUI::default_options_check()
{
    //setting default value
    //we need a better solution!
    if(ui->audioCodecBox->currentIndex() == 0)
    {
        this->audio_codec = ui->audioCodecBox->itemData(0).toString();
    }
    if(ui->audioBitrateBox->currentIndex() == 0)
    {
        this->audio_bitrate = ui->audioBitrateBox->itemData(0).toString()+"k";
    }
}

void AudioUI::receive_process_mode_state(const bool &normal, const bool &merge,
                                             const bool &extract)
{
    //process mode state
    this->normal_mode = normal;
    this->merge_mode = merge;
    this->extract_mode = extract;

    enable_extract_audio_settings(extract_mode);
}

void AudioUI::enable_extract_audio_settings(const bool &extract)
{
    //extract audio settings
    if(extract == true)
    {
        ui->audioContainerLabel->setEnabled(true);
        ui->audioContainerBox->setEnabled(true);
    }
    else
    {
        ui->audioContainerLabel->setEnabled(false);
        ui->audioContainerBox->setEnabled(false);
    }
}
