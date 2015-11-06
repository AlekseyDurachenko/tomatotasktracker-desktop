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
#include <QStack>


static QList<Task *> taskTreeToList(Task *root)
{
    QList<Task *> result = QList<Task *>() << root;
    for (int i = 0; i < root->children().count(); ++i)
        result << taskTreeToList(root->children().at(i));

    return result;
}

static bool isTaskTreeActive(const Task *task, const Task *activeTask)
{
    if (task == activeTask)
        return true;

    for (int i = 0; i < task->children().count(); ++i)
        if (isTaskTreeActive(task->children().at(i), activeTask))
            return true;

    return false;
}

static bool isTaskParentInList(Task *task, const QList<Task *> &list)
{
    while (task && task->parent()) {
        if (list.contains(task->parent()))
            return true;

        task = task->parent();
    }

    return false;
}


Tomato::Tomato(QObject *parent)
    : QObject(parent)
{
    m_rootTask = createTask(TaskData(), 0);
    m_activeTask = 0;

    m_state = Idle;

    m_tomatoStartTime = 0;
    m_workingTime = DefaultWorkingTime;
    m_restingTime =  DefaultRestingTime;

    // configure tomato timer
    m_tomatoTimer.setSingleShot(true);
    connect(&m_tomatoTimer, SIGNAL(timeout()),
            this, SLOT(tomatoTimer_timeout()));

    // this timer used for synchronous update the time fields
    m_timeSyncTimer.setSingleShot(false);
    m_timeSyncTimer.setInterval(500);
    connect(&m_timeSyncTimer, SIGNAL(timeout()),
            this, SIGNAL(timeSyncTimeout()));
    connect(&m_timeSyncTimer, SIGNAL(timeout()),
            this, SLOT(timeSyncTimer_timeout()));
}

void Tomato::setWorkingTime(qint64 seconds)
{
    if (m_workingTime == seconds) {
        return;
    }

    m_workingTime = seconds;
    emit workingTimeChanged();

    if (m_state != Working && m_state != OverWorking) {
        return;
    }

    if (calcTomatoTime() >= m_workingTime) {
        setState(OverWorking);
    }
    else {
        setState(Working);
    }
}

void Tomato::setRestingTime(qint64 seconds)
{
    if (m_restingTime == seconds) {
        return;
    }

    m_restingTime = seconds;
    emit restingTimeChanged();

    if (m_state != Resting && m_state != OverResting) {
        return;
    }

    if (calcTomatoTime() >= m_restingTime) {
        setState(OverResting);
    }
    else {
        setState(Resting);
    }
}

qint64 Tomato::timestamp() const
{
    return QDateTime::currentDateTime().toTime_t();
}

qint64 Tomato::calcTomatoTime() const
{
    if (m_state == Idle) {
        return 0;
    }

    return timestamp() - m_tomatoStartTime;
}

qint64 Tomato::calcTaskTime(int id) const
{
    const Task *task = findTask(id);
    qint64 total = task->totalTime();
    if ((m_state == Working || m_state == OverWorking) && isTaskTreeActive(task->id())) {
        total += timestamp() - m_activeTaskStartTime;
    }

    return total;
}

bool Tomato::isTaskTreeActive(int taskId) const
{
    return ::isTaskTreeActive(findTask(taskId), m_activeTask);
}

void Tomato::setActiveTask(int taskId)
{
    Task *task = findTask(taskId);

    // task is already inactive
    if (!m_activeTask && !task) {
        return;
    }

    // task is already active
    if (m_activeTask == task) {
        return;
    }

    // change the active task
    saveLastTimeToActiveTask();
    Task *prevActiveTask = m_activeTask;
    m_activeTask = task;
    m_activeTaskStartTime = timestamp();

    // start idle regim if active task unseted
    if (!m_activeTask) {
        startIdle();
    }

    if (prevActiveTask) {
        updateTask(prevActiveTask);
    }

    if (task) {
        updateTask(task);
    }

    emit activeTaskChanged();
}

void Tomato::unsetActiveTask()
{
    setActiveTask(-1);
}

TaskTime Tomato::calcActiveTaskTime() const
{
    return TaskTime(m_activeTaskStartTime, timestamp());
}

const TaskData &Tomato::taskData(int taskId) const
{
    return findTask(taskId)->data();
}

