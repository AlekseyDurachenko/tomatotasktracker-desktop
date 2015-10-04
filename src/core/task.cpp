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
#include "task.h"


Task::Task(const TaskData &data, Task *parent)
{
    m_data = data;
    m_parent = parent;

    calcTaskTime();
    calcTotalSubTasksTime();
    calcTotalTaskTime();
}

Task::~Task()
{
    foreach (Task *task, m_children)
        delete task;
}

void Task::setData(const TaskData &data)
{
    m_data = data;

    calcTaskTime();
    calcTotalTaskTime();

    cascadeParentSubtasksTimeUpdate();
}

void Task::addTime(const TaskTime &taskTime)
{
    m_data.addTime(taskTime);

    calcTaskTime();
    calcTotalTaskTime();

    cascadeParentSubtasksTimeUpdate();
}

Task *Task::addChild(const TaskData &data)
{
    Task *task = new Task(data, this);

    m_children.push_back(task);

    calcTotalSubTasksTime();
    calcTotalTaskTime();

    cascadeParentSubtasksTimeUpdate();

    return task;
}

void Task::removeChild(int index)
{
    delete m_children.takeAt(index);

    calcTotalSubTasksTime();
    calcTotalTaskTime();

    cascadeParentSubtasksTimeUpdate();
}

void Task::removeAllChildren()
{
    foreach (Task *task, m_children)
        delete task;
    m_children.clear();

    calcTotalSubTasksTime();
    calcTotalTaskTime();

    cascadeParentSubtasksTimeUpdate();
}

void Task::calcTotalTaskTime()
{
    m_totalTaskTime = m_taskTime + m_totalSubtasksTime;
}

void Task::calcTotalSubTasksTime()
{
    m_totalSubtasksTime = 0;
    foreach (const Task *task, m_children)
        m_totalSubtasksTime += task->totalTaskTime();
}

void Task::calcTaskTime()
{
    m_taskTime = 0;
    foreach (const TaskTime &taskTime, m_data.times())
        m_taskTime += taskTime.total();
}

void Task::cascadeParentSubtasksTimeUpdate()
{
    Task *parent = m_parent;
    while (parent && parent->parent()) {
        parent->calcTotalSubTasksTime();
        parent->calcTotalTaskTime();

        parent = parent->parent();
    }
}

QVariant Task::variantFromPtr(Task *task)
{
    return QVariant::fromValue<void *>(static_cast<void *>(task));
}

Task *Task::variantToPtr(const QVariant &task)
{
    return static_cast<Task *>(task.value<void *>());
}
