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
#include "task.h"


static int m_uniqueCounter = 0;


Task::Task(const TaskData &data, Task *parent)
{
    m_id = (++m_uniqueCounter);

    m_data = data;
    m_parent = parent;

    m_totalTime = 0;
    m_subtasksTime = 0;
    m_taskTime = 0;
}

Task::~Task()
{
    foreach (Task *task, m_children)
        delete task;
}

void Task::calcTotalTime()
{
    m_totalTime = m_taskTime + m_subtasksTime;
}

void Task::calcSubtasksTime()
{
    m_subtasksTime = 0;
    foreach (const Task *task, m_children)
        m_subtasksTime += task->totalTime();
}

void Task::calcTaskTime()
{
    m_taskTime = 0;
    foreach (const TaskTime &taskTime, m_data.times())
        m_taskTime += taskTime.total();
}
