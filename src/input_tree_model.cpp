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

#include "input_tree_model.h"

#include <QStandardItemModel>


InputTreeModel::InputTreeModel(QObject *parent)
    : QObject{parent}
{
    //initialize the tree model
    tree_model = new QStandardItemModel();
}

InputTreeModel::~InputTreeModel()
{
    delete tree_model;
}

void InputTreeModel::generate_model_items(const QStringList &headers)
{
    for(int i = 0; i < headers.size(); ++i)
    {
        QStandardItem *model_item = new QStandardItem();
        this->model_item_list << model_item;
    }
}

void InputTreeModel::generate_child_items(const int &num)
{
    for(int i = 0; i < num; ++i)
    {
        QStandardItem *child_item = new QStandardItem();
        this->child_item_list << child_item;
    }
}

void InputTreeModel::set_model_items(const QStringList &headers)
{
    //sets the standard items models to the model
    int row{0};
    int col{0};
    for(int i = 0; i < headers.size(); ++i)
    {
        this->tree_model->setItem(row, col, this->model_item_list[i]);
        ++col;
    }
}

void InputTreeModel::set_model_child_items(const int &num)
{
    //sets num child items to item model at position index
    int row{0};
    int col{0};
    int index{0};
    for(int i = 0; i < num; ++i)
    {
        this->model_item_list[index]->setChild(row, col, this->child_item_list[i]);
        ++col;
    }
}

void InputTreeModel::set_header_labels(const QStringList &headers)
{
    //set the horizontal headers for the model
    this->tree_model->setHorizontalHeaderLabels(headers);
}

void InputTreeModel::set_column_count(const QStringList &headers)
{
    //set the column count for the model
    this->tree_model->setColumnCount(headers.size());
}

void InputTreeModel::set_treeview_model(QTreeView *treeview)
{
    //set the model to the treeview
    treeview->setModel(this->tree_model);
}

void InputTreeModel::clear_item_list(QList<QStandardItem *> list)
{
    //clear the standard item list
    list.clear();
}
