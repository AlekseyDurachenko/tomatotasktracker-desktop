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
#ifndef TASKDATA_H
#define TASKDATA_H


#include <QString>


class TaskData
{
public:
    TaskData();

    inline const QString &title() const;
    void setTitle(const QString &title);

    inline const QString &desc() const;
    void setDesc(const QString &desc);

    inline qint64 timeLimit() const;
    void setTimeLimit(qint64 timeLimit);

    inline bool isCompleted() const;
    void setCompleted(bool completed);

    bool operator ==(const TaskData &other) const;
    bool operator !=(const TaskData &other) const;

private:
    QString m_title;
    QString m_desc;
    qint64 m_timeLimit;
    bool m_completed;
};

const QString &TaskData::title() const
{
    return m_title;
}

const QString &TaskData::desc() const
{
    return m_desc;
}

qint64 TaskData::timeLimit() const
{
    return m_timeLimit;
}

bool TaskData::isCompleted() const
{
    return m_completed;
}


#endif // TASKDATA_H
