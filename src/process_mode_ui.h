#ifndef PROCESS_MODE_UI_H
#define PROCESS_MODE_UI_H

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


#include <QButtonGroup>
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

Q_SIGNALS:
    void current_process_mode_state(const bool &normal, const bool &merge, const bool &extract);
    void current_process_mode_status(const QString &status, const int &timeout);
    void source_2_field_setenabled(const bool &enabled);

public Q_SLOTS:
    void current_process_mode();

private:
    Ui::ProcessModeUI *ui;

private Q_SLOTS:
    void merge_sources_mode();
    void extract_audio_mode();
    void normal_processing_mode();

private:
    //processing button group
    QButtonGroup *processButtons;

    //mode variables
    bool normal_mode{};
    bool merge_sources{};
    bool extract_audio{};
};

#endif // PROCESS_MODE_UI_H
