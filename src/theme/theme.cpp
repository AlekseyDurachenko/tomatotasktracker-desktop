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
#include "theme.h"


static QHash<theme::Icon, QIcon> m_icons;
static QImage m_appImage;


static QString themeFileName(const QString &baseFileName)
{
    return QString(":/icons/%1.png").arg(baseFileName);
}

static QIcon themeIcon(const QString &baseFileName)
{
    return QIcon(themeFileName(baseFileName));
}

static QImage themeImage(const QString &baseFileName)
{
    return QImage(themeFileName(baseFileName));
}


void theme::init()
{
    m_appImage = themeImage("tomatotasktracker-desktop");

    m_icons.insert(IconApp, themeIcon("tomatotasktracker-desktop"));

    m_icons.insert(IconActionDocumentNew, themeIcon("action-document-new"));
    m_icons.insert(IconActionDocumentOpen, themeIcon("action-document-open"));
    m_icons.insert(IconActionDocumentSave, themeIcon("action-document-save"));
    m_icons.insert(IconActionDocumentSaveAs, themeIcon("action-document-save-as"));
    m_icons.insert(IconActionDocumentClose, themeIcon("action-document-close"));
    m_icons.insert(IconActionDocumentProperties, themeIcon("action-document-properties"));

    m_icons.insert(IconActionQuit, themeIcon("action-quit"));

    m_icons.insert(IconActionStartWorking, themeIcon("action-start-working"));
    m_icons.insert(IconActionStartResting, themeIcon("action-start-resting"));
    m_icons.insert(IconActionStop, themeIcon("action-stop"));

    m_icons.insert(IconActionTaskAdd, themeIcon("action-add"));
    m_icons.insert(IconActionTaskEdit, themeIcon("action-edit"));
    m_icons.insert(IconActionTaskRemove, themeIcon("action-remove"));
    m_icons.insert(IconActionTaskRemoveAll, themeIcon("action-remove-all"));

    m_icons.insert(IconActionTaskTimeAdd, themeIcon("action-add"));
    m_icons.insert(IconActionTaskTimeEdit, themeIcon("action-edit"));
    m_icons.insert(IconActionTaskTimeRemove, themeIcon("action-remove"));
    m_icons.insert(IconActionTaskTimeRemoveAll, themeIcon("action-remove-all"));

    m_icons.insert(IconActionSettings, themeIcon("action-settings"));

    m_icons.insert(IconActionAbout,     themeIcon("action-about"));
    m_icons.insert(IconActionAboutQt,   themeIcon("action-about-qt"));

    m_icons.insert(IconTaskViewTaskWorking, themeIcon("task-working"));
    m_icons.insert(IconTaskViewTaskActive, themeIcon("task-active"));
    m_icons.insert(IconTaskViewTaskInactive, themeIcon("task-inactive"));
    m_icons.insert(IconTaskViewSubtaskActive, themeIcon("task-subtask-active"));
    m_icons.insert(IconTaskViewTaskDone, themeIcon("task-done"));

    m_icons.insert(IconStatusIdle, themeIcon("status-idle"));
    m_icons.insert(IconStatusWorking, themeIcon("status-working"));
    m_icons.insert(IconStatusResting, themeIcon("status-resting"));
    m_icons.insert(IconStatusTimeout, themeIcon("status-timeout"));
}

const QIcon theme::icon(Icon name, const QIcon &defaultIcon)
{
    return m_icons.value(name, defaultIcon);
}

const QImage &theme::appImage()
{
    return m_appImage;
}
