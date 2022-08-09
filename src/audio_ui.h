#ifndef AUDIO_UI_H
#define AUDIO_UI_H

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
