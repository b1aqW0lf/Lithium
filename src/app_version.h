#ifndef APP_VERSION_H
#define APP_VERSION_H

/******************************************************************************
 Copyright (c) 2020-2022 b1aqW0lf

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/


#include "misc/version.h"
#include <QDate>

#define VERSION_STRING "0.8.9"
#define VERSION_INT 000809


extern const char *kGitHash;

QDateTime date{QDateTime::currentDateTime()};
QString compiled_date{date.toString("yyyy.MM.dd")};
QByteArray formattedTimeMsg{compiled_date.toLocal8Bit()};

QString git_hash_str{GIT_COMMIT_HASH};

#endif // APP_VERSION_H
