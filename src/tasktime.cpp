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
#include "tasktime.h"


TaskTime::TaskTime(qint64 start, qint64 end)
{
    if (start <= 0 || end <= 0) {
        start = end = 0;
    }

    if (start < end) {
        m_start = start;
        m_end = end;
    }
    else {
        m_start = end;
        m_end = start;
    }

    m_total = m_end - m_start;
}

bool TaskTime::operator ==(const TaskTime &other) const
{
    return (m_start == other.start()
            && m_end == other.end());
}

bool TaskTime::operator !=(const TaskTime &other) const
{
    return (m_start != other.start()
            || m_end != other.end());
}
