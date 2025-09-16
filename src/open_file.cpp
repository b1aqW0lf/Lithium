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


#include "open_file.h"

#include <QFileDialog>


OpenFile::OpenFile(QWidget *parent)
    : QWidget{parent}
{
    //empty constructor
}

OpenFile::~OpenFile()
{
    //empty destructor
}

void OpenFile::open_source_file()
{
    Q_EMIT this->get_current_process_mode();
    this->get_source_file();
}

void OpenFile::current_process_mode(ProcessMode process_mode)
{
    this->process_mode = process_mode;
}

void OpenFile::get_source_file()
{
    if(this->process_mode == ProcessMode::NormalMode || this->process_mode == ProcessMode::ExtractMode)
    {
        QString video_filename = QFileDialog::getOpenFileName(this, QString(), QString()/*, tr("Videos (*.avi *.mp4 *.mp4v *.mkv "
                                                                           "*.ogv *.ts *.vob *.webm *.mpg *.mpeg *.mov *.m2v "
                                                                            "*.flv *.m1v *.m2v *.mpe *.3gp *.3gpp *.ps *.mts "
                                                                            "*.m2ts *.qt *.wmv *.asf *.m4p)")*/);

        if(!video_filename.isEmpty())
        {
            Q_EMIT send_source_video_file(video_filename);
        }
    }
    /*if(this->process_mode == ProcessMode::MergeMode)
    {
        QString video_filename = QFileDialog::getOpenFileName(this, QString(), QString(), tr("Videos (*.avi *.mp4 *.mp4v *.mkv "
                                                                           "*.ogv *.ts *.vob *.webm *.mpg *.mpeg *.mov *.m2v "
                                                                            "*.flv *.m1v *.m2v *.mpe *.3gp *.3gpp *.ps *.mts "
                                                                            "*.m2ts *.qt *.wmv *.asf *.m4p)"));

        if(!video_filename.isEmpty())
        {
            Q_EMIT send_source_video_file(video_filename);
        }

        QString audio_filename = QFileDialog::getOpenFileName(this, QString(), QString(), tr("Audio/Music (*.m4a *.mpa *.mka *.wav "
                                                                                             "*.flac *.ogg *.oga *.ogx *.mp3 *.aiff "
                                                                                             "*.alac)"));

        if(!audio_filename.isEmpty())
        {
            Q_EMIT send_source_audio_file(audio_filename);
        }
    }*///needs additional implementation. The function works
}
