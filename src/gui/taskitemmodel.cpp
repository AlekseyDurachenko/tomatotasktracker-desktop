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
#include "taskitemmodel.h"
#include "theme.h"
#include "utils.h"
#include <QDateTime>
#include <QDebug>
#include <QFont>
#include <QIcon>
#include <QMimeData>


TaskItemModel::TaskItemModel(Tomato *tomato, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_tomato = tomato;
    connect(m_tomato, SIGNAL(aboutToBeTaskInserted(Task*,int,int)),
            this, SLOT(tomato_aboutToBeTaskInserted(Task*,int,int)));
    connect(m_tomato, SIGNAL(taskInserted(Task*,int,int)),
            this, SLOT(tomato_taskInserted(Task*,int,int)));
    connect(m_tomato, SIGNAL(aboutToBeTaskRemoved(Task*,int,int)),
            this, SLOT(tomato_aboutToBeTaskRemoved(Task*,int,int)));
    connect(m_tomato, SIGNAL(taskRemoved(Task*,int,int)),
            this, SLOT(tomato_taskRemoved(Task*,int,int)));
    connect(m_tomato, SIGNAL(aboutToBeTaskMoved(Task*,int,int,Task*,int)),
            this, SLOT(tomato_aboutToBeTaskMoved(Task*,int,int,Task*,int)));
    connect(m_tomato, SIGNAL(taskMoved(Task*,int,int,Task*,int)),
            this, SLOT(tomato_taskMoved(Task*,int,int,Task*,int)));
    connect(m_tomato, SIGNAL(aboutToBeTaskDataChanged(Task*,int,int)),
            this, SLOT(tomato_aboutToBeTaskDataChanged(Task*,int,int)));
    connect(m_tomato, SIGNAL(taskDataChanged(Task*,int,int)),
            this, SLOT(tomato_taskDataChanged(Task*,int,int)));
    connect(m_tomato, SIGNAL(aboutToBeTaskDisplayChanged(Task*,int,int)),
            this, SLOT(tomato_aboutToBeTaskDataChanged(Task*,int,int)));    
    connect(m_tomato, SIGNAL(taskDisplayChanged(Task*,int,int)),
            this, SLOT(tomato_taskDataChanged(Task*,int,int)));
    connect(m_tomato, SIGNAL(aboutToBeReseted()),
            this, SLOT(tomato_aboutToBeReseted()));
    connect(m_tomato, SIGNAL(reseted()), this, SLOT(tomato_reseted()));

}

QVariant TaskItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Task *task = static_cast<Task *>(index.internalPointer());
    const bool isActiveTask = m_tomato->isActiveTask(task);
    const qint64 totalTime = m_tomato->calcTaskTime(task);

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
        switch (index.column()) {
        case 0:
            return task->data().title();
        case 1:
            if (task->data().timeLimit() == 0)
                return secsToTimeStr(totalTime, true);

            return secsToTimeStr(task->data().timeLimit() - totalTime, true)
                    + "/" + secsToTimeStr(task->data().timeLimit(), true);
        default:
            break;
        }
    }

    if (role == Qt::DecorationRole) {
        if (index.column() == 0) {
            if (isActiveTask) {
                if (m_tomato->activeTask() == task) {
                    if (m_tomato->state() == Tomato::Working
                            || m_tomato->state() == Tomato::OverWorking)
                        return Theme::icon(Theme::IconTaskViewTaskActiveAndWorking);

                    return Theme::icon(Theme::IconTaskViewTaskActive);
                }

                return Theme::icon(Theme::IconTaskViewSubtaskActive);
            }

            if (task->data().isDone())
                return Theme::icon(Theme::IconTaskViewTaskDone);

            return Theme::icon(Theme::IconTaskViewTaskInactive);
        }
    }

    if (role == Qt::FontRole) {
        if (m_tomato->activeTask() == task) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }

    if (role == Qt::TextColorRole) {
        if (task->data().timeLimit() != 0 && task->data().timeLimit() < totalTime)
            return Qt::darkRed;
    }

    if (role == Qt::UserRole) {
        return Task::variantFromPtr(const_cast<Task *>(task));
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

QVariant TaskItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex TaskItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Task *parentTask;
    if (!parent.isValid())
        parentTask = m_tomato->rootTask();
    else
        parentTask = static_cast<Task *>(parent.internalPointer());

    return createIndex(row, column, parentTask->child(row));
}

