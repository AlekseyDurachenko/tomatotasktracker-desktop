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
#include "theme.h"
#include <QFile>


Project::Project(QObject *parent)
    : QObject(parent)
{
    createActions();

    m_tomato = new Tomato(this);
    connect(m_tomato, SIGNAL(workingTimeChanged()),
            this, SLOT(savingDataChanged()));
    connect(m_tomato, SIGNAL(restingTimeChanged()),
            this, SLOT(savingDataChanged()));
    connect(m_tomato, SIGNAL(taskInserted(Task *, int, int)),
            this, SLOT(savingDataChanged()));
    connect(m_tomato, SIGNAL(taskRemoved(Task *, int, int)),
            this, SLOT(savingDataChanged()));
    connect(m_tomato, SIGNAL(taskDataChanged(Task *, int, int)),
            this, SLOT(savingDataChanged()));

    connect(m_tomato, SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateActions()));
    connect(m_tomato, SIGNAL(activeTaskChanged()),
            this, SLOT(updateActions()));

    m_fileName.clear();
    m_hasChanges = false;
    updateActions();
}

bool Project::create(const QString &fileName, QString *reason)
{
    try {
        m_fileName.clear();
        m_tomato->reset();
        m_hasChanges = false;
        updateActions();

        QFile output(fileName);
        if (!output.open(QIODevice::WriteOnly)) {
            emit openStateChanged();
            throw output.errorString();
        }

        if (!saveProjectToXml(fileName, m_tomato, reason)) {
            emit openStateChanged();
            return false;
        }

        m_fileName = fileName;
        updateActions();
        emit openStateChanged();

        return true;
    }
    catch (const QString &error) {
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
        updateActions();

        QFile output(fileName);
        if (!output.open(QIODevice::ReadOnly)) {
            emit openStateChanged();
            throw output.errorString();
        }

        if (!loadProjectFromXml(fileName, m_tomato, reason)) {
            emit openStateChanged();
            return false;
        }

        m_fileName = fileName;
        m_hasChanges = false;
        updateActions();
        emit openStateChanged();

        return true;
    }
    catch (const QString &error) {
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
        updateActions();
        emit saveStateChanged();

        return true;
    }
    catch (const QString &error) {
        if (reason)
            *reason = error;
    }

    return false;
}

void Project::close()
{
    if (!isOpen())
        return;

    m_fileName.clear();
    m_tomato->reset();
    m_hasChanges = false;
    updateActions();
    emit openStateChanged();
}

void Project::savingDataChanged()
{
    if (isOpen()) {
        m_hasChanges = true;
        updateActions();
        // this signal will not emited on create(), close(), open(),
        // because before tomato reseting we close the project by
        // setting m_fileName to empty string.
        emit saveStateChanged();
    }
}

void Project::updateActions()
{
    m_saveAction->setEnabled(!m_fileName.isEmpty() && m_hasChanges);
    m_saveAsAction->setEnabled(!m_fileName.isEmpty() && m_hasChanges);
    m_closeAction->setEnabled(!m_fileName.isEmpty());
    m_propertiesAction->setEnabled(!m_fileName.isEmpty());

    if (m_tomato->hasActiveTask()) {
        m_startAction->setEnabled(true);
        m_stopAction->setEnabled(true);

        switch (m_tomato->state()) {
        case Tomato::Idle:
            m_stopAction->setEnabled(false);
        case Tomato::Resting:
        case Tomato::OverResting:
            m_startAction->setText(tr("&Start working"));
            m_startAction->setIcon(theme::icon(theme::IconActionStartWorking));
            break;
        case Tomato::Working:
        case Tomato::OverWorking:
            m_startAction->setText(tr("&Start resting"));
            m_startAction->setIcon(theme::icon(theme::IconActionStartResting));
        }
    }
    else {
        m_startAction->setEnabled(false);
        m_startAction->setText(tr("&Start working"));
        m_startAction->setIcon(theme::icon(theme::IconActionStartWorking));
        m_stopAction->setEnabled(false);
    }
}

void Project::createActions()
{
    m_newAction = new QAction(this);
    m_newAction->setText(tr("&New..."));
    m_newAction->setIcon(theme::icon(theme::IconActionDocumentNew));

    m_openAction = new QAction(this);
    m_openAction->setText(tr("&Open..."));
    m_openAction->setIcon(theme::icon(theme::IconActionDocumentOpen));

    m_saveAction = new QAction(this);
    m_saveAction->setText(tr("&Save..."));
    m_saveAction->setIcon(theme::icon(theme::IconActionDocumentSave));

    m_saveAsAction = new QAction(this);
    m_saveAsAction->setText(tr("Save &as..."));
    m_saveAsAction->setIcon(theme::icon(theme::IconActionDocumentSaveAs));

    m_closeAction = new QAction(this);
    m_closeAction->setText(tr("Close..."));
    m_closeAction->setIcon(theme::icon(theme::IconActionDocumentClose));

    m_propertiesAction = new QAction(this);
    m_propertiesAction->setText(tr("&Properties..."));
    m_propertiesAction->setIcon(theme::icon(theme::IconActionDocumentProperties));

    m_startAction = new QAction(this);

    m_stopAction = new QAction(this);
    m_stopAction->setText(tr("Sto&p"));
    m_stopAction->setIcon(theme::icon(theme::IconActionStop));
}
