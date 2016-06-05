// Copyright 2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "tomato.h"
#include "consts.h"
#include <QDateTime>


Tomato::Tomato(TaskManager *taskManager, QObject *parent)
    : QObject(parent)
{
    m_taskManager = taskManager;
    connect(m_taskManager, SIGNAL(destroyed(QObject *)),
            this, SLOT(taskManager_destroyed()));
    connect(m_taskManager, SIGNAL(stateChanged(TaskManager::State)),
            this, SLOT(taskManager_stateChanged(TaskManager::State)));

    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

    m_state = Idle;

    m_workingTime = DefaultWorkingTime;
    m_restingTime = DefaultRestingTime;
}

void Tomato::setWorkingTime(qint64 workingTime)
{
    if (m_workingTime == workingTime) {
        return;
    }

    m_workingTime = workingTime;

    if (m_taskManager) {
        const qint64 time = m_workingTime - m_taskManager->calcLastStateTime();
        if (m_state == Working) {
            if (time >= 0) {
                m_timer.start(time * 1000);
            }
            else {
                m_timer.stop();

                m_state = OverWorking;
                emit stateChanged(m_state);
            }
        }
        else if (m_state == OverWorking) {
            if (time > 0) {
                m_timer.start(time * 1000);

                m_state = Working;
                emit stateChanged(m_state);
            }
        }
    }

    emit workingTimeChanged();
}

void Tomato::setRestingTime(qint64 restingTime)
{
    if (m_restingTime == restingTime) {
        return;
    }

    m_restingTime = restingTime;

    if (m_taskManager) {
        const qint64 time = m_restingTime - m_taskManager->calcLastStateTime();
        if (m_state == Resting) {
            if (time >= 0) {
                m_timer.start(time * 1000);
            }
            else {
                m_timer.stop();

                m_state = OverResting;
                emit stateChanged(m_state);
            }
        }
        else if (m_state == OverResting) {
            if (time > 0) {
                m_timer.start(time * 1000);

                m_state = Resting;
                emit stateChanged(m_state);
            }
        }
    }

    emit restingTimeChanged();
}

qint64 Tomato::timestamp()
{
    return QDateTime::currentDateTimeUtc().toTime_t();
}

void Tomato::taskManager_destroyed()
{
    m_taskManager = 0;

    if (m_state == Idle) {
        return;
    }

    m_timer.stop();

    m_state = Idle;
    emit stateChanged(m_state);
}

void Tomato::taskManager_stateChanged(TaskManager::State state)
{
    switch (state) {
    case TaskManager::Idle:
        if (m_state == Idle) {
            return;
        }

        m_timer.stop();

        m_state = Idle;
        emit stateChanged(m_state);

        break;

    case TaskManager::Working:
        if (m_state == Working || m_state == OverWorking) {
            return;
        }

        m_timer.start(m_workingTime * 1000);

        m_state = Working;
        emit stateChanged(m_state);

        break;

    case TaskManager::Resting:
        if (m_state == Resting || m_state == OverResting) {
            return;
        }

        m_timer.start(m_restingTime * 1000);

        m_state = Resting;
        emit stateChanged(m_state);

        break;
    }
}

void Tomato::timer_timeout()
{
    if (m_state == Working) {
        m_state = OverWorking;
        emit stateChanged(m_state);
    }
    else if (m_state == Resting) {
        m_state = OverResting;
        emit stateChanged(m_state);
    }
}
