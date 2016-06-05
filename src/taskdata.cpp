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
#include "taskdata.h"


TaskData::TaskData()
{
    m_timeLimit = 0;
    m_completed = false;
}

void TaskData::setTitle(const QString &title)
{
    m_title = title;
}

void TaskData::setDesc(const QString &desc)
{
    m_desc = desc;
}

void TaskData::setTimeLimit(qint64 timeLimit)
{
    if (timeLimit < 0) {
        timeLimit = 0;
    }
    else {
        m_timeLimit = timeLimit;
    }
}

void TaskData::setCompleted(bool completed)
{
    m_completed = completed;
}

bool TaskData::operator ==(const TaskData &other) const
{
    return (m_title == other.title()
            && m_desc == other.desc()
            && m_timeLimit == other.timeLimit()
            && m_completed == other.isCompleted());
}

bool TaskData::operator !=(const TaskData &other) const
{
    return (m_title != other.title()
            || m_desc != other.desc()
            || m_timeLimit != other.timeLimit()
            || m_completed != other.isCompleted());
}
