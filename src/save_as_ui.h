#ifndef SAVE_AS_UI_H
#define SAVE_AS_UI_H

#include "ui_save_as_ui.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class SaveAsUI;
}
QT_END_NAMESPACE

class SaveAsUI : public QWidget
{
    Q_OBJECT

public:
    explicit SaveAsUI(QWidget *parent = nullptr);
    ~SaveAsUI();

//private:
public:
    Ui::SaveAsUI *ui;

private Q_SLOTS:
    void select_output();

Q_SIGNALS:
    QString saveas_value();

//private:
public:
    QString output_file{};
};

#endif // SAVE_AS_UI_H
