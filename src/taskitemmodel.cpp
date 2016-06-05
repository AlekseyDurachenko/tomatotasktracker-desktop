// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARdedataChangedstinationParentdedataChangedstinationParentRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "taskitemmodel.h"
#include "consts.h"
#include "taskitem.h"
#include "taskmanager.h"
#include "theme.h"
#include "timestr.h"
#include "tomato.h"
#include <QDateTime>
#include <QFont>
#include <QIcon>
#include <QMimeData>


TaskItemModel::TaskItemModel(TaskManager *taskManager, QObject *parent)
    : QAbstractItemModel(parent)
    , m_taskManager(taskManager)
{
    connect(m_taskManager, SIGNAL(aboutToBeTaskInserted(TaskItem *, int, int)),
            this, SLOT(taskManager_aboutToBeTaskInserted(TaskItem *, int, int)));
    connect(m_taskManager, SIGNAL(taskInserted(TaskItem *, int, int)),
            this, SLOT(taskManager_taskInserted(TaskItem *, int, int)));

    connect(m_taskManager, SIGNAL(aboutToBeTaskRemoved(TaskItem *, int, int)),
            this, SLOT(taskManager_aboutToBeTaskRemoved(TaskItem *, int, int)));
    connect(m_taskManager, SIGNAL(taskRemoved()),
            this, SLOT(taskManager_taskRemoved()));

    connect(m_taskManager, SIGNAL(aboutToBeTaskMoved(TaskItem *, int, int, TaskItem *, int)),
            this, SLOT(taskManager_aboutToBeTaskMoved(TaskItem *, int, int, TaskItem *, int)));
    connect(m_taskManager, SIGNAL(taskMoved()),
            this, SLOT(taskManager_taskMoved()));

    connect(m_taskManager, SIGNAL(taskDataChanged(TaskItem *, int, int, bool, bool, bool, bool)),
            this, SLOT(taskManager_taskDataChanged(TaskItem *, int, int, bool, bool, bool, bool)));

    connect(m_taskManager, SIGNAL(stateChanged(TaskManager::State)),
            this, SLOT(taskManager_stateChanged(TaskManager::State)));
}

QVariant TaskItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const qint64 totalTime = m_taskManager->calcTaskTime(static_cast<TaskItem *>(index.internalPointer()));
    const TaskItem *taskItem = static_cast<TaskItem *>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
        switch (index.column()) {
        case 0:
            return taskItem->data().title();
        case 1:
            if (taskItem->data().timeLimit() == 0)
                return secsToTimeStr(totalTime, true);

            return secsToTimeStr(taskItem->data().timeLimit() - totalTime, true)
                   + "/"
                   + secsToTimeStr(taskItem->data().timeLimit(), true);
        default:
            ;
        }
    }

    if (role == Qt::DecorationRole) {
        if (index.column() == 0) {
            switch (taskItem->activeState()) {
            case TaskItem::Active:
                if (m_taskManager->state() == TaskManager::Working)
                    return theme::icon(theme::IconTaskWorking);
                else
                    return theme::icon(theme::IconTaskActive);
            case TaskItem::SubActive:
                return theme::icon(theme::IconSubTaskActive);
            case TaskItem::Inactive:
                if (taskItem->data().isCompleted())
                    return theme::icon(theme::IconTaskDone);
                else
                    return theme::icon(theme::IconTaskInactive);
            }
        }
    }

    if (role == Qt::FontRole) {
        if (taskItem->activeState() != TaskItem::Inactive) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }

    if (role == Qt::TextColorRole) {
        if (taskItem->data().timeLimit() != 0
                && taskItem->data().timeLimit() < totalTime) {
            return QColor(Qt::darkRed);
        }
    }

    if (role == Qt::UserRole) {
        return taskItem->id();
    }

    return QVariant();
}

Qt::ItemFlags TaskItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDragEnabled
               | Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled
           | Qt::ItemIsSelectable
           | Qt::ItemIsDragEnabled
           | Qt::ItemIsDropEnabled;
}

QVariant TaskItemModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Title");
        case 1:
            return tr("Time");
        default:
            break;
        }
    }

    return QVariant();
}

QModelIndex TaskItemModel::index(int row, int column,
                                 const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TaskItem *parentTaskItem;
    if (!parent.isValid())
        parentTaskItem = m_taskManager->rootItem();
    else
        parentTaskItem = static_cast<TaskItem *>(parent.internalPointer());

    return createIndex(row, column, parentTaskItem->value(row));
}

QModelIndex TaskItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TaskItem *childTaskItem = static_cast<TaskItem *>(index.internalPointer());
    TaskItem *parentTaskItem = childTaskItem->parent();

    if (parentTaskItem == m_taskManager->rootItem()) {
        return QModelIndex();
    }

    return createIndex(parentTaskItem->calcIndex(), 0, parentTaskItem);
}

int TaskItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    TaskItem *parentTaskItem;
    if (!parent.isValid())
        parentTaskItem = m_taskManager->rootItem();
    else
        parentTaskItem = static_cast<TaskItem *>(parent.internalPointer());

    return parentTaskItem->count();
}

int TaskItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 2;
}

QStringList TaskItemModel::mimeTypes() const
{
    return QStringList() << TaskIdsMimeData;
}

QMimeData *TaskItemModel::mimeData(const QModelIndexList &indexes) const
{
    QList<int> ids;
    foreach (const QModelIndex &index, indexes) {
        if (!index.isValid()) {
            continue;
        }

        if (index.column() != 0) {
            continue;
        }

        TaskItem *task = static_cast<TaskItem *>(index.internalPointer());
        if (!task) {
            continue;
        }

        ids.append(task->id());
    }

    if (ids.isEmpty()) {
        return 0;
    }

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << ids;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData(TaskIdsMimeData, encodedData);
    return mimeData;
}

bool TaskItemModel::dropMimeData(const QMimeData *data,
                                 Qt::DropAction action,
                                 int row, int column,
                                 const QModelIndex &parent)
{
    Q_UNUSED(action)
    Q_UNUSED(column)

    TaskItem *parentItemItem = static_cast<TaskItem *>(parent.internalPointer());
    if (!parentItemItem) {
        parentItemItem = m_taskManager->rootItem();
    }

    if (data->hasFormat(TaskIdsMimeData)) {
        QByteArray encodedData = data->data(TaskIdsMimeData);
        QDataStream stream(&encodedData, QIODevice::ReadOnly);

        QList<int> ids;
        stream >> ids;

        foreach (int id, ids) {
            TaskItem *item = m_taskManager->findById(id);
            if (item) {
                item->moveTo(parentItemItem, row);
            }
        }

        return true;
    }

    return false;
}

void TaskItemModel::taskManager_aboutToBeTaskInserted(TaskItem *parent, int first, int last)
{
    if (parent == m_taskManager->rootItem())
        beginInsertRows(QModelIndex(), first, last);
    else
        beginInsertRows(createIndex(parent->calcIndex(), 0, parent), first, last);
}

void TaskItemModel::taskManager_taskInserted(TaskItem *parent, int first, int last)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    endInsertRows();
}

void TaskItemModel::taskManager_aboutToBeTaskRemoved(TaskItem *parent, int first, int last)
{
    if (parent == m_taskManager->rootItem())
        beginRemoveRows(QModelIndex(), first, last);
    else
        beginRemoveRows(createIndex(parent->calcIndex(), 0, parent), first, last);
}

void TaskItemModel::taskManager_taskRemoved()
{
    endRemoveRows();
}

void TaskItemModel::taskManager_aboutToBeTaskMoved(TaskItem *sourceParent,
                                                   int sourceFirst,
                                                   int sourceLast,
                                                   TaskItem *destinationParent,
                                                   int destinationChild)
{
    QModelIndex si = QModelIndex();
    QModelIndex di = QModelIndex();
    if (sourceParent != m_taskManager->rootItem())
        si = createIndex(sourceParent->calcIndex(), 0, sourceParent);
    if (destinationParent != m_taskManager->rootItem())
        di = createIndex(destinationParent->calcIndex(), 0, destinationParent);
    beginMoveRows(si, sourceFirst, sourceLast, di, destinationChild);
}

void TaskItemModel::taskManager_taskMoved()
{
    endMoveRows();
}

void TaskItemModel::taskManager_taskDataChanged(TaskItem *parent,
                                                int first,
                                                int last,
                                                bool activeStateChanged,
                                                bool taskDataChanged,
                                                bool timeChanged,
                                                bool displayTimeChanged)
{
    Q_UNUSED(activeStateChanged)
    Q_UNUSED(taskDataChanged)
    Q_UNUSED(timeChanged)
    Q_UNUSED(displayTimeChanged)

    if (!parent) {
        return;
    }

    TaskItem *firstTask = parent->value(first);
    TaskItem *lastTask = parent->value(last);

    emit dataChanged(createIndex(firstTask->calcIndex(), 0, firstTask),
                     createIndex(lastTask->calcIndex(), columnCount() - 1, lastTask));
}

void TaskItemModel::taskManager_stateChanged(TaskManager::State state)
{
    Q_UNUSED(state)

    if (!m_taskManager->activeItem()) {
        return;
    }

    TaskItem *item = m_taskManager->activeItem();
    emit dataChanged(createIndex(item->calcIndex(), 0, item),
                     createIndex(item->calcIndex(), columnCount() - 1, item));
}
