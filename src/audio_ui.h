#ifndef AUDIO_UI_H
#define AUDIO_UI_H

/******************************************************************************
 Copyright (c) 2020-2022 b1aqW0lf

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/


#include "ui_audio_ui.h"

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class AudioUI;
}
QT_END_NAMESPACE

class AudioUI : public QWidget
{
    Q_OBJECT

public:
    explicit AudioUI(QWidget *parent = nullptr);
    ~AudioUI();

private:
    Ui::AudioUI *ui;

private Q_SLOTS:
    void select_aud_bitrate();
    void select_aud_codec();
    void select_samplerate();
    void select_channels();

private:
    QString audio_br_value{};
    QString audio_codec{};
    QString audio_sr_value{};
    QString audio_ac_value{};
    //-----------------------------------
    //Interface String Lists
    QStringList audioCodecList{};
    QStringList audioBitrateList{};
    QStringList audioSampleList{};
    QStringList audioChannelList{};

};

#endif // AUDIO_UI_H
