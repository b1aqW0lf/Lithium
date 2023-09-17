#ifndef INPUT_SOURCE_PROBE_H
#define INPUT_SOURCE_PROBE_H

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


#include <QProcess>
#include <QObject>


class InputSourceProbe : public QObject
{
    Q_OBJECT

public:
    explicit InputSourceProbe(QObject *parent = nullptr);
    ~InputSourceProbe();

public Q_SLOTS:
    void start_probe_process(const QString &file);

Q_SIGNALS:
    void file_path(const QString &text, const int &time);
    //experimental
    void ffprobe_started_message(const QString &message, const int &time_out);

    //source video signals
    void source_vid_codec_name(const QString &codec);
    void source_vid_resolution(const QString &display);
    void source_vid_frame_rate(const QString &framerate);
    void source_vid_sample_aspect_ratio(const QString &sar);
    void source_vid_display_aspect_ratio(const QString &dar);
    void source_vid_bit_rate(const QString &bitrate);
    void source_vid_duration(const QString &total);
    void source_vid_dur_hours(const int &duration_hours);
    void source_vid_dur_mins(const int &duration_mins);
    void source_vid_dur_secs(const double &duration_secs);
    void source_vid_start_time(const double &start);
    void source_vid_pixel_format(const QString &pix_fmt);
    void source_vid_color_space(const QString &cspace);

    //source audio signals
    void source_audio_codec_name(const QString &codec);
    void source_audio_samplerate(const QString &samplerate);
    void source_audio_bitrate(const QString &bitrate);
    void source_audio_channels(const QString &channel);

    //used for testing
    void show_vid_data(const QString &codec, const int &timeout);
    void show_audio_data(const QString &codec, const int &timeout);

private:
    struct VideoStream
    {
        QString video_str{};
        QString input_file_type{};
        QString codec_name{};
        QString codec_type{};
        QString codec_profile{};
        QString resolution{};
        QString sample_aspect_ratio{};
        QString display_aspect_ratio{};
        QString color_space{};
        QString pixel_format{};
        QString frame_rate{};
        double start_time{};
        QString duration{};
        int dur_hours{};
        int dur_mins{};
        double dur_secs{};
        QString bit_rate{};
        QString nb_frames{};
        QString handler_name{};
        QString stream_index1{};
        QString stream_index2{};
    };

    struct AudioStream
    {
        QString audio_str{};
        QString codec_name{};
        QString codec_type{};
        QString sample_rate{};
        QString channels{};
        double start_time{};
        QString duration{};
        QString bit_rate{};
        QString stream_index1{};
        QString stream_index2{};
    };

    struct InputStream
    {
        QString stream_str{};
    };

private Q_SLOTS:
    void read_ffprobe_output();

private:
    QString input_vid{};
    QString input_aud{};

    //ffprobe process
    QProcess *ffprobe;
    QString ffprobe_path{"ffprobe"};//for linux

    //struct instances
    VideoStream vidstream;
    AudioStream audiostream;
    InputStream inputstream;

    //functions
    void ffprobe_started();
    void parse_output(const QString &data);
    void parse_video_output(const QString &data);
    void parse_audio_output(const QString &data);

};

#endif // INPUT_SOURCE_PROBE_H
