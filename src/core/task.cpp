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



//#include "tomato.h"


//Task::Task(Tomato *taskManager, Task *parent)
//{
//    init(taskManager, TaskData(), parent);
//}

//Task::Task(Tomato *taskManager, const TaskData &data, Task *parent)
//{
//    init(taskManager, data, parent);
//}

//Task::~Task()
//{
//    qDeleteAll(m_children);
//}

//void Task::init(Tomato *taskManager, const TaskData &data, Task *parent)
//{
//    m_taskManager = taskManager;
//    m_parent = parent;

//    m_data = data;

//    calcTaskTime();
//    calcTotalSubTasksTime();
//    calcTotalTaskTime();
//}

//void Task::setData(const TaskData &data)
//{
//    const int index = this->index();
//    emit m_taskManager->aboutToBeTaskDataChanged(m_parent, index, index);
//    m_data = data;
//    calcTaskTime();
//    calcTotalTaskTime();
//    emit m_taskManager->taskDataChanged(m_parent, index, index);

//    if (m_parent)
//        m_parent->cascadeUpdateTotalTaskTime();
//}

//void Task::addTime(const TaskTime &taskTime)
//{
//    const int index = this->index();
//    emit m_taskManager->aboutToBeTaskDataChanged(m_parent, index, index);
//    m_data.addTime(taskTime);
//    calcTaskTime();
//    calcTotalTaskTime();
//    emit m_taskManager->taskDataChanged(m_parent, index, index);

//    if (m_parent)
//        m_parent->cascadeUpdateTotalTaskTime();
//}

//bool Task::isActiveTask() const
//{
//    if (m_taskManager->activeTask() == this)
//        return true;

//    foreach (const Task *task, m_children)
//        if (task->isActiveTask())
//            return true;

//    return false;
//}

//Task *Task::addChild(const TaskData &data)
//{
//    Task *task = new Task(m_taskManager, data, this);

//    const int index = childCount();
//    emit m_taskManager->aboutToBeTaskInserted(this, index, index);
//    m_children.push_back(task);
//    calcTotalSubTasksTime();
//    calcTotalTaskTime();
//    emit m_taskManager->taskInserted(this, index, index);

//    if (m_parent)
//        m_parent->cascadeUpdateTotalTaskTime();

//    return task;
//}

//void Task::removeChild(int index)
//{
//    if (m_children.at(index)->isActiveTask()) {
//        m_taskManager->startIdle();
//        m_taskManager->setActiveTask(0);
//    }

//    emit m_taskManager->aboutToBeTaskRemoved(this, index, index);
//    delete m_children.takeAt(index);
//    calcTotalSubTasksTime();
//    calcTotalTaskTime();
//    emit m_taskManager->taskRemoved(this, index, index);

//    if (m_parent)
//        m_parent->cascadeUpdateTotalTaskTime();
//}

//void Task::removeAllChildren()
//{
//    m_taskManager->startIdle();
//    m_taskManager->setActiveTask(0);

//    emit m_taskManager->aboutToBeTaskRemoved(this, 0, m_children.count() - 1);
//    qDeleteAll(m_children);
//    m_children.clear();
//    calcTotalSubTasksTime();
//    calcTotalTaskTime();
//    emit m_taskManager->taskRemoved(this, 0, m_children.count() - 1);

//    if (m_parent)
//        m_parent->cascadeUpdateTotalTaskTime();
//}

//QVariant Task::variantFromPtr(Task *task)
//{
//    return QVariant::fromValue<void *>(static_cast<void *>(task));
//}

//Task *Task::variantToPtr(const QVariant &task)
//{
//    return static_cast<Task *>(task.value<void *>());
//}

//void Task::calcTotalTaskTime()
//{
//    m_totalTaskTime = m_taskTime + m_totalSubtasksTime;
//}

//void Task::calcTotalSubTasksTime()
//{
//    m_totalSubtasksTime = 0;
//    foreach (const Task *task, m_children)
//        m_totalSubtasksTime += task->totalTaskTime();
//}

//void Task::calcTaskTime()
//{
//    m_taskTime = 0;
//    foreach (const TaskTime &taskTime, m_data.times())
//        m_taskTime += taskTime.total();
//}

//void Task::cascadeUpdateTotalTaskTime()
//{
//    int index = 0;

//    if (m_parent) {
//        index = this->index();
//        emit m_taskManager->aboutToBeTaskDataChanged(m_parent, index, index);
//    }

//    calcTotalSubTasksTime();
//    calcTotalTaskTime();

//    if (m_parent) {
//        emit m_taskManager->taskDataChanged(m_parent, index, index);
//        m_parent->cascadeUpdateTotalTaskTime();
//    }
//}
