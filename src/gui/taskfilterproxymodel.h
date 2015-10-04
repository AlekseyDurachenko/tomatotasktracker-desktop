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
#ifndef TASKFILTERPROXYMODEL_H
#define TASKFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
class Tomato;

class TaskFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    TaskFilterProxyModel(Tomato *tomato, QObject *parent = 0);

    inline bool enabled() const;
    void setEnabled(bool enabled);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private slots:
    void invFilter();

private:
    bool m_enabled;
    Tomato *m_tomato;
};

bool TaskFilterProxyModel::enabled() const
{
    return m_enabled;
}

#endif // TASKFILTERPROXYMODEL_H
