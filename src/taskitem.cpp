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
#include "taskitem.h"
#include "taskmanager.h"


TaskItem::TaskItem(TaskManager *manager, const TaskData &data, const QList<TaskTime> &times, TaskItem *parent)
{
    static qint64 id_counter = 0;
    m_id = ++id_counter;

    m_manager = manager;
    m_parent = parent;
    m_data = data;
    m_times = times;
    m_activeState = Inactive;

    m_taskTime = 0;
    foreach (const TaskTime &time, times) {
        m_taskTime += time.total();
    }

    m_totalTime = m_taskTime;
}

TaskItem::~TaskItem()
{
    foreach (TaskItem *item, m_children) {
        delete item;
    }
}

int TaskItem::calcIndex() const
{
    if (m_parent) {
        return m_parent->indexOf(const_cast<TaskItem *>(this));
    }

    return 0;
}

TaskItem *TaskItem::append(const TaskData &data, const QList<TaskTime> &times)
{
    const int index = m_children.count();

    m_manager->beginInsertTasks(this, index, index);
    m_children.append(new TaskItem(m_manager, data, times, this));
    m_manager->endInsertTasks(this, index, index);

    const qint64 addTime = m_children.last()->totalTime();

    TaskItem *item = this;
    while (item) {
        const int index = item->calcIndex();

        m_manager->beginDataChanged(item->parent(), index, index, false, false, false, true);
        item->m_totalTime += addTime;
        m_manager->endDataChanged(item->parent(), index, index, false, false, false, true);

        item = item->parent();
    }

    return m_children.last();
}

void TaskItem::removeAt(int index)
{
    const bool hasActive = m_children.at(index)->activeState() != Inactive;
    const qint64 subTime = m_children.at(index)->totalTime();

    m_manager->beginRemoveTasks(this, index, index);
    delete m_children.takeAt(index);
    m_manager->endRemoveTasks();

    const int thisIndex = calcIndex();

    m_manager->beginDataChanged(m_parent, thisIndex, thisIndex, hasActive, false, false, true);
    m_totalTime -= subTime;
    if (hasActive) {
        m_activeState = Inactive;
    }
    m_manager->endDataChanged(m_parent, thisIndex, thisIndex, hasActive, false, false, true);

    TaskItem *item = this->parent();
    while (item) {
        const int index = item->calcIndex();

        m_manager->beginDataChanged(item->parent(), index, index, hasActive, false, false, true);
        item->m_totalTime -= subTime;
        if (hasActive) {
            item->m_activeState = Inactive;
        }
        m_manager->endDataChanged(item->parent(), index, index, hasActive, false, false, true);

        item = item->parent();
    }
}

void TaskItem::removeAll()
{
    const bool hasActive = m_activeState == SubActive;
    const qint64 subTime = m_totalTime - m_taskTime;

    m_manager->beginRemoveTasks(this, 0, m_children.count() - 1);
    foreach (TaskItem *item, m_children) {
        delete item;
    }
    m_children.clear();
    m_manager->endRemoveTasks();

    const int index = calcIndex();

    m_manager->beginDataChanged(m_parent, index, index, hasActive, false, false, true);
    m_totalTime -= subTime;
    if (hasActive) {
        m_activeState = Inactive;
    }
    m_manager->endDataChanged(m_parent, index, index, hasActive, false, false, true);

    TaskItem *item = this->parent();
    while (item) {
        const int index = item->calcIndex();

        m_manager->beginDataChanged(item->parent(), index, index, hasActive, false, false, true);
        item->m_totalTime -= subTime;
        if (hasActive) {
            item->m_activeState = Inactive;
        }
        m_manager->endDataChanged(item->parent(), index, index, hasActive, false, false, true);

        item = item->parent();
    }
}

bool TaskItem::canMoveTo(TaskItem *task, int index) const
{
    const TaskItem *item = task;
    while (item) {
        if (item == this) {
            return false;
        }

        item = item->parent();
    }

    if (index > task->count()) {
        return false;
    }

    const int currentIndex = m_parent->indexOf(const_cast<TaskItem *>(this));
    if (m_parent == task && (index == currentIndex || index == currentIndex + 1)) {
        return false;
    }

    return true;
}

