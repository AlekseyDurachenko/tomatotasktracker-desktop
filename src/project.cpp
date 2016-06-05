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
#include "project.h"
#include "consts.h"
#include "projectxml.h"
#include "tomato.h"
#include <QFile>


Project::Project(QObject *parent)
    : QObject(parent)
{
    m_taskManager = new TaskManager(this);
    m_tomato = new Tomato(m_taskManager, this);

    // any changes of values of task manager or tomato
    // should be interpreted as "project changes"
    connect(m_taskManager, SIGNAL(taskInserted(TaskItem *, int, int)),
            this, SLOT(taskManager_or_tomato_changed()));
    connect(m_taskManager, SIGNAL(taskMoved()),
            this, SLOT(taskManager_or_tomato_changed()));
    connect(m_taskManager, SIGNAL(taskRemoved()),
            this, SLOT(taskManager_or_tomato_changed()));
    connect(m_taskManager, SIGNAL(taskDataChanged(TaskItem *, int, int, bool, bool, bool, bool)),
            this, SLOT(taskManager_taskDataChanged(TaskItem *, int, int, bool, bool, bool, bool)));
    connect(m_tomato, SIGNAL(workingTimeChanged()),
            this, SLOT(taskManager_or_tomato_changed()));
    connect(m_tomato, SIGNAL(restingTimeChanged()),
            this, SLOT(taskManager_or_tomato_changed()));

    m_fileName.clear();
    m_hasChanges = false;
}

bool Project::create(const QString &fileName, QString *reason)
{
    try {
        m_fileName.clear();
        m_taskManager->rootItem()->removeAll();
        m_tomato->setWorkingTime(DefaultWorkingTime);
        m_tomato->setRestingTime(DefaultRestingTime);
        m_hasChanges = false;

        QFile output(fileName);
        if (!output.open(QIODevice::WriteOnly)) {
            emit openStateChanged();
            throw output.errorString();
        }

        if (!saveProjectToXml(fileName, m_tomato, m_taskManager, reason)) {
            emit openStateChanged();
            return false;
        }

        m_fileName = fileName;
        emit openStateChanged();

        return true;
    }
    catch (const QString &error) {
        if (reason) {
            *reason = error;
        }
    }

    return false;
}

bool Project::open(const QString &fileName, QString *reason)
{
    try {
        m_fileName.clear();
        m_taskManager->rootItem()->removeAll();
        m_hasChanges = false;

        QFile output(fileName);
        if (!output.open(QIODevice::ReadOnly)) {
            emit openStateChanged();
            throw output.errorString();
        }

        if (!loadProjectFromXml(fileName, m_tomato, m_taskManager, reason)) {
            emit openStateChanged();
            return false;
        }

        m_fileName = fileName;
        emit openStateChanged();

        return true;
    }
    catch (const QString &error) {
        if (reason) {
            *reason = error;
        }
    }

    return false;
}

bool Project::save(QString *reason)
{
    return saveAs(m_fileName, reason);
}

bool Project::saveAs(const QString &fileName, QString *reason)
{
    try {
        QFile output(fileName);
        if (!output.open(QIODevice::WriteOnly)) {
            throw output.errorString();
        }

        if (!saveProjectToXml(fileName, m_tomato, m_taskManager, reason)) {
            return false;
        }

        m_fileName = fileName;
        m_hasChanges = false;
        emit saveStateChanged();

        return true;
    }
    catch (const QString &error) {
        if (reason) {
            *reason = error;
        }
    }

    return false;
}

void Project::close()
{
    if (!isOpen()) {
        return;
    }

    m_fileName.clear();
    m_taskManager->rootItem()->removeAll();
    m_tomato->setWorkingTime(DefaultWorkingTime);
    m_tomato->setRestingTime(DefaultRestingTime);
    m_hasChanges = false;

    emit openStateChanged();
}

void Project::sync()
{
    if (isOpen() && m_taskManager->state() != TaskManager::Idle) {
        m_hasChanges = true;
        emit saveStateChanged();
    }
}

void Project::taskManager_or_tomato_changed()
{
    if (!isOpen()) {
        return;
    }

    m_hasChanges = true;
    emit saveStateChanged();
}

void Project::taskManager_taskDataChanged(TaskItem *parent,
                                          int first,
                                          int last,
                                          bool activeStateChanged,
                                          bool dataChanged,
                                          bool timeChanged,
                                          bool displayTimeChanged)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)
    Q_UNUSED(activeStateChanged)
    Q_UNUSED(displayTimeChanged)

    if (dataChanged || timeChanged) {
        taskManager_or_tomato_changed();
    }
}
