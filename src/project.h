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
#ifndef PROJECT_H
#define PROJECT_H


#include <QObject>
class TaskItem;
class TaskManager;
class Tomato;


class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);

    inline Tomato *tomato() const;
    inline TaskManager *taskManager() const;

    inline bool isOpen() const;
    inline bool hasChanges() const;
    inline const QString &fileName() const;

    bool create(const QString &fileName, QString *reason = 0);
    bool open(const QString &fileName, QString *reason = 0);
    bool save(QString *reason = 0);
    bool saveAs(const QString &fileName, QString *reason = 0);
    void close();

public slots:
    // this slot must be called by global synchronization timer
    // if project is open and task manager not in idle, the save state will
    // be changed to "has changed" and signal saveStateChaned() emited
    void sync();

signals:
    void saveStateChanged();
    void openStateChanged();

private slots:
    void taskManager_or_tomato_changed();
    void taskManager_taskDataChanged(TaskItem *parent, int first, int last,
                                     bool activeStateChanged,
                                     bool dataChanged,
                                     bool timeChanged,
                                     bool displayTimeChanged);

private:
    Tomato *m_tomato;
    TaskManager *m_taskManager;

    QString m_fileName;
    bool m_hasChanges;
};

Tomato *Project::tomato() const
{
    return m_tomato;
}

TaskManager *Project::taskManager() const
{
    return m_taskManager;
}

bool Project::isOpen() const
{
    return !m_fileName.isEmpty();
}

bool Project::hasChanges() const
{
    return m_hasChanges;
}

const QString &Project::fileName() const
{
    return m_fileName;
}


#endif // PROJECT_H
