// Copyright 2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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


#include "taskmanager.h"
#include <QAbstractItemModel>


class TaskItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TaskItemModel(TaskManager *taskManager, QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QStringList mimeTypes() const;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

private slots:
    void taskManager_aboutToBeTaskInserted(TaskItem *parent, int first, int last);
    void taskManager_taskInserted(TaskItem *parent, int first, int last);

    void taskManager_aboutToBeTaskRemoved(TaskItem *parent, int first, int last);
    void taskManager_taskRemoved();

    void taskManager_aboutToBeTaskMoved(TaskItem *sourceParent, int sourceFirst, int sourceLast,
                                        TaskItem *destinationParent, int destinationChild);
    void taskManager_taskMoved();

    void taskManager_taskDataChanged(TaskItem *parent, int first, int last,
                                     bool activeStateChanged,
                                     bool dataChanged,
                                     bool timeChanged,
                                     bool displayTimeChanged);

    void taskManager_stateChanged(TaskManager::State state);

private:
    TaskManager *m_taskManager;
};


#endif // TASKITEMMODEL_H
