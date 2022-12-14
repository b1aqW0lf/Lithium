/******************************************************************************
 Copyright (c) 2020-2022 b1aqW0lf
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


#include "audio_ui.h"

AudioUI::AudioUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->audioCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_aud_codec);
    connect(ui->audioBitrateBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_aud_bitrate);
    connect(ui->audioSampleBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_samplerate);
    connect(ui->audioChannelBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioUI::select_channels);
    //-------------------------------------------------------------------------


    //Initialize audio interface widgets
    //audio codec
    ui->audioCodecBox->insertItem(0, "Source");
    ui->audioCodecBox->insertSeparator(1);
    audioCodecList << "AAC" << "AC3" << "AC3_Fixed" << "EAC3" << "FLAC" <<"MP3"
                   << "WAV" << "Vorbis" << "Opus" << "PCM_s16le" << "PCM_s24le"
                   << "PCM_s32le" << "PCM_s16be" << "PCM_s24be" << "PCM_s32be"
                   << "PCM_alaw" << "PCM_mulaw" << "PCM_u8";
    ui->audioCodecBox->insertItems(2, audioCodecList);

    //audio bitrate
    ui->audioBitrateBox->insertItem(0, "Source");
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
    ui->audioChannelBox->insertItem(0, "Source");
    ui->audioChannelBox->insertSeparator(1);
    audioChannelList << "Mono" << "Stereo" << "3" << "4" << "5" << "6" << "7"
                     << "8";
    ui->audioChannelBox->insertItems(2, audioChannelList);

    //group box
    ui->audioGroupBox->setTitle(tr("Audio"));
    ui->audioGroupBox->setAlignment(Qt::AlignLeft);

}

AudioUI::~AudioUI()
{
    delete ui;
}

void AudioUI::select_aud_codec()
{
    if(ui->audioCodecBox->currentIndex() == 0)//it works!
    {
        audio_codec = "copy";
    }
    else if(ui->audioCodecBox->currentIndex() == 2)
    {
        //aac settings
#ifdef Q_OS_WIN
        audio_codec = "libfdk_aac";//create test to verify user has libfdk_aac
#else
        audio_codec = "aac";
#endif
    }
    else if(ui->audioCodecBox->currentIndex() == 8)
    {
        //settings for WAV
        audio_codec = "wavpack";
    }
    else if(ui->audioCodecBox->currentIndex() == 9)
    {
        //ensure vorbis codec is libvorbis
        audio_codec = "libvorbis";
    }
    else if(ui->audioCodecBox->currentIndex() == 10)
    {
        //ensure opus codec is libopus
        audio_codec = "libopus";
    }
    else
    {
        audio_codec = ui->audioCodecBox->currentText().toLower();
    }
}

void AudioUI::select_aud_bitrate()
{
    if(ui->audioBitrateBox->currentIndex() == 0)
    {
       audio_br_value = "copy";
    }
    else
    {
        //audio bitrate processing is done in encoding_started()
        audio_br_value = ui->audioBitrateBox->currentText()+"k";
    }
}

void AudioUI::select_samplerate()
{
    if(ui->audioSampleBox->currentIndex() == 0)
    {
        audio_sr_value = "copy";
    }
    else
    {
        audio_sr_value = ui->audioSampleBox->currentText();
    }
}

void AudioUI::select_channels()
{
    if(ui->audioChannelBox->currentIndex() == 0)
    {
        audio_ac_value = "copy";
    }
    else if(ui->audioChannelBox->currentIndex() == 2)
    {
        //value for mono
        audio_ac_value = "1";
    }
    else if(ui->audioChannelBox->currentIndex() == 3)
    {
        //value for stereo
        audio_ac_value = "2";
    }
    else
    {
        audio_ac_value = ui->audioChannelBox->currentText();
    }
}

