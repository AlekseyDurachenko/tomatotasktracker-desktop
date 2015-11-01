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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"


SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Settings"));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

bool SettingsDialog::isPlayWorkingFinishSound() const
{
    return ui->playWorkingSound_checkBox->isChecked();
}

void SettingsDialog::setPlayWorkingFinishSound(bool state)
{
    ui->playWorkingSound_checkBox->setChecked(state);
}

bool SettingsDialog::isPlayRestingFinishSound() const
{
    return ui->playRestingSound_checkBox->isChecked();
}

void SettingsDialog::setPlayRestingFinishSound(bool state)
{
    ui->playRestingSound_checkBox->setChecked(state);
}

bool SettingsDialog::isShowWorkingFinishedTrayNotify() const
{
    return ui->showWorkingTrayNotify_checkBox->isChecked();
}

void SettingsDialog::setShowWorkingFinishedTrayNotify(bool state)
{
    ui->showWorkingTrayNotify_checkBox->setChecked(state);
}

bool SettingsDialog::isShowRestingFinishedTrayNotify() const
{
    return ui->showRestingTrayNotify_checkBox->isChecked();
}

void SettingsDialog::setShowRestingFinishedTrayNotify(bool state)
{
    ui->showRestingTrayNotify_checkBox->setChecked(state);
}

bool SettingsDialog::isSaveChangesOnExit() const
{
    return ui->saveChangesOnExit_checkBox->isChecked();
}

void SettingsDialog::setSaveChangesOnExit(bool state)
{
    ui->saveChangesOnExit_checkBox->setChecked(state);
}
