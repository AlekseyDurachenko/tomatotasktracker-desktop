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
#include "settings.h"
#include "getopenfilenamewidget.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>


SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Settings"));


    m_saveChangesOnExitCheckBox = new QCheckBox(this);
    m_saveChangesOnExitCheckBox->setText(
        tr("Automatically save the changes before exiting"));

    m_saveChangesPeriodicallyCheckBox = new QCheckBox(this);
    m_saveChangesPeriodicallyCheckBox->setText(
        tr("Automatically save the changes every"));

    m_saveChangesIntervalSpinBox = new QSpinBox(this);
    m_saveChangesIntervalSpinBox->setMinimum(1);
    m_saveChangesIntervalSpinBox->setMaximum(99999);
    m_saveChangesIntervalSpinBox->setValue(5);
    m_saveChangesIntervalSpinBox->setSuffix(tr(" minutes"));
    m_saveChangesIntervalSpinBox->setEnabled(false);
    connect(m_saveChangesPeriodicallyCheckBox, SIGNAL(toggled(bool)),
            m_saveChangesIntervalSpinBox, SLOT(setEnabled(bool)));

    m_workingFinishedTrayNotifyCheckBox = new QCheckBox(this);
    m_workingFinishedTrayNotifyCheckBox->setText(
        tr("Show a notification when the working time is over"));

    m_restingFinishedTrayNotifyCheckBox = new QCheckBox(this);
    m_restingFinishedTrayNotifyCheckBox->setText(
        tr("Show a notification when the resting time is over"));


    m_playWorkingFinishedSoundCheckBox = new QCheckBox(this);
    m_playWorkingFinishedSoundCheckBox->setText(
        tr("Play a sound when the working time is over"));

    m_playRestingFinishedSoundCheckBox = new QCheckBox(this);
    m_playRestingFinishedSoundCheckBox->setText(
        tr("Play a sound when the resting time is over"));


    m_workingFinishedSoundWidget = new GetOpenFileNameWidget(this);
    m_workingFinishedSoundWidget->setFileMask(tr("Audio files (*.wav)"));
    m_workingFinishedSoundWidget->setSettingsKey(SettingsWorkingFinishedSoundFile);
    m_workingFinishedSoundWidget->setEnabled(false);
    connect(m_playWorkingFinishedSoundCheckBox, SIGNAL(toggled(bool)),
            m_workingFinishedSoundWidget, SLOT(setEnabled(bool)));

    m_restingFinishedSoundWidget = new GetOpenFileNameWidget(this);
    m_restingFinishedSoundWidget->setFileMask(tr("Audio files (*.wav)"));
    m_restingFinishedSoundWidget->setSettingsKey(SettingsRestingFinishedSoundFile);
    m_restingFinishedSoundWidget->setEnabled(false);
    connect(m_playRestingFinishedSoundCheckBox, SIGNAL(toggled(bool)),
            m_restingFinishedSoundWidget, SLOT(setEnabled(bool)));


    QGroupBox *trayIconNotifyGroupBox = new QGroupBox(this);
    trayIconNotifyGroupBox->setTitle(tr("Tray notification options"));
    trayIconNotifyGroupBox->setLayout(new QVBoxLayout);
    trayIconNotifyGroupBox->layout()->addWidget(m_workingFinishedTrayNotifyCheckBox);
    trayIconNotifyGroupBox->layout()->addWidget(m_restingFinishedTrayNotifyCheckBox);

    QGroupBox *soundNotifyGroupBox = new QGroupBox(this);
    soundNotifyGroupBox->setTitle(tr("Sound notification options"));
    soundNotifyGroupBox->setLayout(new QVBoxLayout);
    soundNotifyGroupBox->layout()->addWidget(m_playWorkingFinishedSoundCheckBox);
    soundNotifyGroupBox->layout()->addWidget(m_workingFinishedSoundWidget);
    soundNotifyGroupBox->layout()->addWidget(m_playRestingFinishedSoundCheckBox);
    soundNotifyGroupBox->layout()->addWidget(m_restingFinishedSoundWidget);

    QHBoxLayout *saveChangesEveryMinutesHBox = new QHBoxLayout;
    saveChangesEveryMinutesHBox->addWidget(m_saveChangesPeriodicallyCheckBox);
    saveChangesEveryMinutesHBox->addWidget(m_saveChangesIntervalSpinBox);

    QGroupBox *savingGroupBox = new QGroupBox(this);
    savingGroupBox->setTitle(tr("Save options"));
    savingGroupBox->setLayout(new QVBoxLayout);
    savingGroupBox->layout()->addWidget(m_saveChangesOnExitCheckBox);
    static_cast<QVBoxLayout *>(savingGroupBox->layout())->addLayout(saveChangesEveryMinutesHBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(savingGroupBox);
    vbox->addWidget(trayIconNotifyGroupBox);
    vbox->addWidget(soundNotifyGroupBox);
    vbox->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    vbox->addWidget(buttonBox);
    setLayout(vbox);
}

bool SettingsDialog::isSaveChangesOnExit() const
{
    return m_saveChangesOnExitCheckBox->isChecked();
}

void SettingsDialog::setSaveChangesOnExit(bool state)
{
    m_saveChangesOnExitCheckBox->setChecked(state);
}

bool SettingsDialog::isSaveChangesPeriodically() const
{
    return m_saveChangesPeriodicallyCheckBox->isChecked();
}

void SettingsDialog::setSaveChangesPeriodically(bool state)
{
    m_saveChangesPeriodicallyCheckBox->setChecked(state);
}

int SettingsDialog::saveChangesInterval() const
{
    return m_saveChangesIntervalSpinBox->value();
}

void SettingsDialog::setSaveChangesInterval(int minutes)
{
    m_saveChangesIntervalSpinBox->setValue(minutes);
}

bool SettingsDialog::playWorkingFinishedSound() const
{
    return m_playWorkingFinishedSoundCheckBox->isChecked();
}

void SettingsDialog::setPlayWorkingFinishedSound(bool state)
{
    m_playWorkingFinishedSoundCheckBox->setChecked(state);
}

bool SettingsDialog::playRestingFinishedSound() const
{
    return m_playRestingFinishedSoundCheckBox->isChecked();
}

void SettingsDialog::setPlayRestingFinishedSound(bool state)
{
    m_playRestingFinishedSoundCheckBox->setChecked(state);
}

QString SettingsDialog::workingFinishedSound() const
{
    return m_workingFinishedSoundWidget->fileName();
}

void SettingsDialog::setWorkingFinishedSound(const QString &fileName)
{
    m_workingFinishedSoundWidget->setFileName(fileName);
}

QString SettingsDialog::restingFinishedSound() const
{
    return m_restingFinishedSoundWidget->fileName();
}

void SettingsDialog::setRestingFinishedSound(const QString &fileName)
{
    m_restingFinishedSoundWidget->setFileName(fileName);
}

bool SettingsDialog::isShowWorkingFinishedTrayNotify() const
{
    return m_workingFinishedTrayNotifyCheckBox->isChecked();
}

void SettingsDialog::setShowWorkingFinishedTrayNotify(bool state)
{
    m_workingFinishedTrayNotifyCheckBox->setChecked(state);
}

bool SettingsDialog::isShowRestingFinishedTrayNotify() const
{
    return m_restingFinishedTrayNotifyCheckBox->isChecked();
}

void SettingsDialog::setShowRestingFinishedTrayNotify(bool state)
{
    m_restingFinishedTrayNotifyCheckBox->setChecked(state);
}
