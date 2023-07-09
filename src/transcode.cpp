/******************************************************************************
 Copyright (c) 2020-2023 b1aqW0lf
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


#include "transcode.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>

Transcode::Transcode(QWidget *parent)
    : QWidget{parent}
{
    ffmpeg = new QProcess{this};

    //local connection
    connect(this->ffmpeg, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Transcode::encoding_process_finished);
}

Transcode::~Transcode(){}

void Transcode::receive_source_video_file(const QString &source_video)
{
    this->source_vid_file = source_video;
}

void Transcode::source_input_file_check()
{
    //conversion preparation
    //ffmpeg argument
    QStringList args;
    int timeout{0};

    //check for existence of the input file
    if(this->source_vid_file.isEmpty())
    {
        Q_EMIT source_vid_file_status(tr("No Input"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Input file not specified"));

        //ui->actionEncode->setChecked(false);
        Q_EMIT encode_button_set_checked(false);
        return; //nothing is returned
    }
}

void Transcode::receive_output_file_path(const QString &output_path)
{
    this->output_vid_file = output_path;
}

void Transcode::output_video_path_check()
{
    int timeout{0};

    //check for the existence of a specified output file
    if(this->output_vid_file.isEmpty())
    {
        Q_EMIT output_vid_file_status(tr("No Output"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Output file not specified"));

        Q_EMIT encode_button_set_checked(false);
        return; //nothing is returned
    }

    //check if specified output file already exists
    //use output_vid_file in this test
    Q_EMIT output_vid_file_status(tr("Output file check... ").append(this->output_vid_file), timeout);
    if(QFile::exists(this->output_vid_file))
    {
        if(QMessageBox::question(this, tr("Lithium"),
                                  tr("There already exists a file called %1 in "
                                     "the currect directory. Overwrite file?").arg(output_vid_file),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            return;
        QFile::remove(this->output_vid_file);

        while(QFile::exists(this->output_vid_file))
        {
            Q_EMIT output_vid_file_status(tr("Output file path is set"), timeout);
        }
    }
}

void Transcode::enable_two_pass_encode(const bool &status)
{
    this->two_pass_val = status;
}

//receive current audio options
void Transcode::receive_current_audio_options(const QString &codec, const QString &channels,
                                              const QString &samplerate, const QString &bitrate)
{
    this->audio_codec = codec;
    this->audio_channels = channels;
    this->audio_samplerate = samplerate;
    this->audio_bitrate = bitrate;

    //start encode
    audio_settings_ready = true;
}

//receive current video options
void Transcode::receive_current_video_options(const QString &codec, const QString &video_bitrate,
                                              const QString &crf_value, const QString &qscale_value,
                                              const QString &video_res_value, const QString &video_dar_value,
                                              const QString &video_fps_val, const QString &encoder_preset_val)
{
    this->video_codec = codec;
    this->vid_avg_bitrate = video_bitrate;
    this->crf_value = crf_value;
    this->qscale_value = qscale_value;
    this->video_res = video_res_value;
    this->video_dar = video_dar_value;
    this->video_fps = video_fps_val;
    this->vid_encoder_preset = encoder_preset_val;

    //start encode
    video_settings_ready = true;
    start_encode_check();/**/
}


void Transcode::start_encode_check()
{
    if(video_settings_ready == true && audio_settings_ready == true)
    {
        normal_mode_transcode();/**/
    }
}

//use with actionEncode
void Transcode::start_normal_mode_transcode()
{
    //normal_mode_transcode();
}

