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
#include <QDomDocument>


class Tomato : public QObject
{
    Q_OBJECT
    friend class TaskItemModel;
    friend bool saveProjectToXml(const QString &, const Tomato *, QString *);
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
    void setWorkingTime(qint64 seconds);

    inline qint64 restingTime() const;
    void setRestingTime(qint64 seconds);

    inline State state() const;

    qint64 timestamp() const;
    qint64 calcTomatoTime() const;
    qint64 calcTaskTime(int taskId) const;

    inline bool hasActiveTask() const;
    inline bool isActiveTask(int taskId) const;
    inline int activeTaskId() const;
    bool findActiveSubtask(int taskId) const;
    void setActiveTask(int taskId);
    void unsetActiveTask();
    TaskTime calcActiveTaskTime() const;

    const TaskData &taskData(int taskId) const;
    bool setTaskData(int taskId, const TaskData &data);

    int addTask(int parentTaskId, const TaskData &data);
    void removeTask(int taskId);
    void removeTasks(const QList<int> taskIds);
    void removeAllTasks();
    bool moveTask(int taskId, int destParentTaskId, int destIndex);
    bool taskCanBeMoved(int taskId, int destParentTaskId);

    inline int rootTaskId() const;
    inline int rootTaskCount() const;

private:
    inline Task *rootTask() const;
    inline Task *findTask(int id) const;

signals:
    void workingTimeChanged();
    void restingTimeChanged();
    void activeTaskChanged();
    void stateChanged(Tomato::State state);
    void timeSyncTimeout();

    void aboutToBeTaskInserted(Task *parent, int first, int last);
    void taskInserted(Task *parent, int first, int last);
    void aboutToBeTaskRemoved(Task *parent, int first, int last);
    void taskRemoved(Task *parent, int first, int last);
    void aboutToBeTaskDataChanged(Task *parent, int first, int last);
    void taskDataChanged(Task *parent, int first, int last);
    void aboutToBeTaskDisplayChanged(Task *parent, int first, int last);
    void taskDisplayChanged(Task *parent, int first, int last);
    void aboutToBeTaskMoved(Task *sourceParent,
                            int sourceFirst,
                            int sourceLast,
                            Task *destinationParent,
                            int destinationIndex);
    void taskMoved(Task *sourceParent,
                   int sourceFirst,
                   int sourceLast,
                   Task *destinationParent,
                   int destinationIndex);

    void aboutToBeReseted();
    void reseted();

public:
    static QString stateName(State state);

public slots:
    void startWorking();
    void startResting();
    void startIdle();

    void reset();

private slots:
    void tomatoTimer_timeout();
    void timeSyncTimer_timeout();

private:
    Task *createTask(const TaskData &data, Task *parent);
    void destroyTask(Task *task);

    void updateTask(Task *task);
    void setState(State state);
    void saveLastTimeToActiveTask();

private:
    Task *m_rootTask;

    Task *m_activeTask;
    qint64 m_activeTaskStartTime;

    State m_state;

    qint64 m_tomatoStartTime;
    qint64 m_workingTime;
    qint64 m_restingTime;

    QTimer m_tomatoTimer;
    QTimer m_timeSyncTimer;

    QHash<int, Task *> m_taskHash;
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

bool Tomato::hasActiveTask() const
{
    return m_activeTask;
}

bool Tomato::isActiveTask(int id) const
{
    if (!m_activeTask) {
        return false;
    }

    return m_activeTask->id() == id;
}

int Tomato::activeTaskId() const
{
    return m_activeTask->id();
}

int Tomato::rootTaskId() const
{
    return m_rootTask->id();
}

int Tomato::rootTaskCount() const
{
    return m_rootTask->children().count();
}

Task *Tomato::rootTask() const
{
    return m_rootTask;
}

Task *Tomato::findTask(int id) const
{
    return m_taskHash.value(id, 0);
}


#endif // TOMATO_H
