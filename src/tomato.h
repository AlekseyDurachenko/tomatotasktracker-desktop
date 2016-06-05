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
#ifndef TOMATO_H
#define TOMATO_H


#include "taskmanager.h"
#include <QObject>
#include <QTimer>


class Tomato : public QObject
{
    Q_OBJECT
public:
    enum State {
        Idle        = 0,
        Working     = 1,
        Resting     = 2,
        OverWorking = 3,
        OverResting = 4
    };

public:
    explicit Tomato(TaskManager *taskManager, QObject *parent = 0);

    inline State state() const;

    inline qint64 workingTime() const;
    void setWorkingTime(qint64 workingTime);

    inline qint64 restingTime() const;
    void setRestingTime(qint64 restingTime);

public:
    static qint64 timestamp();

signals:
    void stateChanged(Tomato::State state);

    void workingTimeChanged();
    void restingTimeChanged();

private slots:
    void taskManager_destroyed();
    void taskManager_stateChanged(TaskManager::State state);

    void timer_timeout();

private:
    TaskManager *m_taskManager;
    QTimer m_timer;
    State m_state;
    qint64 m_workingTime;
    qint64 m_restingTime;
};

Tomato::State Tomato::state() const
{
    return m_state;
}

qint64 Tomato::workingTime() const
{
    return m_workingTime;
}

qint64 Tomato::restingTime() const
{
    return m_restingTime;
}


#endif // TOMATO_H