bool TaskItem::moveTo(TaskItem *task, int index)
{
    if (!canMoveTo(task, index)) {
        return false;
    }

    if (index < 0) {
        index = task->m_children.count();
    }

    TaskItem *item = m_parent;
    while (item) {
        const int index = item->calcIndex();

        m_manager->beginDataChanged(item->parent(), index, index, true, false, false, true);
        item->m_totalTime -= m_totalTime;
        if (m_activeState != Inactive) {
            item->m_activeState = Inactive;
        }
        m_manager->endDataChanged(item->parent(), index, index, true, false, false, true);

        item = item->parent();
    }

    const int thisIndex = calcIndex();

    m_manager->beginMoveTasks(m_parent, thisIndex, thisIndex, task, index);
    task->m_children.insert(index, m_parent->m_children.takeAt(m_parent->m_children.indexOf(this)));
    m_parent = task;
    m_manager->endMoveTasks();

    item = this->parent();
    while (item) {
        const int index = item->calcIndex();

        m_manager->beginDataChanged(item->parent(), index, index, true, false, false, true);
        item->m_totalTime += m_totalTime;
        if (m_activeState != Inactive) {
            item->m_activeState = SubActive;
        }
        m_manager->endDataChanged(item->parent(), index, index, true, false, false, true);

        item = item->parent();
    }

    return true;
}

void TaskItem::setData(const TaskData &data)
{
    if (m_data == data) {
        return;
    }

    const int index = calcIndex();

    m_manager->beginDataChanged(m_parent, index, index, false, true, false, false);
    m_data = data;
    m_manager->endDataChanged(m_parent, index, index, false, true, false, false);
}

void TaskItem::setData(const TaskData &data, const QList<TaskTime> &times)
{
    const bool isDataEquial = (m_data == data);
    const bool isTimesEquial = (m_times == times);

    if (isDataEquial && isTimesEquial) {
        return;
    }

    if (isTimesEquial) {
        const int index = calcIndex();

        m_manager->beginDataChanged(m_parent, index, index, false, true, false, false);
        m_data = data;
        m_manager->endDataChanged(m_parent, index, index, false, true, false, false);

        return;
    }

    qint64 prevTime = 0;
    foreach (const TaskTime &time, m_times) {
        prevTime += time.total();
    }

    qint64 nextTime = 0;
    foreach (const TaskTime &time, times) {
        nextTime += time.total();
    }

    const int index = calcIndex();

    m_manager->beginDataChanged(m_parent, index, index, false, true, true, true);
    m_data = data;
    m_times = times;
    m_taskTime = nextTime;
    m_totalTime -= prevTime;
    m_totalTime += nextTime;
    m_manager->endDataChanged(m_parent, index, index, false, true, true, true);

    TaskItem *item = this->parent();
    while (item) {
        const int index = item->calcIndex();

        m_manager->beginDataChanged(item->parent(), index, index, false, false, false, true);
        item->m_totalTime -= prevTime;
        item->m_totalTime += nextTime;
        m_manager->endDataChanged(item->parent(), index, index, false, false, false, true);

        item = item->parent();
    }
}

void TaskItem::setTimes(const QList<TaskTime> &times)
{
    if (m_times == times) {
        return;
    }

    qint64 prevTime = 0;
    foreach (const TaskTime &time, m_times) {
        prevTime += time.total();
    }

    qint64 nextTime = 0;
    foreach (const TaskTime &time, times) {
        nextTime += time.total();
    }

    const int index = calcIndex();

    m_manager->beginDataChanged(m_parent, index, index, false, false, true, true);
    m_times = times;
    m_taskTime = nextTime;
    m_totalTime -= prevTime;
    m_totalTime += nextTime;
    m_manager->endDataChanged(m_parent, index, index, false, false, true, true);

    TaskItem *item = this->parent();
    while (item) {
        const int index = item->calcIndex();

        m_manager->beginDataChanged(item->parent(), index, index, false, false, false, true);
        item->m_totalTime -= prevTime;
        item->m_totalTime += nextTime;
        m_manager->endDataChanged(item->parent(), index, index, false, false, false, true);

        item = item->parent();
    }
}

void TaskItem::appendTime(const TaskTime &time)
{
    const int index = calcIndex();

    m_manager->beginDataChanged(m_parent, index, index, false, false, true, true);
    m_times.append(time);
    m_taskTime += time.total();
    m_totalTime += time.total();
    m_manager->endDataChanged(m_parent, index, index, false, false, true, true);

    TaskItem *item = this->parent();
    while (item) {
        const int index = item->calcIndex();

        m_manager->beginDataChanged(item->parent(), index, index, false, false, false, true);
        item->m_totalTime += time.total();
        m_manager->endDataChanged(item->parent(), index, index, false, false, false, true);

        item = item->parent();
    }
}

void TaskItem::setActiveStateWithoutEmitDataChanged(TaskItem::ActiveState state)
{
    m_activeState = state;
}

void TaskItem::appendTimeWithoutEmitDataChanged(const TaskTime &time)
{
    m_times.append(time);
    m_taskTime += time.total();
    m_totalTime += time.total();

    TaskItem *item = this->parent();
    while (item) {
        item->m_totalTime += time.total();
        item = item->parent();
    }
}
