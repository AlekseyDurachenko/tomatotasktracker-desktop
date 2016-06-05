// Copyright 2015-2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "taskitemfilterproxymodel.h"
#include "taskitem.h"
#include "taskmanager.h"


TaskItemFilterProxyModel::TaskItemFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_isComplitedVisible = true;
}

void TaskItemFilterProxyModel::setComplitedVisible(bool isComplitedVisible)
{
    m_isComplitedVisible = isComplitedVisible;
    updateFilter();
}

bool TaskItemFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const TaskItem *taskItem = static_cast<TaskItem *>(sourceModel()->index(sourceRow, 0, sourceParent).internalPointer());
    if (!m_isComplitedVisible) {
        return (!taskItem->data().isCompleted()
                || taskItem->activeState() == TaskItem::Active
                || taskItem->activeState() == TaskItem::SubActive);
    }

    return true;
}

void TaskItemFilterProxyModel::updateFilter()
{
    invalidateFilter();
}
