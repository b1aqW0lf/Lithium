#ifndef INPUT_TREE_MODEL_H
#define INPUT_TREE_MODEL_H


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


#include <QList>
#include <QObject>
#include <QStandardItem>
#include <QTreeView>


class InputTreeModel : public QObject
{
    Q_OBJECT
    friend class InputTreeView;

public:
    explicit InputTreeModel(QObject *parent = nullptr);
    ~InputTreeModel();

private:
    QStandardItemModel *tree_model;
    QList<QStandardItem *> model_item_list; //storage - stores the model items
    QList<QStandardItem *> child_item_list; //storage - stores the child items

    //functions
    void generate_model_items(const QStringList &headers);
    void generate_child_items(const int &num);
    void set_model_items(const QStringList &headers);
    void set_model_child_items(const int &num);
    void set_header_labels(const QStringList &headers);
    void set_column_count(const QStringList &headers);
    void set_treeview_model(QTreeView *treeview);
    void clear_item_list(QList<QStandardItem *> list);
};

#endif // INPUT_TREE_MODEL_H
