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


#include "lithium_theme.h"

#include <QApplication>

LithiumTheme::LithiumTheme(QWidget *parent)
    : QWidget{parent}
{
}

LithiumTheme::~LithiumTheme()
{
    //empty destructor
}

void LithiumTheme::set_dark_mode_theme()
{
    this->get_dark_mode_theme();
}

void LithiumTheme::get_dark_mode_theme()
{
    //Create a dark palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(50, 50, 50));//
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    //darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white/*QColor(178,178,179)//off-white*/);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(112, 168, 224));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);/*
    progress.setStyleSheet("QProgressBar { text-align: center }"
                            "QProgressBar::chunk { background-color: #70A8E0; width: 10px; margin: 0.75px; }");*/
    qApp->setStyleSheet("QSpinBox { background-color: transparent; } QSpinBox QLineEdit { background-color: transparent; }");
    progress.set_progressbar_theme();
    /*qApp->setStyleSheet("QProgressBar { text-align: center }"
                        "QProgressBar::chunk { background-color: #70A8E0; width: 10px; margin: 0.75px; }");*/
    /*progress.setStyleSheet("QProgressBar { background-color: #70A8E0 }");*///RGB 	112	168	224
    //app.setStyleSheet("QProgressBar::chunk { background-color: #70A8E0; width: 10px; margin: 0.75px; }");
}
