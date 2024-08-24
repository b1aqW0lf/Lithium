#ifndef TRANSCODE_H
#define TRANSCODE_H


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


#include <QObject>
#include <QProcess>
#include <QWidget>


class Transcode : public QWidget
{
    Q_OBJECT

public:
    explicit Transcode(QWidget *parent = nullptr);
    ~Transcode();

Q_SIGNALS:
    void source_file_status(const QString &status, const int &timeout);
    void output_file_status(const QString &status, const int &timeout);
    void send_encoder_status(const QString &status, const int &timeout);
    void enable_encode_button();

public Q_SLOTS:
    //video processing
    void receive_source_video_file(const QString &source_video, const QString &input_flag);
    void receive_source_audio_file(const QString &source_audio, const QString &input_flag);
    void receive_output_file_path(const QString &output_path);
    void two_pass_encode_enabled(const bool &status);
    void average_bitrate_encode_enabled(const bool &status);
    void transcode_processing_mode(const bool &normal, const bool &merge, const bool &extract);

    //receive current video options - in normal mode
    void receive_current_video_options(const QString &codec, const QString &video_bitrate,
                                       const QString &crf_value, const QString &qscale_value,
                                       const QString &video_res_value, const QString &video_dar_value,
                                       const bool &calculate_dar_enabled, const QString &video_fps_val,
                                       const QString &encoder_preset_val, const QStringList &pixel_format,
                                       const bool &pixel_format_enabled);

    //audio processing
    //receive current audio options - in normal mode
    void receive_current_audio_options(const QString &codec, const QString &channels,
                                       const QString &samplerate, const QString &bitrate);

    //start encode process
    void start_encode_process();
    //cancel
    void cancel_encode_process();

private Q_SLOTS:
    void encoding_process_started();
    void encoding_process_finished(const int &index);

private:
    //ffmpeg process
    QProcess *ffmpeg;
    QString ffmpeg_path{};
    QStringList args{};

    //functions
    void source_video_file_check();
    void source_audio_file_check();
    void output_file_path_check();
    void normal_mode_transcode_check();
    void normal_mode_transcode();
    void average_bitrate_encode();
    void two_pass_encode_1st_pass();
    void two_pass_encode_2nd_pass();
    void merge_mode_transcode();
    void extract_mode_transcode();
    void ffmpeg_path_check();
    void start_encode_mode_check();

    //processing variables
    QString source_video_file{};
    QString source_audio_file{};
    QString output_file{};
    QString input_file_flag1{};
    QString input_file_flag2{};
    bool two_pass_enabled{};
    bool average_bitrate_enabled{};

    //transcoder variables
    //video
    QString video_codec{};
    QString crf_value{};
    QString qscale_value{};
    QString vid_avg_bitrate{};
    QString video_res{};
    QString video_dar{};
    QString video_fps{};
    QString vid_encoder_preset{};
    QStringList pixel_format{};
    bool pixel_format_enabled{};
    bool calculate_dar_enabled{};

    //transcode variable
    //2-pass log
    QString pass_log_location{};
    //audio
    QString audio_codec{};
    QString audio_channels{};
    QString audio_samplerate{};
    QString audio_bitrate{};

    //mode variables
    bool normal_mode{};
    bool merge_mode{};
    bool extract_mode{};

};

#endif // TRANSCODE_H
