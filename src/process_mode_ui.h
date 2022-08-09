#ifndef PROCESS_MODE_UI_H
#define PROCESS_MODE_UI_H

#include "ui_process_mode_ui.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ProcessModeUI;
}
QT_END_NAMESPACE

class ProcessModeUI : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessModeUI(QWidget *parent = nullptr);
    ~ProcessModeUI();

private:
    Ui::ProcessModeUI *ui;

private Q_SLOTS:
    /*void merge_sources();
    void extract_audio();
    void normal_mode();*/

private:
    //processing button group
    QButtonGroup *processButtons;
};

#endif // PROCESS_MODE_UI_H
