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


void theme::init()
{
    m_appImage = QImage(":/icons/tomatotasktracker-desktop.png");

    m_icons.insert(IconApp, QIcon(":/icons/tomatotasktracker-desktop.png"));

    m_icons.insert(IconActionDocumentNew, QIcon(":/icons/action-document-new.png"));
    m_icons.insert(IconActionDocumentOpen, QIcon(":/icons/action-document-open.png"));
    m_icons.insert(IconActionDocumentSave, QIcon(":/icons/action-document-save.png"));
    m_icons.insert(IconActionDocumentSaveAs, QIcon(":/icons/action-document-save-as.png"));
    m_icons.insert(IconActionDocumentClose, QIcon(":/icons/action-document-close.png"));
    m_icons.insert(IconActionDocumentProperties, QIcon(":/icons/action-document-properties.png"));

    m_icons.insert(IconActionQuit, QIcon(":/icons/action-quit.png"));

    m_icons.insert(IconActionStartWorking, QIcon(":/icons/action-start-working.png"));
    m_icons.insert(IconActionStartResting, QIcon(":/icons/action-start-resting.png"));
    m_icons.insert(IconActionStop, QIcon(":/icons/action-stop.png"));

    m_icons.insert(IconActionTaskAdd, QIcon(":/icons/action-add.png"));
    m_icons.insert(IconActionTaskEdit, QIcon(":/icons/action-edit.png"));
    m_icons.insert(IconActionTaskRemove, QIcon(":/icons/action-remove.png"));
    m_icons.insert(IconActionTaskRemoveAll, QIcon(":/icons/action-remove-all.png"));

    m_icons.insert(IconActionTaskTimeAdd, QIcon(":/icons/action-add.png"));
    m_icons.insert(IconActionTaskTimeEdit, QIcon(":/icons/action-edit.png"));
    m_icons.insert(IconActionTaskTimeRemove, QIcon(":/icons/action-remove.png"));
    m_icons.insert(IconActionTaskTimeRemoveAll, QIcon(":/icons/action-remove-all.png"));

    m_icons.insert(IconActionSettings, QIcon(":/icons/action-settings.png"));

    m_icons.insert(IconActionAbout,     QIcon(":/icons/action-about.png"));
    m_icons.insert(IconActionAboutQt,   QIcon(":/icons/action-about-qt.png"));

    m_icons.insert(IconTaskViewTaskWorking, QIcon(":/icons/task-working.png"));
    m_icons.insert(IconTaskViewTaskActive, QIcon(":/icons/task-active.png"));
    m_icons.insert(IconTaskViewTaskInactive, QIcon(":/icons/task-inactive.png"));
    m_icons.insert(IconTaskViewSubtaskActive, QIcon(":/icons/task-subtask-active.png"));
    m_icons.insert(IconTaskViewTaskDone, QIcon(":/icons/task-done.png"));

    m_icons.insert(IconStatusIdle, QIcon(":/icons/status-idle.png"));
    m_icons.insert(IconStatusWorking, QIcon(":/icons/status-working.png"));
    m_icons.insert(IconStatusResting, QIcon(":/icons/status-resting.png"));
    m_icons.insert(IconStatusTimeout, QIcon(":/icons/status-timeout.png"));
}

const QIcon theme::icon(Icon name, const QIcon &defaultIcon)
{
    return m_icons.value(name, defaultIcon);
}

const QImage &theme::appImage()
{
    return m_appImage;
}