void Transcode::normal_mode_transcode()
{
    int timeout{0};
    source_input_file_check();
    output_video_path_check();

    /*//encoding processing
    //sending input and output arguments to ffmpeg and start processing
    //two-pass encoding
    if(ui->twoPassEncode->isChecked())
    {
        //two-pass encoding after video and audio file have been merged
        if(ui->mergeSourcesRadio->isChecked())
        {
            //actual two-pass merged sources code must be written
            args << "-version";
        }
        //two-pass encoding after normal processing
        if(ui->normalModeRadio->isChecked())
        {
            args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                 << "-i" << input_file1 << "-passlogfile" << "ffmpeg2pass"
                 << "-c:v" << video_codec << "-b:v" << "2048k"
                 << "-pass" << "1" << "-r" << vid_framerate << "-color_primaries"
                 << "1" << "-color_trc" << "1" << "-colorspace" << "1"
                 << "-pix_fmt" << "yuv420p" << "-an" << "-f" << "null"
                 << "/dev/null" << "-i" << input_file1 << "-passlogfile"
                 << "ffmpeg2pass" << "-c:v" << video_codec << "-b:v" << "2048k"
                 << "-pass" << "2" << "-r" << video_fps_val << "-color_primaries"
                 << "1" << "-color_trc" << "1" << "-colorspace" << "1"
                 << "-pix_fmt" << "yuv420p" << "-c:a" << audio_codec
                 << output_file;
        }
    }
    if(ui->mergeSourcesRadio->isChecked() &&
        !ui->twoPassEncode->isChecked())
    {
        //merge sources
        args <<"-i"<< input_file1 <<"-i"<< input_file2 << "-c:v" << "copy"
             << "-color_primaries" << "1" << "-color_trc" << "1"
             << "-colorspace" << "1" << "-pix_fmt" << "yuv420p" << "-c:a"
             << "copy" << "-map" << "0:0" << "-map" << "1:0" << output_file;
    }
    if(ui->extractAudioRadio->isChecked() &&
        !ui->twoPassEncode->isChecked())
    {
        //extract audio
        args <<"-i"<< input_file1 << "-vn" << "-c:a" << audio_codec << "-b:a"
             << audio_br_value << output_file;
    }*/
    /*//---> two_pass_val = false;
    if(this->two_pass_val == false)
    {
        //Normal Encoding Mode Processing
        //processing with libxvid specific settings
        if(video_codec == "libxvid")
        {
            args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                 << "-i" << source_vid_file << "-c:v" << video_codec << "-vf"
                 << video_res << "-aspect" << video_dar
                 << "-color_primaries" << "1" << "-color_trc" << "1"
                 << "-colorspace" << "1" << "-r" << vid_framerate
                 << "-qscale:v" << qscale_value << "-g" << "240"
                 << "-bf" << "2" << "-c:a" << audio_codec << "-ar"
                 << audio_samplerate << "-b:a" << audio_bitrate << "-ac"
                 << audio_channels << output_vid_file;
        //---->}*/
        //processing with libvpx-vp9 settings - single pass
        /*else if(ui->videoCodecBox->currentIndex() == 5)
        {
            //disconnect select_qscale() from videoRFSlider
            disconnect(ui->videoRFSlider, SIGNAL(valueChanged(int)),
                       this, SLOT(select_qscale()));
            //reconnect select_crf() to videoRFSlider
            connect(ui->videoRFSlider, SIGNAL(valueChanged(int)),
                    this, SLOT(select_crf()));

            if(ui->videoAVGBitField->text().isEmpty())
            {
                //libvpx constant quality mode
                video_bitrate = "0";
            }
            else if(!ui->videoAVGBitField->text().isEmpty())
            {
                //libvpx constrained quality mode
                video_bitrate = ui->videoAVGBitField->text()+"k";
            }
            else
            {
                video_bitrate = ui->videoAVGBitField->text()+"k";
            }

            args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                 << "-i" << input_file1 << "-c:v" << video_codec << "-b:v"
                 << video_bitrate << "-vf" << video_res_value << "-aspect"
                 << vid_aspect_val << "-color_primaries" << "1" << "-color_trc"
                 << "1" << "-colorspace" << "1" << "-r" << video_fps_val << "-crf"
                 << crf_value << "-b:v" << video_br_value << "-c:a" << audio_codec
                 << "-ar" << audio_sr_value << "-b:a" << audio_br_value << "-ac"
                 << audio_ac_value << output_file;
        }
        //processing with libtheora specific settings
        else if(ui->videoCodecBox->currentIndex() == 6)
        {
            //disconnect select_crf() from videoRFSlider
            disconnect(ui->videoRFSlider, SIGNAL(valueChanged(int)),
                       this, SLOT(select_crf()));
            //connect select_qscale() to videoRFSlider
            connect(ui->videoRFSlider, SIGNAL(valueChanged(int)),
                    this, SLOT(select_qscale()));

            args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                 << "-i" << input_file1 << "-c:v" << video_codec << "-vf"
                 << video_res_value << "-aspect" << vid_aspect_val
                 << "-color_primaries" << "1" << "-color_trc" << "1"
                 << "-colorspace" << "1" << "-r" << video_fps_val << "-qscale:v"
                 << video_qs_value << "-c:a" << audio_codec << "-ar"
                 << audio_sr_value << "-b:a" << audio_br_value << "-ac"
                 << audio_ac_value << output_file;
        }
        //processing with mpeg1/mpeg2 specific settings
        else if(ui->videoCodecBox->currentIndex() == 7 ||
                 ui->videoCodecBox->currentIndex() == 8)
        {
            //disconnect select_crf() from videoRFSlider
            disconnect(ui->videoRFSlider, SIGNAL(valueChanged(int)),
                       this, SLOT(select_crf()));
            //connect select_qscale() to videoRFSlider
            connect(ui->videoRFSlider, SIGNAL(valueChanged(int)),
                    this, SLOT(select_qscale()));

            args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                 << "-i" << input_file1 << "-c:v" << video_codec << "-b:v"
                 << video_bitrate << "-vf" << video_res_value << "-aspect"
                 << vid_aspect_val << "-color_primaries" << "1" << "-color_trc"
                 << "1" << "-colorspace" << "1" << "-r" << video_fps_val
                 << "-bufsize:v" << "20000k" << "-maxrate" << "200000k" << "-bf"
                 << "2" << "-qscale:v" << video_qs_value << "-c:a" << audio_codec
                 << "-ar" << audio_sr_value << "-b:a" << audio_br_value << "-ac"
                 << audio_ac_value << output_file;
        }
        //processing with librav1e/av1 specific settings
        else if(ui->videoCodecBox->currentIndex() == 9)
        {
            //disconnect select_crf() from videoRFSlider
            disconnect(ui->videoRFSlider, &QSlider::valueChanged,
                       this, &MainWindow::select_crf);
            //connect select_qscale() to videoRFSlider
            connect(ui->videoRFSlider, &QSlider::valueChanged,
                    this, &MainWindow::select_qscale);

            args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                 << "-i" << input_file1 << "-c:v" << video_codec << "-b:v"
                 << video_bitrate << "-vf" << video_res_value << "-aspect"
                 << vid_aspect_val << "-pix_fmt" << "yuv420p" << "-f"
                 << "yuv4mpegpipe" << "-color_primaries" << "1" << "-color_trc"
                 << "1" << "-colorspace" << "1" << "-r" << video_fps_val
                 << "-qp" << video_qs_value << "-speed" << pr_value
                 << "-c:a" << audio_codec << "-ar" << audio_sr_value
                 << "-b:a" << audio_br_value << "-ac" << audio_ac_value
                 << output_file;
        }
        //normal processing mode
        //copy, not transcode
        else if(ui->audioCodecBox->currentIndex() == 0 &&
                 ui->audioBitrateBox->currentIndex() == 0 &&
                 ui->audioSampleBox->currentIndex() == 0 &&
                 ui->audioChannelBox->currentIndex() == 0 &&
                 ui->videoCodecBox->currentIndex() == 0 &&
                 ui->videoResBox->currentIndex() == 0 &&
                 ui->videoAspectRatBox->currentIndex() == 0 &&
                 ui->videoFPSBox->currentIndex() == 0)
        {
            args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                 << "-i" << input_file1 << "-c:v" << "copy"
                 << "-color_primaries" << "1" << "-color_trc" << "1"
                 << "-colorspace" << "1" << "-pix_fmt" << "yuv420p" << "-c:a"
                 << "copy" << output_file;
        }*/
        /*else
        {*/
            /*args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
                 << "-i" << source_vid_file << "-c:v" << video_codec << "-vf"
                 << video_res << pixel_format << "-aspect"
                 << video_dar << "-color_primaries" << "1" << "-color_trc"
                 << "1" << "-colorspace" << "1" << "-r" << vid_framerate
                 << "-preset" << vid_encoder_preset << "-crf" << crf_value << "-c:a"
                 << audio_codec << "-ar" << audio_samplerate << "-b:a"
                 << audio_bitrate << "-ac" << audio_channels << output_vid_file; */
        //}
    //}

    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_vid_file << "-c:v" << video_codec << "-crf"
         << crf_value << "-speed" << vid_encoder_preset << "-c:a" << audio_codec
         << "-b:a" << audio_bitrate << output_vid_file;

    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
#ifdef Q_OS_WIN    
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir);
    }
    if(QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
#elif defined Q_OS_LINUX
    this->ffmpeg_path = "ffmpeg";
#endif
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    if((this->ffmpeg->QProcess::state() == QProcess::Running))
    {
        //this logic works!
        Q_EMIT send_encoder_status(tr("Encoding Started "), timeout);
    }
    args.clear();/**/
}

void Transcode::cancel_encode_process()
{
    int timeout{0};

    this->ffmpeg->kill();
    this->ffmpeg->close();
    this->ffmpeg->closeWriteChannel();
    Q_EMIT send_encoder_status(tr("Encoding Cancelled "), timeout);
}

void Transcode::encoding_process_finished()
{
    int timeout{0};

    //Set the encoding status by checking output file's existence
    this->ffmpeg->waitForFinished();

    if(QFile::exists(output_vid_file) &&
        (this->ffmpeg->QProcess::exitStatus() == QProcess::NormalExit))
    {
        Q_EMIT send_encoder_status(tr("Encoding Status: Successful "), timeout);
        //playOutput->setEnabled(true);//<===pay attention to this!!, no ui
    }
    Q_EMIT enable_encode_button();
    this->ffmpeg->closeWriteChannel();//close write channel after encoding finishes
}
