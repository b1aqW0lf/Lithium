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


#include <QStatusBar>
#include <QLabel>

#include "statusbar_ui.h"
#include "ui_statusbar_ui.h"

StatusBarUI::StatusBarUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBarUI)
{
    ui->setupUi(this);

    ui->durationTimeLabel->setText("Progress:");
    //ui->progressBarLabel->hide();
    ui->statProgressBar->setFixedHeight(22);
    ui->statProgressBar->setFixedWidth(150);
    ui->statProgressBar->setMinimum(0);
    ui->statProgressBar->setMaximum(100);
    ui->statProgressBar->setValue(0);
}

StatusBarUI::~StatusBarUI()
{
    delete ui;
}
