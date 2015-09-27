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


QHash<Theme::Icon, QIcon> Theme::m_icons;
QImage Theme::m_appIconImage;


void Theme::init()
{
    m_appIconImage = QImage(":/icons/taskview-task-active-and-started.png");

    m_icons.insert(IconActionTomatoStartWorking,    QIcon(":/icons/action-tomato-start-working.png"));
    m_icons.insert(IconActionTomatoStartResting,    QIcon(":/icons/action-tomato-start-resting.png"));
    m_icons.insert(IconActionTomatoStop,            QIcon(":/icons/action-tomato-stop.png"));

    m_icons.insert(IconActionTaskTimeAdd,       QIcon(":/icons/action-tasktime-add.png"));
    m_icons.insert(IconActionTaskTimeEdit,      QIcon(":/icons/action-tasktime-edit.png"));
    m_icons.insert(IconActionTaskTimeRemove,    QIcon(":/icons/action-tasktime-remove.png"));
    m_icons.insert(IconActionTaskTimeRemoveAll, QIcon(":/icons/action-tasktime-removeall.png"));

    m_icons.insert(IconActionAbout,     QIcon(":/icons/action-about.png"));
    m_icons.insert(IconActionAboutQt,   QIcon(":/icons/action-about-qt.png"));

    m_icons.insert(IconTaskViewTaskActiveAndWorking,    QIcon(":/icons/taskview-task-active-and-started.png"));
    m_icons.insert(IconTaskViewTaskActive,              QIcon(":/icons/taskview-task-active.png"));
    m_icons.insert(IconTaskViewTaskInactive,            QIcon(":/icons/taskview-task-inactive.png"));
    m_icons.insert(IconTaskViewSubtaskActive,           QIcon(":/icons/taskview-task-subtaskactive.png"));
}

const QIcon Theme::icon(Icon name, const QIcon &defaultIcon)
{
    return m_icons.value(name, defaultIcon);
}

const QImage &Theme::appIconImage()
{
    return m_appIconImage;
}
