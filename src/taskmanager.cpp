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
#include "taskmanager.h"
#include "tomato.h"


static QList<int> findTaskIdsRecursive(const TaskItem *item)
{
    QList<int> ids;
    ids.append(item->id());
    for (int i = 0; i < item->count(); ++i) {
        ids.append(findTaskIdsRecursive(item->at(i)));
    }

    return ids;
}


TaskManager::TaskManager(QObject *parent)
    : QObject(parent)
{
    m_rootItem = new TaskItem(this, TaskData(), QList<TaskTime>(), 0);
    m_activeItem = 0;
    m_activeItemStartTime = Tomato::timestamp();
    m_lastStateStartTime = Tomato::timestamp();
    m_state = Idle;
}

TaskManager::~TaskManager()
{
    delete m_rootItem;
}

void TaskManager::setActiveItem(TaskItem *task)
{
    if (m_activeItem == task) {
        return;
    }

    if (m_activeItem) {
        bool tomatoIsWorking = false;
        if (m_state == Working) {
            tomatoIsWorking = true;
            m_activeItem->appendTimeWithoutEmitDataChanged(TaskTime(m_activeItemStartTime, Tomato::timestamp()));
            m_activeItemStartTime = Tomato::timestamp();
        }

        TaskItem *item = m_activeItem;
        while (item) {
            const int index = item->calcIndex();
            const bool timeChanged = tomatoIsWorking && item == m_activeItem;

            beginDataChanged(item->parent(), index, index, true, false, timeChanged, tomatoIsWorking);
            item->setActiveStateWithoutEmitDataChanged(TaskItem::Inactive);
            endDataChanged(item->parent(), index, index, true, false, timeChanged, tomatoIsWorking);

            item = item->parent();
        }
    }

    m_activeItem = task;
    if (m_activeItem) {
        const int index = m_activeItem->calcIndex();

        beginDataChanged(m_activeItem->parent(), index, index, true, false, false, false);
        m_activeItem->setActiveStateWithoutEmitDataChanged(TaskItem::Active);
        endDataChanged(m_activeItem->parent(), index, index, true, false, false, false);

        TaskItem *item = m_activeItem->parent();
        while (item) {
            const int index = item->calcIndex();

            beginDataChanged(item->parent(), index, index, true, false, false, false);
            item->setActiveStateWithoutEmitDataChanged(TaskItem::SubActive);
            endDataChanged(item->parent(), index, index, true, false, false, false);

            item = item->parent();
        }
    }

    emit activeItemChanged();
}

qint64 TaskManager::calcLastStateTime() const
{
    return Tomato::timestamp() - m_lastStateStartTime;
}

TaskTime TaskManager::calcLastStateTaskTime() const
{
    return TaskTime(m_lastStateStartTime, Tomato::timestamp());
}

qint64 TaskManager::calcTaskTime(TaskItem *task) const
{
    if (!task) {
        return 0;
    }

    if (m_state != Working) {
        return task->totalTime();
    }

    if (task->activeState() == TaskItem::Inactive) {
        return task->totalTime();
    }

    return task->totalTime() + (Tomato::timestamp() - m_activeItemStartTime);
}

void TaskManager::sync()
{
    if (m_state != Working) {
        return;
    }

    TaskItem *item = m_activeItem;
    while (item) {
        const int index = item->calcIndex();

        beginDataChanged(item->parent(), index, index, false, false, false, true);
        endDataChanged(item->parent(), index, index, false, false, false, true);

        item = item->parent();
    }
}

void TaskManager::startWorking()
{
    if (!m_activeItem) {
        return;
    }

    if (m_state == Working) {
        m_activeItem->appendTime(TaskTime(m_activeItemStartTime, Tomato::timestamp()));
    }

    m_activeItemStartTime = Tomato::timestamp();
    m_lastStateStartTime = Tomato::timestamp();
    m_state = Working;

    emit stateChanged(m_state);
}

void TaskManager::startResting()
{
    if (!m_activeItem) {
        return;
    }

    if (m_state == Working) {
        m_activeItem->appendTime(TaskTime(m_activeItemStartTime, Tomato::timestamp()));
    }

    m_activeItemStartTime = Tomato::timestamp();
    m_lastStateStartTime = Tomato::timestamp();
    m_state = Resting;
    emit stateChanged(m_state);
}

void TaskManager::startIdle()
{
    if (m_state == Idle) {
        return;
    }

    if (m_state == Working) {
        if (m_activeItem) {
            m_activeItem->appendTime(TaskTime(m_activeItemStartTime, Tomato::timestamp()));
        }
    }

    m_activeItemStartTime = Tomato::timestamp();
    m_lastStateStartTime = Tomato::timestamp();
    m_state = Idle;
    emit stateChanged(m_state);
}

void TaskManager::beginInsertTasks(TaskItem *parent, int first, int last)
{
    emit aboutToBeTaskInserted(parent, first, last);
}

void TaskManager::endInsertTasks(TaskItem *parent, int first, int last)
{
    for (int i = first; i <= last; ++i) {
        m_items.insert(parent->at(i)->id(), parent->value(i));
    }

    emit taskInserted(parent, first, last);
}

void TaskManager::beginRemoveTasks(TaskItem *parent, int first, int last)
{
    for (int i = first; i <= last; ++i) {
        if (parent->at(i)->activeState() != TaskItem::Inactive) {
            m_activeItem = 0;
            emit activeItemChanged();
            startIdle();
            break;
        }
    }

    emit aboutToBeTaskRemoved(parent, first, last);
    for (int i = first; i <= last; ++i) {
        foreach (const int id, findTaskIdsRecursive(parent->at(i))) {
            m_items.remove(id);
        }
    }
}

void TaskManager::endRemoveTasks()
{
    emit taskRemoved();
}

void TaskManager::beginMoveTasks(TaskItem *sourceParent,
                                 int sourceFirst, int sourceLast,
                                 TaskItem *destinationParent,
                                 int destinationChild)
{
    emit aboutToBeTaskMoved(sourceParent, sourceFirst, sourceLast,
                            destinationParent, destinationChild);
}

void TaskManager::endMoveTasks()
{
    emit taskMoved();
}

void TaskManager::beginDataChanged(TaskItem *parent, int first, int last,
                                   bool activeStateChanged,
                                   bool dataChanged,
                                   bool timeChanged,
                                   bool displayTimeChanged)
{
    emit aboutToBeTaskDataChanged(parent, first, last, activeStateChanged,
                                  dataChanged, timeChanged, displayTimeChanged);
}

void TaskManager::endDataChanged(TaskItem *parent, int first, int last,
                                 bool activeStateChanged,
                                 bool dataChanged,
                                 bool timeChanged,
                                 bool displayTimeChanged)
{
    emit taskDataChanged(parent, first, last, activeStateChanged,
                         dataChanged, timeChanged, displayTimeChanged);
}