bool Tomato::setTaskData(int taskId, const TaskData &data)
{
    Task *task = findTask(taskId);
    if (!task) {
        return false;
    }

    const int index = task->index();

    emit aboutToBeTaskDataChanged(task->parent(), index, index);
    task->m_data = data;
    emit taskDataChanged(task->parent(), index, index);

    return true;
}

int Tomato::addTask(int parentTaskId, const TaskData &data)
{
    Task *parent = findTask(parentTaskId);
    Task *task = createTask(data, parent);
    const int index = parent->children().count();

    emit aboutToBeTaskInserted(parent, index, index);
    parent->m_children.push_back(task);
    task->calcTaskTime();
    task->calcTotalTime();
    emit taskInserted(parent, index, index);

    Task *tmp = task->parent();
    while (tmp && tmp->parent()) {
        const int index = tmp->index();

        emit aboutToBeTaskDataChanged(tmp->parent(), index, index);
        tmp->calcSubtasksTime();
        tmp->calcTotalTime();
        emit taskDataChanged(tmp->parent(), index, index);

        tmp = tmp->parent();
    }

    return task->id();
}

void Tomato::removeTask(int taskId)
{
    Task *task = findTask(taskId);
    if (!task) {
        return;
    }

    if (task == m_activeTask) {
        startIdle();
        unsetActiveTask();
    }

    Task *parent = task->parent();
    const int index = task->index();

    emit aboutToBeTaskRemoved(parent, index, index);
    destroyTask(task);
    parent->m_children.removeAt(index);
    emit taskRemoved(parent, index, index);

    Task *tmp = parent;
    while (tmp && tmp->parent()) {
        const int index = tmp->index();

        emit aboutToBeTaskDataChanged(tmp->parent(), index, index);
        tmp->calcSubtasksTime();
        tmp->calcTotalTime();
        emit taskDataChanged(tmp->parent(), index, index);

        tmp = tmp->parent();
    }
}

void Tomato::removeTasks(const QList<int> taskIds)
{
    // convert ids to tasks
    QList<Task *> uniqueTasks;
    foreach (const int id, taskIds) {
        Task *task = findTask(id);
        if (task) {
            uniqueTasks.push_back(task);
        }
    }

    // remove all child tasks
    // item1
    // + item11     (-- will be removed from unique tasks)
    // + item12     (-- will be removed from unique tasks)
    //   + item121  (-- will be removed from unique tasks)
    // item2
    // item3
    while (1) {
        bool removed = false;

        for (int i = 0; i < uniqueTasks.count(); ++i) {
            if (isTaskParentInList(uniqueTasks[i], uniqueTasks)) {
                uniqueTasks.removeAt(i);
                removed = true;
                break;
            }
        }

        if (!removed)
            break;
    }

    // stop the tomaton on removing active task
    if (uniqueTasks.contains(m_activeTask)) {
        startIdle();
        unsetActiveTask();
    }

    foreach (const Task *task, uniqueTasks) {
        removeTask(task->id());
    }
}

void Tomato::removeAllTasks()
{
    if (m_activeTask) {
        startIdle();
        unsetActiveTask();
    }

    const int first = 0;
    const int last = rootTask()->children().count() - 1;

    emit aboutToBeTaskRemoved(rootTask(), first, last);
    foreach (Task *task, m_rootTask->m_children) {
        delete task;
    }
    m_taskHash.clear();
    m_taskHash.insert(rootTask()->id(), rootTask());
    m_rootTask->m_children.clear();
    emit taskRemoved(rootTask(), first, last);
}

bool Tomato::moveTask(int taskId, int destParentTaskId, int destIndex)
{
    Task *task = findTask(taskId);
    Task *destParent = findTask(destParentTaskId);

    // task not found
    if (!task) {
        return false;
    }

    // task is root item
    if (!task->parent()) {
        return false;
    }

    // task is the same
    if (destIndex == -1 && task->parent() == destParent) {
        return false;
    }

    // task is parent of destParent
    if (taskTreeToList(task).contains(destParent)) {
        return false;
    }

    // drop to destParent is append to last position
    if (destIndex == -1) {
        destIndex = destParent->children().count();
    }

    int index = task->index();
    Task *srcParent = task->parent();
    emit aboutToBeTaskMoved(srcParent, index, index, destParent, destIndex);
    destParent->m_children.append(srcParent->m_children.takeAt(index));
    task->m_parent = destParent;
    srcParent->calcSubtasksTime();
    srcParent->calcTotalTime();
    destParent->calcSubtasksTime();
    destParent->calcTotalTime();
    emit taskMoved(srcParent, index, index, destParent, destIndex);

    Task *tmp = srcParent->parent();
    while (tmp && tmp->parent()) {
        const int index = tmp->index();

        emit aboutToBeTaskDataChanged(tmp->parent(), index, index);
        tmp->calcSubtasksTime();
        tmp->calcTotalTime();
        emit taskDataChanged(tmp->parent(), index, index);

        tmp = tmp->parent();
    }

    tmp = destParent->parent();
    while (tmp && tmp->parent()) {
        const int index = tmp->index();

        emit aboutToBeTaskDataChanged(tmp->parent(), index, index);
        tmp->calcSubtasksTime();
        tmp->calcTotalTime();
        emit taskDataChanged(tmp->parent(), index, index);

        tmp = tmp->parent();
    }

    return true;
}

