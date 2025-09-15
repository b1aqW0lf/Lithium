#ifndef INPUT_PROBE_H
#define INPUT_PROBE_H

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


#include "encoder_process.h"

#include <QObject>
#include <QProcess>


class InputProbe : public QObject
{
    Q_OBJECT

public:
    explicit InputProbe(QObject *parent = nullptr);
    ~InputProbe();

Q_SIGNALS:
    void send_input_probe_data(const QString &video_codec, const QString &video_res,
                               const QString &video_fps, const QString &video_dar,
                               const QString &pixel_format, const QString &video_codec_type,
                               const QString &audio_codec, const QString &audio_channels,
                               const QString &audio_codec_type);

    /*void send_input_probe_video_data(const QString &video_codec, const QString &video_resolution,
                                     const QString &video_file_extension, const QString &video_dar,
                                     const QString &video_fps);//experimental*/

    void send_source_file_audio_data(const QString &audio_codec, const QString &audio_bitrate,
                                     const QString &audio_samplerate, const QString &audio_channels);//experimental

    void send_source_file_video_data(const QString &video_codec, const QString &video_resolution,
                                     const QString &video_framerate, const QString &video_aspect_ratio);//experimental

public Q_SLOTS:
    void receive_source_file(const QString &source_file);
    void read_ffprobe_output(QString &ffprobe_output); //new

private Q_SLOTS:
    void ffprobe_standard_output();
    //void read_ffprobe_output(const int &exit_code);

private:
    //ffprobe
    QProcess *ffprobe;
    EncoderProcess encoder;

    //function
    void start_ffprobe_process(const QString &source_file);
    void ffprobe_path_check();
    void parse_video_output(QString &output);
    void parse_audio_output(QString &output);
    void get_display_aspect_ratio(QString &output);
    /*void get_source_file_extension();//new-->experimental*/
    void send_output_data();

    //variables
    QString ffprobe_path{};

    struct VideoStream
    {
        QString video_str{};
        QString codec_name{};
        QString codec_long_name{};
        QString codec_type{};
        QString codec_profile{};
        QString width{};
        QString height{};
        QString coded_width{};
        QString coded_height{};
        QString resolution{};
        QString sample_aspect_ratio{};
        QString display_aspect_ratio{};
        QString color_space{};
        QString pixel_format{};
        QString framerate{};
        double start_time{};
        QString duration{};
        int dur_hours{};
        int dur_mins{};
        double dur_secs{};
        QString bitrate{};
        QString nb_frames{};
        QString handler_name{};
        QString stream_index1{};
        QString stream_index2{};
    }videostream;

    struct AudioStream
    {
        QString audio_str{};
        QString codec_name{};
        QString codec_type{};
        QString codec_profile{};
        QString samplerate{};
        QString channels{};
        double start_time{};
        QString duration{};
        QString bitrate{};
        QString stream_index1{};
        QString stream_index2{};
    }audiostream;
};

#endif // INPUT_PROBE_H
