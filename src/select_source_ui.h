#ifndef SELECT_SOURCE_UI_H
#define SELECT_SOURCE_UI_H

#include "ui_select_source_ui.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class SelectSourceUI;
}
QT_END_NAMESPACE

class SelectSourceUI : public QWidget
{
    Q_OBJECT

public:
    explicit SelectSourceUI(QWidget *parent = nullptr);
    ~SelectSourceUI();

//private:
public:
    Ui::SelectSourceUI *ui;

private Q_SLOTS:
    void select_input1();
    void select_input2();

private:
    QString input_file1{};
    QString input_file2{};

};

#endif // SELECT_SOURCE_UI_H
