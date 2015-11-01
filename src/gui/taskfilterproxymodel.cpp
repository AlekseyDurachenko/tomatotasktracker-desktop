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
#include "taskfilterproxymodel.h"
#include "task.h"
#include "tomato.h"


TaskFilterProxyModel::TaskFilterProxyModel(Tomato *tomato, QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_tomato = tomato;
    m_enabled = false;
    connect(tomato, SIGNAL(activeTaskChanged()),
            this, SLOT(tomato_activeTaskChanged()));
}

void TaskFilterProxyModel::setEnabled(bool enabled)
{
    m_enabled = enabled;
    invalidateFilter();
}

bool TaskFilterProxyModel::filterAcceptsRow(
    int sourceRow,
    const QModelIndex &sourceParent) const
{
    const Task *task = static_cast<Task *>(sourceModel()->index(sourceRow, 0, sourceParent).internalPointer());
    if (m_enabled) {
        return ((!task->data().isCompleted()) || m_tomato->findActiveSubtask(task->id()));
    }

    return true;
}

void TaskFilterProxyModel::tomato_activeTaskChanged()
{
    invalidateFilter();
}
