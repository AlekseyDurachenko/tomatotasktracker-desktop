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
#include "tomato.h"
#include "consts.h"
#include <QDateTime>
#include <QDebug>


static QList<Task *> findTaskRecursive(Task *root)
{
    QList<Task *> result = QList<Task *>() << root;
    for (int i = 0; i < root->childCount(); ++i)
        result << findTaskRecursive(root->child(i));

    return result;
}


Tomato::Tomato(QObject *parent) : QObject(parent)
{
    m_rootTask = new Task(TaskData(), 0);
    m_taskHash.insert(m_rootTask->id(), m_rootTask);
    m_activeTask = 0;

    m_state = Idle;

    m_tomatoStartTime = 0;

    m_workingTime = DefaultWorkingTime;
    m_restingTime =  DefaultRestingTime;

    // configure tomato timer
    m_tomatoTimer.setSingleShot(true);
    connect(&m_tomatoTimer, SIGNAL(timeout()),
            this, SLOT(tomatoStartTimer_timeout()));
}

void Tomato::setWorkingTime(qint64 workingTime)
{
    m_workingTime = workingTime;
    emit workingTimeChanged();

    if (m_state != Working && m_state != OverWorking)
        return;

    if (calcTomatoTime() >= m_workingTime) {
        setState(OverWorking);
    } else {
        setState(Working);
    }
}

void Tomato::setRestingTime(qint64 restingTime)
{
    m_restingTime = restingTime;
    emit restingTimeChanged();

    if (m_state != Resting && m_state != OverResting)
        return;

    if (calcTomatoTime() >= m_restingTime) {
        setState(OverResting);
    } else {
        setState(Resting);
    }
}

QString Tomato::stateText() const
{
    switch (m_state) {
    case Idle:
        return tr("IDLE");
    case Working:
        return tr("WORKING");
    case OverWorking:
        return tr("OVERWORKING");
    case Resting:
        return tr("RESTING");
    case OverResting:
        return tr("OVERRESTING");
    }

    return QString();
}

qint64 Tomato::calcTomatoTime() const
{
    if (m_state == Idle)
        return 0;

    return timestamp() - m_tomatoStartTime;
}

qint64 Tomato::calcTaskTime(const Task *task) const
{
    qint64 total = task->totalTaskTime();
    if ((m_state == Working || m_state == OverWorking) && isActiveTask(task))
        total += timestamp() - m_activeTaskStartTime;

    return total;
}

void Tomato::setActiveTask(Task *task)
{
    if (activeTask() == task)
        return;

    saveLastTaskTime();

    if (activeTask()) {
        const int index = activeTask()->index();
        emit aboutToBeTaskDisplayChanged(activeTask()->parent(), index, index);
        emit taskDisplayChanged(activeTask()->parent(), index, index);
    }

    if (task) {
        const int index = task->index();
        emit aboutToBeTaskDisplayChanged(task->parent(), index, index);
        emit taskDisplayChanged(task->parent(), index, index);
    }

    m_activeTask = task;
    m_activeTaskStartTime = timestamp();

    emit activeTaskChanged();
}

bool Tomato::isActiveTask(const Task *task) const
{
    if (task == activeTask())
        return true;

    for (int i = 0; i < task->childCount(); ++i)
        if (isActiveTask(task->child(i)))
            return true;

    return false;
}

TaskTime Tomato::calcActiveTaskTime() const
{
    return TaskTime(m_activeTaskStartTime, timestamp());
}

void Tomato::setTaskData(Task *task, const TaskData &data)
{
    cascadeEmitAboutToBeTaskDataChanged(task);

    task->setData(data);

    cascadeEmitTaskDataChanged(task);
}

void Tomato::addTaskTime(Task *task, const TaskTime &taskTime)
{
    cascadeEmitAboutToBeTaskDataChanged(task);

    task->addTime(taskTime);

    cascadeEmitTaskDataChanged(task);
}

Task *Tomato::addChildTask(Task *parent, const TaskData &data)
{
    int index = parent->childCount();

    cascadeEmitAboutToBeTaskDataChanged(parent);
    emit aboutToBeTaskInserted(parent, index, index);

    Task *task = parent->addChild(data);
    m_taskHash.insert(task->id(), task);

    emit taskInserted(parent, index, index);
    cascadeEmitTaskDataChanged(parent);

    return task;
}

void Tomato::removeChildTask(Task *parent, int index)
{
    if (isActiveTask(parent->child(index))) {
        startIdle();
        setActiveTask(0);
    }

    cascadeEmitAboutToBeTaskDataChanged(parent);
    emit aboutToBeTaskRemoved(parent, index, index);

    foreach (Task *task, findTaskRecursive(parent->child(index)))
        m_taskHash.remove(task->id());
    parent->removeChild(index);

    emit taskRemoved(parent, index, index);
    cascadeEmitTaskDataChanged(parent);
}

