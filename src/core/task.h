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
#ifndef TASK_H
#define TASK_H


#include "taskdata.h"
#include <QVariant>
class Tomato;


class Task
{
    friend class Tomato;
private:
    Task(const TaskData &data, Task *parent = 0);
    ~Task();
public:

    inline Task *parent() const;

    inline const TaskData &data() const;

    inline qint64 totalTaskTime() const;
    inline qint64 totalSubtasksTime() const;
    inline qint64 taskTime() const;

    inline int index() const;
    inline int indexOfChild(const Task *task) const;

    inline int childCount() const;
    inline Task *child(int index) const;

private:
    void setData(const TaskData &data);
    void addTime(const TaskTime &taskTime);

    Task *addChild(const TaskData &data);
    void removeChild(int index);
    void removeAllChildren();

    void calcTotalTaskTime();
    void calcTotalSubTasksTime();
    void calcTaskTime();

    void cascadeParentSubtasksTimeUpdate();

public:
    static QVariant variantFromPtr(Task *task);
    static Task *variantToPtr(const QVariant &task);

private:
    Task *m_parent;

    TaskData m_data;
    QList<Task *> m_children;

    qint64 m_totalTaskTime;
    qint64 m_totalSubtasksTime;
    qint64 m_taskTime;
};

Task *Task::parent() const
{
    return m_parent;
}

const TaskData &Task::data() const
{
    return m_data;
}

qint64 Task::totalTaskTime() const
{
    return m_totalTaskTime;
}

qint64 Task::totalSubtasksTime() const
{
    return m_totalSubtasksTime;
}

qint64 Task::taskTime() const
{
    return m_taskTime;
}

int Task::index() const
{
    return m_parent->indexOfChild(this);
}

int Task::childCount() const
{
    return m_children.count();
}

Task *Task::child(int index) const
{
    return m_children.at(index);
}

int Task::indexOfChild(const Task *task) const
{
    return m_children.indexOf(const_cast<Task *>(task));
}


#endif // TASK_H
