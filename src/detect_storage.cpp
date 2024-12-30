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


#include "detect_storage.h"

#include<QLocale>


DetectStorage::DetectStorage(QObject *parent) : QObject(parent)
{
    //empty constructor
}

DetectStorage::~DetectStorage(){}

QString DetectStorage::get_available_storage_size()
{
    //the get_available_storage_size function is based on the smartSize function
    //from the Torrent File Editor project with some modifications for this project

    storage = QStorageInfo::root();
    storage.refresh();
    double bytes = storage.bytesAvailable();
    int i = 0;

    while (bytes >= 1000.0) {
        bytes /= 1000.0;
        ++i;
    }

    QString res = QLocale::system().toString(bytes, 'g', 4);

    // Drop zeroes
    while (res.contains(QLocale::system().decimalPoint()) && res.right(1) == QLatin1String("0"))
        res.chop(1);

    if (res.right(1)[0] == QLocale::system().decimalPoint())
        res.chop(1);

    switch (i) {
    case 0:
        res += QStringLiteral(" ") + tr("B");
        break;

    case 1:
        res += QStringLiteral(" ") + tr("KB");
        break;

    case 2:
        res += QStringLiteral(" ") + tr("MB");
        break;

    case 3:
        res += QStringLiteral(" ") + tr("GB");
        break;

    case 4:
        res += QStringLiteral(" ") + tr("TB");
        break;
    }

    return res;
}