void Tomato::removeTasks(const QList<Task *> tasks)
{
    QList<Task *> uniqueTasks = tasks;

    struct pfunc {
        static bool taskHasParent(Task *task, const QList<Task *> &list)
        {
            while (task && task->parent()) {
                if (list.contains(task->parent()))
                    return true;

                task = task->parent();
            }

            return false;
        }
    };

    while (1) {
        bool itemWasRemoved = false;

        for (int i = 0; i < uniqueTasks.count(); ++i) {
            if (pfunc::taskHasParent(uniqueTasks[i], uniqueTasks)) {
                uniqueTasks.removeAt(i);
                itemWasRemoved = true;
                break;
            }
        }

        if (!itemWasRemoved)
            break;
    }

    if (uniqueTasks.contains(activeTask()))
        setActiveTask(0);

    for (int i = 0; i < uniqueTasks.count(); ++i)
        removeChildTask(uniqueTasks[i]->parent(), uniqueTasks[i]->index());
}

void Tomato::removeAllTasks()
{
    if (activeTask()) {
        startIdle();
        setActiveTask(0);
    }

    int first = 0;
    int last = rootTask()->childCount() - 1;

    cascadeEmitAboutToBeTaskDataChanged(rootTask());
    emit aboutToBeTaskRemoved(rootTask(), first, last);

    foreach (Task *task, findTaskRecursive(rootTask()))
        m_taskHash.remove(task->id());

    rootTask()->removeAllChildren();

    emit taskRemoved(rootTask(), first, last);
    cascadeEmitTaskDataChanged(rootTask());
}

void Tomato::moveTask(Task *task, Task *destParent, int destIndex)
{
    if (!taskCanBeMoved(task, destParent))
        return;

    int index = task->index();
    emit aboutToBeTaskMoved(task->parent(), index, index, destParent, destIndex);

    Task *srcParent = task->parent();
    srcParent->m_children.removeAt(index);
    destParent->m_children.insert(destIndex, task);
    task->m_parent = destParent;

    srcParent->calcTaskTime();
    srcParent->calcTotalSubTasksTime();
    srcParent->calcTotalTaskTime();
    srcParent->cascadeParentSubtasksTimeUpdate();
    destParent->calcTaskTime();
    destParent->calcTotalSubTasksTime();
    destParent->calcTotalTaskTime();
    destParent->cascadeParentSubtasksTimeUpdate();

    emit taskMoved(srcParent, index, index, destParent, destIndex);
}

bool Tomato::taskCanBeMoved(Task *task, Task *destParent)
{
    if (!task->parent())
        return false;

    if (findTaskRecursive(task).contains(destParent))
        return false;

    return true;
}

Task *Tomato::findTask(int id) const
{
    return m_taskHash.value(id, 0);
}

void Tomato::reset()
{
    emit aboutToBeReseted();

    m_rootTask->removeAllChildren();
    m_activeTask = 0;
    m_state = Idle;
    m_activeTaskStartTime = 0;
    m_workingTime = DefaultWorkingTime;
    m_restingTime = DefaultRestingTime;
    m_tomatoTimer.stop();

    emit reseted();
}

void Tomato::startWorking()
{
    saveLastTaskTime();

    if (!activeTask())
        return;

    m_activeTaskStartTime = timestamp();
    m_tomatoStartTime = timestamp();
    m_tomatoTimer.start(m_workingTime * 1000);
    setState(Working);
}

void Tomato::startResting()
{
    saveLastTaskTime();

    if (!activeTask())
        return;

    m_tomatoStartTime = timestamp();
    m_tomatoTimer.start(m_restingTime * 1000);
    setState(Resting);
}

void Tomato::startIdle()
{
    saveLastTaskTime();

    if (!activeTask())
        return;

    m_tomatoTimer.stop();
    setState(Idle);
}

void Tomato::updateActiveTaskDisplay()
{
    if (activeTask())
        updateTask(activeTask());
}

void Tomato::tomatoStartTimer_timeout()
{
    if (m_state == Working)
        setState(OverWorking);
    else if (m_state == Resting)
        setState(OverResting);
}

qint64 Tomato::timestamp() const
{
    return QDateTime::currentDateTime().toTime_t();
}

void Tomato::updateTask(Task *task)
{
    while (task && task->parent()) {
        const int index = task->index();
        emit aboutToBeTaskDisplayChanged(task->parent(), index, index);
        emit taskDisplayChanged(task->parent(), index, index);

        task = task->parent();
    }
}

void Tomato::setState(Tomato::State state)
{
    if (m_state == state)
        return;

    m_state = state;
    if (activeTask())
        updateTask(activeTask());

    emit stateChanged(m_state);
}

void Tomato::saveLastTaskTime()
{
    if (activeTask() == 0)
        return;

    if (m_state != Working && m_state != OverWorking)
        return;

    const TaskTime taskTime = TaskTime(m_activeTaskStartTime, timestamp());
    if (!taskTime.isEmpty())
        addTaskTime(activeTask(), taskTime);
}

void Tomato::cascadeEmitAboutToBeTaskDataChanged(Task *task)
{
    while (task && task->parent()) {
        int index = task->index();
        emit aboutToBeTaskDataChanged(task->parent(), index, index);
        task = task->parent();
    }
}

void Tomato::cascadeEmitTaskDataChanged(Task *task)
{
    while (task && task->parent()) {
        int index = task->index();
        emit taskDataChanged(task->parent(), index, index);
        task = task->parent();
    }
}
