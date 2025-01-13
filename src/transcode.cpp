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

    connect(this->ffmpeg, &QProcess::started, this, &Transcode::encoding_process_started);
}

Transcode::~Transcode()
{
    //stop ffmpeg if application closes before it exits normally
    delete ffmpeg;
}

void Transcode::receive_source_video_file(const QString &source_video, const QString &input_flag)
{
    this->input_file_flag1 = input_flag;
    this->source_video_file = source_video;
}

void Transcode::receive_source_audio_file(const QString &source_audio, const QString &input_flag)
{
    this->input_file_flag2 = input_flag;
    this->source_audio_file = source_audio;
}

void Transcode::source_video_file_check()
{
    //conversion preparation
    int timeout{0};

    //check for existence of the input file
    if(this->source_video_file.isEmpty())
    {
        Q_EMIT source_file_status(tr("No Input"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Input file not specified"));

        //ui->actionEncode->setChecked(false);
        Q_EMIT enable_encode_button();
        return; //nothing is returned
    }
}

void Transcode::source_audio_file_check()
{
    //conversion preparation
    int timeout{0};

    //check for existence of the input file
    if(this->source_audio_file.isEmpty())
    {
        Q_EMIT source_file_status(tr("No Input"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Input file not specified"));

        //ui->actionEncode->setChecked(false);
        Q_EMIT enable_encode_button();
        return; //nothing is returned
    }
}

void Transcode::receive_output_file_path(const QString &output_path)
{
    this->output_file = output_path;
}

void Transcode::output_file_path_check()
{
    int timeout{0};

    //check for the existence of a specified output file
    if(this->output_file.isEmpty())
    {
        Q_EMIT output_file_status(tr("No Output"), timeout);
        QMessageBox::information(this, tr("Lithium"),
                                 tr("Output file not specified"));

        Q_EMIT enable_encode_button();
        return; //nothing is returned
    }

    //check if specified output file already exists
    if(QFile::exists(this->output_file))
    {
        //use output_vid_file in this test
        Q_EMIT output_file_status(tr("Destination file already exists"), timeout);
        if(QMessageBox::question(this, tr("Lithium"),
                                  tr("There already exists a file called %1 in "
                                     "the currect directory. Overwrite file?").arg(output_file),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            Q_EMIT enable_encode_button();
            return;
        QFile::remove(this->output_file);

        while(QFile::exists(this->output_file))
        {
            Q_EMIT output_file_status(tr("Output file path is set"), timeout);
        }
    }
}

void Transcode::two_pass_encode_enabled(const bool &status)
{
    this->two_pass_enabled = status;
}

void Transcode::average_bitrate_encode_enabled(const bool &status)
{
    this->average_bitrate_enabled = status;
}

void Transcode::transcode_processing_mode(const bool &normal, const bool &merge, const bool &extract)
{
    this->normal_mode = normal;
    this->merge_mode = merge;
    this->extract_mode = extract;
}

//receive current audio options
void Transcode::receive_current_audio_options(const QString &codec, const QString &channels,
                                              const QString &samplerate, const QString &bitrate)
{
    this->audio_codec = codec;
    this->audio_channels = channels;
    this->audio_samplerate = samplerate;
    this->audio_bitrate = bitrate;
}

//receive current video options
void Transcode::receive_current_video_options(const QString &codec, const QString &video_bitrate,
                                              const QString &crf_qscale_value, const QString &video_res_value,
                                              const QString &video_dar_value, const bool &calculate_dar_enabled,
                                              const QString &video_fps_val, const QString &encoder_preset_val,
                                              const QStringList &pixel_format, const bool &pixel_format_enabled,
                                              const QString &codec_profile, const bool &codec_profile_enabled,
                                              const QString &codec_level, const bool &codec_level_enabled)
{
    this->video_codec = codec;
    this->vid_avg_bitrate = video_bitrate;
    this->crf_qscale_value = crf_qscale_value;
    this->video_res = video_res_value;
    this->video_dar = video_dar_value;
    this->calculate_dar_enabled = calculate_dar_enabled;
    this->video_fps = video_fps_val;
    this->vid_encoder_preset = encoder_preset_val;
    this->pixel_format = pixel_format;
    this->pixel_format_enabled = pixel_format_enabled;
    this->video_codec_profile = codec_profile;
    this->codec_profile_enabled = codec_profile_enabled;
    this->video_codec_level = codec_level;
    this->video_codec_level_enabled = codec_level_enabled;
}

void Transcode::start_encode_process()
{
    //start encode process
    start_encode_mode_check();/**/
}


void Transcode::start_encode_mode_check()
{
    if(this->normal_mode == true)
    {
        normal_mode_transcode_check();
    }
    else if(this->merge_mode == true)
    {
        merge_mode_transcode();
    }
    else if(this->extract_mode == true)
    {
        extract_mode_transcode();
    }
}

void Transcode::normal_mode_transcode_check()
{
    //input and output file check
    source_video_file_check();
    output_file_path_check();

    //normal mode check
    if(this->average_bitrate_enabled == false)
    {
        normal_mode_transcode();
    }
    if(this->average_bitrate_enabled == true)
    {
        if(this->two_pass_enabled == false)
        {
            average_bitrate_encode();
        }
        if(this->two_pass_enabled == true)
        {
            two_pass_encode_1st_pass();
            two_pass_encode_2nd_pass();
        }
    }
}

void Transcode::normal_mode_transcode()
{
    int timeout{0};

    //normal transcode
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-sn" << "-c:v" << video_codec;    

    if(video_codec.contains("VP9", Qt::CaseInsensitive))
    {
        args << "-b:v" << "0";
    }
    if(pixel_format_enabled == true)
    {
        args.append(pixel_format);
    }
    if(codec_profile_enabled == true)
    {
        args << "-profile:v" << video_codec_profile;
    }
    if(video_codec_level_enabled == true)
    {
        args << "-level:v" << video_codec_level;
    }

    if(calculate_dar_enabled == true)
    {
        args << "-vf" << video_res+","+video_dar;
    }
    else
    {
        args << "-vf" << video_res << "-aspect" << video_dar;
    }    

    if(video_codec.contains("Theora", Qt::CaseInsensitive))
    {
        args << "-qscale:v" << crf_qscale_value << "-b:v" << "0";
    }
    else
    {
        args << "-crf" << crf_qscale_value;
    }
    args << "-preset" << vid_encoder_preset << "-color_primaries" << "1" << "-color_trc"
         << "1" << "-colorspace" << "1" << "-c:a" << audio_codec << "-map_metadata" << "0"
         << output_file;

    //check for path to ffmpeg
    ffmpeg_path_check();

    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    args.clear();
}

void Transcode::average_bitrate_encode()
{
    int timeout{0};

    //normal transcode + average bitrate
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-c:v" << video_codec;

    if(pixel_format_enabled == true)
    {
        args.append(pixel_format);
    }
    if(codec_profile_enabled == true)
    {
        args << "-profile:v" << video_codec_profile;
    }
    if(video_codec_level_enabled == true)
    {
        args << "-level:v" << video_codec_level;
    }

    if(calculate_dar_enabled == true)
    {
        args << "-vf" << video_res+","+video_dar;
    }
    else
    {
        args << "-vf" << video_res << "-aspect" << video_dar;
    }

    args << "-b:v" << vid_avg_bitrate << "-preset" << vid_encoder_preset
         << "-color_primaries" << "1" << "-color_trc" << "1" << "-colorspace"
         << "1" << "-c:a" << audio_codec << "-map_metadata" << "0" << output_file;

    //check for path to ffmpeg
    ffmpeg_path_check();

    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    args.clear();
}

void Transcode::two_pass_encode_1st_pass()
{
    int timeout{0};
    int msecs{-1};

    //normal transcode + 2-pass encoding
    //1st pass
    QDir file = QFileInfo(source_video_file).absoluteDir();
    pass_log_location = file.absolutePath()+"/ffmpeg2pass";

    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-sn" << "-c:v" << video_codec;

    if(pixel_format_enabled == true)
    {
        args.append(pixel_format);
    }
    if(codec_profile_enabled == true)
    {
        args << "-profile:v" << video_codec_profile;
    }
    if(video_codec_level_enabled == true)
    {
        args << "-level:v" << video_codec_level;
    }

    if(calculate_dar_enabled == true)
    {
        args << "-vf" << video_res+","+video_dar;
    }
    else
    {
        args << "-vf" << video_res << "-aspect" << video_dar;
    }

    args << "-b:v" << vid_avg_bitrate << "-preset" << vid_encoder_preset
         << "-map_metadata" << "0" << "-passlogfile" << pass_log_location
         << "-pass" << "1" << "-an" << "-f" << "null"
#ifdef Q_OS_LINUX
         << "/dev/null";
#elif defined Q_OS_WIN
         << "NUL";
#endif

    //check for path to ffmpeg
    ffmpeg_path_check();

    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        //this logic works!
        Q_EMIT send_encoder_status(tr("2 Pass Encode Started - 1st Pass "), timeout);
    }
    this->ffmpeg->waitForFinished(msecs);
    args.clear();
}

void Transcode::two_pass_encode_2nd_pass()
{
    int timeout{0};
    int msecs{-1};

    //normal transcode + 2-pass encoding
    //2nd Pass
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-sn" << "-c:v" << video_codec;

    if(pixel_format_enabled == true)
    {
        args.append(pixel_format);
    }
    if(codec_profile_enabled == true)
    {
        args << "-profile:v" << video_codec_profile;
    }
    if(video_codec_level_enabled == true)
    {
        args << "-level:v" << video_codec_level;
    }

    if(calculate_dar_enabled == true)
    {
        args << "-vf" << video_res+","+video_dar;
    }
    else
    {
        args << "-vf" << video_res << "-aspect" << video_dar;
    }

    args << "-b:v" << vid_avg_bitrate << "-preset" << vid_encoder_preset
         << "-map_metadata" << "0" << "-passlogfile" << pass_log_location
         << "-pass" << "2" << "-c:a" << audio_codec << output_file;

    //check for path to ffmpeg
    ffmpeg_path_check();

    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    //this->ffmpeg->waitForStarted();
    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        //this logic works!
        Q_EMIT send_encoder_status(tr("2 Pass Encode Started - 2nd Pass "), timeout);
    }
    this->ffmpeg->waitForFinished(msecs);
    args.clear();
}

void Transcode::merge_mode_transcode()
{
    int timeout{0};
    source_video_file_check();
    source_audio_file_check();
    output_file_path_check();

    //merge sources transcode
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-i" << source_audio_file
         << "-c:v" << "copy" << "-c:a" << "copy" << "-map_metadata"
         << "0" << output_file;    

    //check for path to ffmpeg
    ffmpeg_path_check();

    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    args.clear();
}

void Transcode::extract_mode_transcode()
{
    int timeout{0};
    source_video_file_check();
    output_file_path_check();

    //extract audio transcode
    args << "-v" << "warning" << "-hide_banner" << "-stats" << "-y"
         << "-i" << source_video_file << "-vn" << "-c:a" << audio_codec
         << "-map_metadata" << "0" << output_file;    

    //check for path to ffmpeg
    ffmpeg_path_check();

    this->ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    this->ffmpeg->start(this->ffmpeg_path, args);
    this->ffmpeg->waitForStarted();
    args.clear();
}

void Transcode::ffmpeg_path_check()
{
#ifdef Q_OS_WIN
    QString application_path{QCoreApplication::applicationDirPath()};
    QString application_dir{QDir(application_path).absolutePath()};

    if(QFile::exists(application_dir+"/ffmpeg.exe") &&
       QFile::exists(application_dir+"/ffmpeg/ffmpeg.exe"))
    {
        //If ffmpeg is found in both root directory and subdirectory
        //use ffmpeg found in user-created "ffmpeg" subdirectory
        this->ffmpeg_path = application_dir+"/ffmpeg/ffmpeg.exe";
        this->ffmpeg->setWorkingDirectory(application_dir+"/ffmpeg");
    }
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
}

void Transcode::cancel_encode_process()
{
    int timeout{0};

    if(this->ffmpeg->QProcess::state() == QProcess::NotRunning)
    {
        //check for existence of the input file
        if(this->source_video_file.isEmpty())
        {
            Q_EMIT source_file_status(tr("No Input to cancel"), timeout);
            QMessageBox::information(this, tr("Lithium"),
                                     tr("Input file not specified"));
            return; //nothing is returned
        }

        //check for the existence of a specified output file
        if(this->output_file.isEmpty())
        {
            Q_EMIT output_file_status(tr("No Output is being processed"), timeout);
            QMessageBox::information(this, tr("Lithium"),
                                     tr("No Output file was specified"));
            return; //nothing is returned
        }

        //check if source_vid_file has data but ffmpeg is not running
        if(!this->source_video_file.isEmpty())
        {
            this->ffmpeg->closeWriteChannel();
            Q_EMIT send_encoder_status(tr("Encoding Cancelled "), timeout);
        }
    }
    else
    {
        this->ffmpeg->kill();
        this->ffmpeg->close();
        this->ffmpeg->closeWriteChannel();
        Q_EMIT send_encoder_status(tr("Encoding Cancelled "), timeout);
    }
}

void Transcode::encoding_process_started()
{
    int timeout{0};

    if(this->ffmpeg->QProcess::state() == QProcess::Running)
    {
        if(normal_mode == true &&
            average_bitrate_enabled == false)
        {
            //this logic works!
            Q_EMIT send_encoder_status(tr("Encoding Started "), timeout);
        }
        else if(average_bitrate_enabled == true &&
                   two_pass_enabled == false)
        {
            Q_EMIT send_encoder_status(tr("Average Bitrate Encoding Started "), timeout);
        }
        else if(average_bitrate_enabled == true &&
                 two_pass_enabled == true)
        {
            Q_EMIT send_encoder_status(tr("Two Pass Encoding Started "), timeout);
        }
        else if(merge_mode == true)
        {
            Q_EMIT send_encoder_status(tr("Encoding Started -- Merging "), timeout);
        }
        else if(extract_mode == true)
        {
            Q_EMIT send_encoder_status(tr("Encoding Started -- Extracting "), timeout);
        }
        else
        {
            return;
        }
    }
}

void Transcode::encoding_process_finished(const int &index)
{
    int timeout{0};

    //Set the encoding status by checking output file's existence
    this->ffmpeg->waitForFinished();

    if(index == 0)//QProcess::NormalExit
    {
        if(QFile::exists(output_file))
        {
            Q_EMIT send_encoder_status(tr("Encoding Status: Successful "), timeout);
            //playOutput->setEnabled(true);//<===pay attention to this!!, no ui
            Q_EMIT enable_encode_button();
            this->ffmpeg->closeWriteChannel();//close write channel after encoding finishes
        }
    }
    else if(index == 1)//QProcess::CrashExit
    {
        Q_EMIT send_encoder_status(tr("Encoding Status: Stopped Abnormally "), timeout);
    }
}
