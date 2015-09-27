// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef TASKITEMMODEL_H
#define TASKITEMMODEL_H


#include "tomato.h"
#include "task.h"
#include <QAbstractItemModel>


class TaskItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TaskItemModel(Tomato *tomato, QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
private slots:
    void tomato_aboutToBeTaskInserted(Task *parent, int first, int last);
    void tomato_taskInserted(Task *parent, int first, int last);
    void tomato_aboutToBeTaskRemoved(Task *parent, int first, int last);
    void tomato_taskRemoved(Task *parent, int first, int last);
    void tomato_aboutToBeTaskDataChanged(Task *parent, int first, int last);
    void tomato_taskDataChanged(Task *parent, int first, int last);
    void tomato_aboutToBeReseted();
    void tomato_reseted();
private:
    Tomato *m_tomato;
};


#endif // TASKITEMMODEL_H
