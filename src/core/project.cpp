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
#include "project.h"
#include "projectxml.h"
#include <QFile>


Project::Project(Tomato *tomato, QObject *parent) : QObject(parent)
{
    m_tomato = tomato;
    connect(m_tomato, SIGNAL(workingTimeChanged()),
            this, SLOT(tomatoSavingDataChanged()));
    connect(m_tomato, SIGNAL(restingTimeChanged()),
            this, SLOT(tomatoSavingDataChanged()));
    connect(m_tomato, SIGNAL(taskInserted(Task*,int,int)),
            this, SLOT(tomatoSavingDataChanged()));
    connect(m_tomato, SIGNAL(taskRemoved(Task*,int,int)),
            this, SLOT(tomatoSavingDataChanged()));
    connect(m_tomato, SIGNAL(taskDataChanged(Task*,int,int)),
            this, SLOT(tomatoSavingDataChanged()));

    m_fileName.clear();
    m_hasChanges = false;
}

bool Project::create(const QString &fileName, QString *reason)
{
    try {
        m_fileName.clear();
        m_tomato->reset();
        m_hasChanges = false;

        QFile output(fileName);
        if (!output.open(QIODevice::WriteOnly))
            throw output.errorString();

        if (!saveProjectToXml(fileName, m_tomato, reason))
            return false;

        m_fileName = fileName;
        return true;
    } catch (const QString &error) {
        if (reason)
            *reason = error;
    }

    return false;
}

bool Project::open(const QString &fileName, QString *reason)
{
    try {
        m_fileName.clear();
        m_tomato->reset();
        m_hasChanges = false;

        QFile output(fileName);
        if (!output.open(QIODevice::ReadOnly))
            throw output.errorString();

        if (!loadProjectFromXml(fileName, m_tomato, reason))
            return false;

        m_fileName = fileName;
        m_hasChanges = false;
        return true;
    } catch (const QString &error) {
        if (reason)
            *reason = error;
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
        if (!output.open(QIODevice::WriteOnly))
            throw output.errorString();

        if (!saveProjectToXml(fileName, m_tomato, reason))
            return false;

        m_fileName = fileName;
        m_hasChanges = false;
        return true;
    } catch (const QString &error) {
        if (reason)
            *reason = error;
    }

    return false;
}

void Project::close()
{
    m_fileName.clear();
    m_tomato->reset();
    m_hasChanges = false;
}

void Project::tomatoSavingDataChanged()
{
    if (isOpen()) {
        m_hasChanges = true;
        // this signal will not emited on create(), close(), open(),
        // because before tomato reseting we close the project by
        // setting m_fileName to empty string.
        emit changed();
    }
}
