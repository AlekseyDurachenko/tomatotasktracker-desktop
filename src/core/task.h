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
    explicit Task(const TaskData &data, Task *parent = 0);
    ~Task();

public:
    inline int id() const;

    inline const TaskData &data() const;

    inline qint64 totalTime() const;
    inline qint64 subtasksTime() const;
    inline qint64 taskTime() const;

    inline Task *parent() const;
    inline int index() const;
    inline const QList<Task *> &children() const;

private:
    void calcTotalTime();
    void calcSubtasksTime();
    void calcTaskTime();

private:
    int m_id;

    TaskData m_data;

    qint64 m_totalTime;
    qint64 m_subtasksTime;
    qint64 m_taskTime;

    Task *m_parent;
    QList<Task *> m_children;
};

int Task::id() const
{
    return m_id;
}

const TaskData &Task::data() const
{
    return m_data;
}

qint64 Task::totalTime() const
{
    return m_totalTime;
}

qint64 Task::subtasksTime() const
{
    return m_subtasksTime;
}

qint64 Task::taskTime() const
{
    return m_taskTime;
}

Task *Task::parent() const
{
    return m_parent;
}

int Task::index() const
{
    return m_parent->children().indexOf(const_cast<Task *>(this));
}

const QList<Task *> &Task::children() const
{
    return m_children;
}


#endif // TASK_H
