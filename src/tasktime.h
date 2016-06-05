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
#ifndef TASKTIME_H
#define TASKTIME_H


#include <QtGlobal>


class TaskTime
{
public:
    explicit TaskTime(qint64 start = 0, qint64 end = 0);

    inline bool isNull() const;

    inline qint64 start() const;
    inline qint64 end() const;
    inline qint64 total() const;

    bool operator == (const TaskTime &other) const;
    bool operator != (const TaskTime &other) const;

private:
    qint64 m_start;
    qint64 m_end;
    qint64 m_total;
};

bool TaskTime::isNull() const
{
    return ((m_start <= 0) || (m_end <= 0));
}

qint64 TaskTime::start() const
{
    return m_start;
}

qint64 TaskTime::end() const
{
    return m_end;
}

qint64 TaskTime::total() const
{
    return m_total;
}


#endif // TASKTIME_H
