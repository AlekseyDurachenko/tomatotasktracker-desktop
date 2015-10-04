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
#ifndef SETTINGS_H
#define SETTINGS_H

#include "version.h"


#define G_SETTINGS_INIT()                                                   \
        QSettings settings(QSettings::IniFormat, QSettings::UserScope,      \
                appName(), appName());                                      \


const char SettingLastPath[] = "LastPath";
const char SettingLastProject[] = "LastProjectFileName";
const char SettingPlayWorkingFinishSound[] = "PlayWorkingFinishSound";
const char SettingPlayRestingFinishSound[] = "PlayRestingFinishSound";
const char SettingSaveChangesOnExit[] = "SaveChangesOnExit";
const char SettingMainWindowGeometry[] = "MainWindow/geometry";
const char SettingMainWindowState[] = "MainWindow/state";
const char SettingTaskViewHeaderState[] = "TaskView/header/state";


#endif // SETTINGS_H
