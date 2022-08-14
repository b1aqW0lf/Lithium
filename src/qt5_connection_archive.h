#ifndef QT5_CONNECTION_ARCHIVE_H
#define QT5_CONNECTION_ARCHIVE_H

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


/***********************************************************************
**This is an archive of Qt5 Signal/Slot commands that have been replaced
** with Qt4 versions.
***********************************************************************/
                 /* Connection List */
                 /*******************/

/*1. connect(ffmpeg, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &MainWindow::encoding_finished);*/

/*2. connect(ffmpeg, &QProcess::readyReadStandardOutput,
        this, &MainWindow::ffmpegReadStandardOutput);*/

/*3. connect(ffprobe, &QProcess::readyReadStandardOutput,
        this, &MainWindow::ffprobeReadStandardOutput);*/

//-----------------------------------------------------------------------------

/*4. connect(ui->ffEncodeButton, &QPushButton::clicked,
        this, &MainWindow::encoding_started);*/

/*5. connect(ui->ffCancelButton, &QPushButton::clicked,
        this, &MainWindow::encoding_cancelled);*/

//-----------------------------------------------------------------------------

/*6. connect(ui->sourceBrowse1, &QPushButton::clicked,
        this, &MainWindow::select_input1);*/

/*7. connect(ui->sourceBrowse2, &QPushButton::clicked,
        this, &MainWindow::select_input2);*/

/*8. connect(ui->saveASButton, &QPushButton::clicked,
        this, &MainWindow::select_output);*/

/*9. connect(ui->videoRFSlider, &QSlider::valueChanged,
        this, &MainWindow::select_crf);*/

/*10. connect(ui->videoRFSlider, &QSlider::valueChanged,
        this, &MainWindow::select_qscale);*/

/*11. connect(ui->videoRFSlider, &QSlider::valueChanged,
        ui->videoRFSpinBox, &QSpinBox::setValue);*/

/*12. connect(ui->videoEncoderDial, &QDial::valueChanged,
        this, &MainWindow::encoder_preset);*/

//-----------------------------------------------------------------------------

/*13. connect(ui->audioCodecBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_aud_codec()));*/

/*14. connect(ui->audioBitrateBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_bitrate()));*/

/*15. connect(ui->audioSampleBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_samplerate()));*/

/*16. connect(ui->audioChannelBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_channels()));*/

/*17. connect(ui->videoCodecBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_vid_codec()));*/

/*18. connect(ui->videoCodecBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(vid_codec_settings()));*/

/*19. connect(ui->videoContainerBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_container()));*/

/*20. connect(ui->videoResBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_vid_res()));*/

/*21. connect(ui->videoAspectRatBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_aspect_rat()));*/

/*22. connect(ui->videoFPSBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_vid_fps()));*/

/*23. connect(ui->videoMotionESTBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(select_motion_est()));*/

/*13. connect(ui->audioCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_aud_codec);*/

/*14. connect(ui->audioBitrateBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_bitrate);*/

/*15. connect(ui->audioSampleBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_samplerate);*/

/*16. connect(ui->audioChannelBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_channels);*/

/*17. connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_vid_codec);*/

/*18. connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::vid_codec_interface);*/

/*19. connect(ui->videoContainerBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_container);*/

/*20. connect(ui->videoResBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_vid_res);*/

/*21. connect(ui->videoAspectRatBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_aspect_rat);*/

/*22. connect(ui->videoFPSBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_vid_fps);*/

/*23. connect(ui->videoMotionESTBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::select_motion_est);*/

//-----------------------------------------------------------------------------

//24. connect(ui->aboutButton, &QPushButton::clicked, qApp, &QApplication::aboutQt);

//-----------------------------------------------------------------------------

/*25. connect(ui->mergeSourcesRadio, &QRadioButton::clicked,
        this, &MainWindow::merge_sources);*/

/*26. connect(ui->extractAudioRadio, &QRadioButton::clicked,
        this, &MainWindow::extract_audio);*/

/*27. connect(ui->normalModeRadio, &QRadioButton::clicked,
        this, &MainWindow::normal_mode);*/

/*28. connect(ui->twoPassEncode, &QRadioButton::clicked,
        this, &MainWindow::two_pass_encode);*/

//-----------------------------------------------------------------------------
//Processing connections in encoding_started()

/*29 disconnect(ui->videoRFSlider, &QSlider::valueChanged,
              this, &MainWindow::select_crf);
connect(ui->videoRFSlider, &QSlider::valueChanged,
        this, &MainWindow::select_qscale);*/

/*30 disconnect(ui->videoRFSlider, &QSlider::valueChanged,
              this, &MainWindow::select_qscale);
connect(ui->videoRFSlider, &QSlider::valueChanged,
        this, &MainWindow::select_crf);*/

/*31 disconnect(ui->videoRFSlider, &QSlider::valueChanged,
           this, &MainWindow::select_crf);
connect(ui->videoRFSlider, &QSlider::valueChanged,
        this, &MainWindow::select_qscale);*/

/*32 disconnect(ui->videoRFSlider, &QSlider::valueChanged,
           this, &MainWindow::select_crf);
connect(ui->videoRFSlider, &QSlider::valueChanged,
        this, &MainWindow::select_qscale);*/

/*33 disconnect(ui->videoRFSlider, &QSlider::valueChanged,
           this, &MainWindow::select_crf);
connect(ui->videoRFSlider, &QSlider::valueChanged,
        this, &MainWindow::select_qscale);*/

/*34 disconnect(ui->videoRFSlider, &QSlider::valueChanged,
           this, &MainWindow::select_qscale);
connect(ui->videoRFSlider, &QSlider::valueChanged,
        this, &MainWindow::select_crf);*/

/*
 *
    actionSave = new QAction(this);
    saveMenu = new QMenu(this);
    actionSave->setToolTip(tr("Save"));
    saveMenu->addAction(actionSave);
    ui->toolBar->addAction(saveMenu->menuAction());
    actionSave->setIcon(QIcon("/home/Ultron/Pictures/pngkit_song-icon-png_900x900.png"));
    saveMenu->setIcon(QIcon("/home/Ultron/Pictures/pngkit_song-icon-png_900x900.png"));
    */

#endif // QT5_CONNECTION_ARCHIVE_H
