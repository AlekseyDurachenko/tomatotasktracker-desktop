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
#ifndef TASKITEM_H
#define TASKITEM_H


#include "taskdata.h"
#include "tasktime.h"
#include <QObject>
class TaskManager;


class TaskItem
{
    friend class TaskManager;
private:
    TaskItem(TaskManager *manager, const TaskData &data, const QList<TaskTime> &times, TaskItem *parent = 0);
    ~TaskItem();

public:
    enum ActiveState {
        Inactive    = 0,    // item is not active
        SubActive   = 1,    // sub item is active
        Active      = 2     // item is active
    };

    inline qint64 id() const;

    inline TaskItem *parent() const;
    inline TaskManager *manager() const;
    int calcIndex() const;

    inline int count() const;
    inline const TaskItem *at(int index) const;
    inline TaskItem *value(int index);
    inline int indexOf(TaskItem *item) const;
    TaskItem *append(const TaskData &data, const QList<TaskTime> &times = QList<TaskTime>());
    void removeAt(int index);
    void removeAll();

    bool canMoveTo(TaskItem *task, int index = -1) const;
    bool moveTo(TaskItem *task, int index = -1);

    inline const TaskData &data() const;
    void setData(const TaskData &data);
    void setData(const TaskData &data, const QList<TaskTime> &times);

    inline const QList<TaskTime> &times() const;
    void setTimes(const QList<TaskTime> &times);
    void appendTime(const TaskTime &time);

    inline ActiveState activeState() const;

    inline qint64 taskTime() const;
    inline qint64 totalTime() const;

private:
    void setActiveStateWithoutEmitDataChanged(ActiveState state);
    void appendTimeWithoutEmitDataChanged(const TaskTime &time);

private:
    qint64 m_id;

    TaskManager *m_manager;

    TaskData m_data;
    QList<TaskTime> m_times;

    ActiveState m_activeState;

    qint64 m_taskTime;
    qint64 m_totalTime;

    TaskItem *m_parent;
    QList<TaskItem *> m_children;
};

qint64 TaskItem::id() const
{
    return m_id;
}

TaskItem *TaskItem::parent() const
{
    return m_parent;
}

TaskManager *TaskItem::manager() const
{
    return m_manager;
}

int TaskItem::count() const
{
    return m_children.count();
}

const TaskItem *TaskItem::at(int index) const
{
    return m_children.at(index);
}

TaskItem *TaskItem::value(int index)
{
    return m_children.value(index);
}

int TaskItem::indexOf(TaskItem *item) const
{
    return m_children.indexOf(item);
}

const TaskData &TaskItem::data() const
{
    return m_data;
}

const QList<TaskTime> &TaskItem::times() const
{
    return m_times;
}

TaskItem::ActiveState TaskItem::activeState() const
{
    return m_activeState;
}

qint64 TaskItem::taskTime() const
{
    return m_taskTime;
}

qint64 TaskItem::totalTime() const
{
    return m_totalTime;
}


/*
class Task
{
public:
    Task(const TaskData &m_data, const QList<TaskTime> &m_times, Task *m_parent = 0);
    ~Task();

    inline Task *parent() const;

    inline qint64 id() const;

    inline const TaskData &data() const;
    void setData(const TaskData &data);

    inline const QList<TaskTime> &times() const;
    void setTimes(const QList<TaskTime> &times);
    void appendTime(const TaskTime &time);

    inline bool isActive() const;
    void setActive(bool active);

    inline qint64 taskTime() const;
    inline qint64 totalTime() const;

private:
    void updateTimeTree();

private:
    // the unique id of the task (generated automatically in constructor)
    qint64 m_id;

    TaskData m_data;
    QList<TaskTime> m_times;
    bool m_active;

    qint64 m_taskTime;
    qint64 m_totalTime;

    Task *m_parent;
    QList<Task *> m_children;
};
*/

#endif // TASKITEM_H