QModelIndex TaskItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Task *childTask = static_cast<Task *>(index.internalPointer());
    Task *parentTask = childTask->parent();

    if (parentTask == m_tomato->rootTask())
        return QModelIndex();

    return createIndex(parentTask->index(), 0, parentTask);
}

int TaskItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    Task *parentTask;
    if (!parent.isValid())
        parentTask = m_tomato->rootTask();
    else
        parentTask = static_cast<Task *>(parent.internalPointer());

    return parentTask->childCount();
}

int TaskItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QStringList TaskItemModel::mimeTypes() const
{
    return QStringList() << "tomatotasktracker-desktop/task-ids";
}

QMimeData *TaskItemModel::mimeData(const QModelIndexList &indexes) const
{
    QList<int> ids;
    foreach (const QModelIndex &index, indexes)
    {
        if (!index.isValid())
            continue;

        if (index.column() != 0)
            continue;

        Task *task = static_cast<Task *>(index.internalPointer());
        if (!task)
            continue;

        ids.append(task->id());
    }

    if (ids.isEmpty())
        return 0;

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << ids;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("tomatotasktracker-desktop/task-ids", encodedData);
    return mimeData;
}

bool TaskItemModel::dropMimeData(const QMimeData *data, Qt::DropAction /*action*/,
                                 int row, int /*column*/, const QModelIndex &parent)
{
    Task *parentItem = static_cast<Task *>(parent.internalPointer());
    if (!parentItem)
        parentItem = m_tomato->rootTask();

    if (data->hasFormat("tomatotasktracker-desktop/task-ids")) {
        QByteArray encodedData = data->data("tomatotasktracker-desktop/task-ids");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        QList<int> ids;
        stream >> ids;

        foreach (int id, ids) {
            m_tomato->moveTask(m_tomato->findTask(id), parentItem, row);
        }

        return true;
    }

    return false;
}

void TaskItemModel::tomato_aboutToBeTaskInserted(Task *parent, int first, int last)
{
    if (parent == m_tomato->rootTask())
        beginInsertRows(QModelIndex(), first, last);
    else
        beginInsertRows(createIndex(parent->index(), 0, parent), first, last);
}

void TaskItemModel::tomato_taskInserted(Task */*parent*/, int /*first*/, int /*last*/)
{
    endInsertRows();
}

void TaskItemModel::tomato_aboutToBeTaskRemoved(Task *parent, int first, int last)
{
    if (parent == m_tomato->rootTask())
        beginRemoveRows(QModelIndex(), first, last);
    else
        beginRemoveRows(createIndex(parent->index(), 0, parent), first, last);
}

void TaskItemModel::tomato_taskRemoved(Task */*parent*/, int /*first*/, int /*last*/)
{
    endRemoveRows();
}

void TaskItemModel::tomato_aboutToBeTaskDataChanged(Task */*parent*/, int /*first*/, int /*last*/)
{
}

void TaskItemModel::tomato_taskDataChanged(Task *parent, int first, int last)
{
    Task *firstTask = parent->child(first);
    Task *lastTask = parent->child(last);
    emit dataChanged(createIndex(firstTask->index(), 0, firstTask),
                     createIndex(lastTask->index(),  columnCount() - 1, lastTask));
}

void TaskItemModel::tomato_aboutToBeTaskMoved(Task *srcParent,
        int srcFirst, int srcLast,
        Task *dstParent, int dstIndex)
{
    QModelIndex si = QModelIndex();
    QModelIndex di = QModelIndex();
    if (srcParent != m_tomato->rootTask())
        si = createIndex(srcParent->index(), 0, srcParent);
    if (dstParent != m_tomato->rootTask())
        di = createIndex(dstParent->index(), 0, dstParent);
    beginMoveRows(si, srcFirst, srcLast, di, dstIndex);
}

void TaskItemModel::tomato_taskMoved(Task */*srcParent*/,
        int /*srcFirst*/, int /*srcLast*/,
        Task */*dstParent*/, int /*dstIndex*/)
{
    endMoveRows();
}

void TaskItemModel::tomato_aboutToBeReseted()
{
    beginResetModel();
}

void TaskItemModel::tomato_reseted()
{
    endResetModel();
}
