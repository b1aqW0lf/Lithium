#ifndef SAVE_AS_FIELD_H
#define SAVE_AS_FIELD_H

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


#include "process_mode_flags.h"
#include "save_as_extension_list.h"

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
class SaveAsField;
}
QT_END_NAMESPACE

class SaveAsField : public QWidget
{
    Q_OBJECT

public:
    explicit SaveAsField(QWidget *parent = nullptr);
    ~SaveAsField();

Q_SIGNALS:
    void send_output_file_path(const QString &output_path);
    void send_save_field_statusbar_message(const QString &message, const int &timeout);

public Q_SLOTS:
    void receive_input_file_name(const QString &file_name);
    void current_process_mode(ProcessMode process_mode);
    void send_selected_output_path();

private Q_SLOTS:
    void select_save_destination();
    void select_output_file_container(const int &index);

private:
    Ui::SaveAsField *ui;
    SaveAsContainerList extensions;

    //functions
    void setup_clear_button();
    void set_default_process_mode();
    void set_default_extensions();
    void setup_ui_tooltips();
    void enable_save_file_extension_list();
    void process_input_file_name(const QString &file_name);
    void get_input_file_extension(const QString &file_name);
    void set_output_file_extension(const QString &output_ext);

    //variables
    ProcessMode process_mode;
    QString input_file_name{};
    QString input_file_ext{};
};

#endif // SAVE_AS_FIELD_H
