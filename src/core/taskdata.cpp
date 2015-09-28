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
#include "taskdata.h"


TaskData::TaskData()
{
    m_timeLimit = 0;
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
    m_timeLimit = timeLimit;
}

void TaskData::setTimes(const QList<TaskTime> &times)
{
    m_times = times;
}

void TaskData::addTimes(const QList<TaskTime> &times)
{
    m_times.append(times);
}

void TaskData::addTime(const TaskTime &time)
{
    m_times.append(time);
}