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
#ifndef CONSTS_H
#define CONSTS_H


#include <QtGlobal>


const char DisplayTaskTimeFormat[] = "yyyy-MM-dd HH:mm:ss";

const char DefaultProjectFileName[] = "untitled.ttt";

const char TaskIdsMimeData[] = "tomatotasktracker-desktop/task-ids";


const qint64 DefaultWorkingTime = 1500;     // seconds
const qint64 DefaultRestingTime = 300;      // seconds

const bool DefaultSettingsSavingChangesOnExit = true;
const bool DefaultSettingsSavingChangesOnTimer = true;
const qint64 DefaultSettingsSaveChangesInterval = 5;    // minutes

const bool DefaultSettingsPlayWorkingFinishedSound = true;
const bool DefaultSettingsPlayRestingFinishedSound = true;
const bool DefaultSettingsShowWorkingFinishTrayNotify = true;
const bool DefaultSettingsShowRestingFinishTrayNotify = true;
const char DefaultSettingsWorkingFinishedSound[] = "notify.wav";
const char DefaultSettingsRestingFinishedSound[] = "notify.wav";


const char SettingsLastPath[] = "LastPath";
const char SettingsLastProject[] = "LastProjectFileName";

const char SettingsMainWindowGeometry[] = "MainWindow/geometry";
const char SettingsMainWindowState[] = "MainWindow/state";
const char SettingsTaskViewHeaderState[] = "TaskView/header/state";

const char SettingsSaveChangesOnExit[] = "SaveChangesOnExit";
const char SettingsSaveChangesOnTimer[] = "SaveChangesOnTimer";
const char SettingsSaveChangesInterval[] = "SaveChangesInterval";

const char SettingsHideCompletedTasks[] = "HideCompletedTasks";

const char SettingsWorkingFinishedSoundFile[] = "WorkingFinishedSoundFile";
const char SettingsRestingFinishedSoundFile[] = "RestingFinishedSoundFile";
const char SettingsPlayWorkingFinishedSound[] = "WorkingPlayFinishedSound";
const char SettingsPlayRestingFinishedSound[] = "RestingPlayFinishedSound";
const char SettingsShowWorkingFinishTrayNotify[] = "ShowWorkingFinishTrayNotify";
const char SettingsShowRestingFinishTrayNotify[] = "ShowRestingFinishTrayNotify";


#endif // CONSTS_H
