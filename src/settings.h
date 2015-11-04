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
#include <QCoreApplication>
#include <QDir>


#ifdef APP_PORTABLE
    #define G_SETTINGS_INIT()                                               \
            QSettings settings(QCoreApplication::applicationDirPath()       \
                               + QDir::separator()                          \
                               + "settings.ini", QSettings::IniFormat);
#else
    #define G_SETTINGS_INIT()                                               \
            QSettings settings(QSettings::IniFormat, QSettings::UserScope,  \
                    appName(), appName());
#endif


const char SettingsLastPath[] = "LastPath";
const char SettingsLastProject[] = "LastProjectFileName";

const char SettingsMainWindowGeometry[] = "MainWindow/geometry";
const char SettingsMainWindowState[] = "MainWindow/state";
const char SettingsTaskViewHeaderState[] = "TaskView/header/state";
const char SettingsTaskViewHideCompleted[] = "TaskView/hideCompleted";

const char SettingsSaveChangesOnExit[] = "SaveChangesOnExit";
const char SettingsSaveChangesPeriodically[] = "SaveChangesPeriodically";
const char SettingsSaveChangesInterval[] = "SaveChangesInterval";

const char SettingsShowWorkingFinishTrayNotify[] = "ShowWorkingFinishTrayNotify";
const char SettingsShowRestingFinishTrayNotify[] = "ShowRestingFinishTrayNotify";

const char SettingsWorkingFinishedSoundFile[] = "WorkingFinishedSoundFile";
const char SettingsRestingFinishedSoundFile[] = "RestingFinishedSoundFile";
const char SettingsPlayWorkingFinishedSound[] = "WorkingPlayFinishedSound";
const char SettingsPlayRestingFinishedSound[] = "RestingPlayFinishedSound";


#endif // SETTINGS_H
