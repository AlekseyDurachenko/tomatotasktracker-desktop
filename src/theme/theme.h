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
#ifndef THEME_H
#define THEME_H

#include <QHash>
#include <QIcon>


class Theme
{
public:
    enum Icon {
        IconApp,

        IconActionTomatoStartWorking,
        IconActionTomatoStartResting,
        IconActionTomatoStop,

        IconActionTaskTimeAdd,
        IconActionTaskTimeEdit,
        IconActionTaskTimeRemove,
        IconActionTaskTimeRemoveAll,

        IconActionAbout,
        IconActionAboutQt,

        IconTaskViewTaskActiveAndWorking,
        IconTaskViewTaskActive,
        IconTaskViewSubtaskActive,
        IconTaskViewTaskInactive,
        IconTaskViewTaskDone
    };

    static void init();
    static const QIcon icon(Icon name, const QIcon &defaultIcon = QIcon());
    static const QImage &appIconImage();
private:
    static QHash<Icon, QIcon> m_icons;
    static QImage m_appIconImage;
};


#endif // THEME_H