bool Tomato::taskCanBeMoved(int taskId, int destParentTaskId)
{
    Task *task = findTask(taskId);
    Task *destParent = findTask(destParentTaskId);

    // task not found
    if (!task) {
        return false;
    }

    // task is root item
    if (!task->parent()) {
        return false;
    }

    // task is parent of destParent
    if (taskTreeToList(task).contains(destParent)) {
        return false;
    }

    return true;
}

QString Tomato::stateName(State state)
{
    switch (state) {
    case Idle:
        return tr("IDLE");
    case Working:
        return tr("WORKING");
    case OverWorking:
        return tr("IT'S TIME TO REST");
    case Resting:
        return tr("RESTING");
    case OverResting:
        return tr("IT'S TIME TO WORK");
    }

    return QString();
}

void Tomato::reset()
{
    emit aboutToBeReseted();

    foreach (Task *task, m_rootTask->m_children) {
        delete task;
    }
    m_taskHash.clear();
    m_taskHash.insert(rootTask()->id(), rootTask());
    m_rootTask->m_children.clear();

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
    saveLastTimeToActiveTask();

    if (!m_activeTask)
        return;

    m_activeTaskStartTime = timestamp();
    m_tomatoStartTime = timestamp();
    m_tomatoTimer.start(m_workingTime * 1000);
    setState(Working);

    m_timeSyncTimer.start();
}

void Tomato::startResting()
{
    saveLastTimeToActiveTask();

    if (!m_activeTask)
        return;

    m_tomatoStartTime = timestamp();
    m_tomatoTimer.start(m_restingTime * 1000);
    setState(Resting);

    m_timeSyncTimer.start();
}

void Tomato::startIdle()
{
    saveLastTimeToActiveTask();

    if (!m_activeTask)
        return;

    m_tomatoTimer.stop();
    setState(Idle);

    m_timeSyncTimer.stop();
}

void Tomato::timeSyncTimer_timeout()
{
    if (m_activeTask)
        updateTask(m_activeTask);
}

void Tomato::tomatoTimer_timeout()
{
    if (m_state == Working) {
        setState(OverWorking);
    }
    else if (m_state == Resting) {
        setState(OverResting);
    }
}

Task *Tomato::createTask(const TaskData &data, Task *parent)
{
    Task *task = new Task(data, parent);
    m_taskHash.insert(task->id(), task);
    return task;
}

void Tomato::destroyTask(Task *task)
{
    m_taskHash.remove(task->id());
    delete task;
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
    if (m_state == state) {
        return;
    }

    m_state = state;
    if (m_activeTask) {
        updateTask(m_activeTask);
    }

    emit stateChanged(m_state);
}

void Tomato::saveLastTimeToActiveTask()
{
    if (!m_activeTask) {
        return;
    }

    if (m_state != Working  && m_state != OverWorking) {
        return;
    }

    const TaskTime taskTime = TaskTime(m_activeTaskStartTime, timestamp());
    if (taskTime.isEmpty()) {
        return;
    }

    const int index = m_activeTask->index();
    emit aboutToBeTaskDataChanged(m_activeTask->parent(), index, index);
    m_activeTask->m_data.addTime(taskTime);
    m_activeTask->calcTaskTime();
    m_activeTask->calcTotalTime();
    emit taskDataChanged(m_activeTask->parent(), index, index);


    Task *tmp = m_activeTask->parent();
    while (tmp && tmp->parent()) {
        const int index = tmp->index();

        emit aboutToBeTaskDataChanged(tmp->parent(), index, index);
        tmp->calcSubtasksTime();
        tmp->calcTotalTime();
        emit taskDataChanged(tmp->parent(), index, index);

        tmp = tmp->parent();
    }
}
