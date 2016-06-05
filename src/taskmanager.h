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
#ifndef TASKMANAGER_H
#define TASKMANAGER_H


#include "taskitem.h"
#include <QObject>
#include <QHash>
class Tomato;


class TaskManager : public QObject
{
    Q_OBJECT
    friend class TaskItem;
public:
    enum State {
        Idle        = 0,
        Working     = 1,
        Resting     = 2,
    };

public:
    explicit TaskManager(QObject *parent = 0);
    virtual ~TaskManager();

    inline TaskItem *rootItem() const;
    inline TaskItem *findById(qint64 id);

    inline TaskItem *activeItem() const;
    void setActiveItem(TaskItem *item);

    inline State state() const;
    qint64 calcLastStateTime() const;
    TaskTime calcLastStateTaskTime() const;
    qint64 calcTaskTime(TaskItem *task) const;

public slots:
    // this slot emited the datachanged signal for all active items
    // if state is working. will be connected to the global sync timer
    void sync();

signals:
    void stateChanged(TaskManager::State state);
    void activeItemChanged();

    void aboutToBeTaskInserted(TaskItem *parent, int first, int last);
    void taskInserted(TaskItem *parent, int first, int last);

    void aboutToBeTaskRemoved(TaskItem *parent, int first, int last);
    void taskRemoved();

    void aboutToBeTaskMoved(TaskItem *sourceParent, int sourceFirst, int sourceLast,
                            TaskItem *destinationParent, int destinationChild);
    void taskMoved();

    void aboutToBeTaskDataChanged(TaskItem *parent, int first, int last,
                                  bool activeStateChanged,
                                  bool dataChanged,
                                  bool timeChanged,
                                  bool displayTimeChanged);
    void taskDataChanged(TaskItem *parent, int first, int last,
                         bool activeStateChanged,
                         bool dataChanged,
                         bool timeChanged,
                         bool displayTimeChanged);

public slots:
    void startWorking();
    void startResting();
    void startIdle();

private:
    void beginInsertTasks(TaskItem *parent, int first, int last);
    void endInsertTasks(TaskItem *parent, int first, int last);

    void beginRemoveTasks(TaskItem *parent, int first, int last);
    void endRemoveTasks();

    void beginMoveTasks(TaskItem *sourceParent, int sourceFirst, int sourceLast,
                        TaskItem *destinationParent, int destinationChild);
    void endMoveTasks();

    void beginDataChanged(TaskItem *parent, int first, int last,
                          bool activeStateChanged,
                          bool dataChanged,
                          bool timeChanged,
                          bool displayTimeChanged);
    void endDataChanged(TaskItem *parent, int first, int last,
                        bool activeStateChanged,
                        bool dataChanged,
                        bool timeChanged,
                        bool displayTimeChanged);

private:
    TaskItem *m_rootItem;
    TaskItem *m_activeItem;
    QHash<qint64, TaskItem *> m_items;
    qint64 m_activeItemStartTime;
    qint64 m_lastStateStartTime;
    State m_state;
};

TaskItem *TaskManager::rootItem() const
{
    return m_rootItem;
}

TaskItem *TaskManager::findById(qint64 id)
{
    return m_items.value(id, 0);
}

TaskItem *TaskManager::activeItem() const
{
    return m_activeItem;
}

TaskManager::State TaskManager::state() const
{
    return m_state;
}


#endif // TASKMANAGER_H
