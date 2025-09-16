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


#include "storage_data.h"
#include "src/ui_storage_data.h"

#include <QStorageInfo>


StorageData::StorageData(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StorageData)
{
    ui->setupUi(this);

    QStorageInfo storage = QStorageInfo::root();
    qint64 bytes = storage.bytesAvailable();
#ifdef Q_OS_WINDOWS
    QString size_available = QLocale().formattedDataSize(bytes, 2, QLocale::DataSizeTraditionalFormat);
    ui->harddriveLabel1->setPixmap(tr("C:/users/Kraken/Documents/Source_Code/Projects/Yttrium/resources/hd_16px.png"));
#else
    QString size_available = QLocale().formattedDataSize(bytes, 2, QLocale::DataSizeSIFormat);
    ui->harddriveLabel1->setPixmap(tr("/home/Ultron/Documents/Source_Code/Projects/Yttrium/resources/hd_16px.png"));
#endif
    ui->harddriveLabel1->setToolTip("Available storage on main disk");
    ui->harddriveLabel2->setText(size_available);
}

StorageData::~StorageData()
{
    delete ui;
}
