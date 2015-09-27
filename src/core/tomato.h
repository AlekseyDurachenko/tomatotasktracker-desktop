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
#ifndef TOMATO_H
#define TOMATO_H

#include "task.h"
#include <QObject>
#include <QTimer>


class Tomato : public QObject
{
    Q_OBJECT
public:
    enum State {
        Idle,
        Working,
        OverWorking,
        Resting,
        OverResting
    };
public:
    explicit Tomato(QObject *parent = 0);

    inline qint64 workingTime() const;
    void setWorkingTime(qint64 workingTime);

    inline qint64 restingTime() const;
    void setRestingTime(qint64 restingTime);

    inline State state() const;
    QString stateText() const;

    qint64 calcTomatoTime() const;
    qint64 calcTaskTime(const Task *task) const;

    inline Task *activeTask() const;
    void setActiveTask(Task *task);   
    bool isActiveTask(const Task *task) const;
    TaskTime calcActiveTaskTime() const;

    inline Task *rootTask() const;

    void setTaskData(Task *task, const TaskData &data);
    void addTaskTime(Task *task, const TaskTime &taskTime);

    Task *addChildTask(Task *parent, const TaskData &data);
    void removeChildTask(Task *parent, int index);
    void removeTasks(const QList<Task *> tasks);
    void removeAllTasks();

signals:
    void stateChanged(Tomato::State state);

    void activeTaskChanged();
    void workingTimeChanged();
    void restingTimeChanged();

    void aboutToBeTaskInserted(Task *parent, int first, int last);
    void taskInserted(Task *parent, int first, int last);
    void aboutToBeTaskRemoved(Task *parent, int first, int last);
    void taskRemoved(Task *parent, int first, int last);
    void aboutToBeTaskDataChanged(Task *parent, int first, int last);
    void taskDataChanged(Task *parent, int first, int last);
    void aboutToBeTaskDisplayChanged(Task *parent, int first, int last);
    void taskDisplayChanged(Task *parent, int first, int last);

    void aboutToBeReseted();
    void reseted();

public slots:
    void reset();

    void startWorking();
    void startResting();
    void startIdle();

    void updateActiveTaskDisplay();

private slots:
    void tomatoStartTimer_timeout();

private:
    qint64 timestamp() const;

    void updateTask(Task *task);
    void setState(State state);
    void saveLastTaskTime();    

    void cascadeEmitAboutToBeTaskDataChanged(Task *task);
    void cascadeEmitTaskDataChanged(Task *task);

private:
    Task *m_rootTask;
    Task *m_activeTask;
    qint64 m_activeTaskStartTime;

    State m_state;

    qint64 m_tomatoStartTime;

    qint64 m_workingTime;
    qint64 m_restingTime;

    QTimer m_tomatoTimer;
};

qint64 Tomato::workingTime() const
{
    return m_workingTime;
}

qint64 Tomato::restingTime() const
{
    return m_restingTime;
}

Tomato::State Tomato::state() const
{
    return m_state;
}

Task *Tomato::activeTask() const
{
    return m_activeTask;
}

Task *Tomato::rootTask() const
{
    return m_rootTask;
}


#endif // TOMATO_H
