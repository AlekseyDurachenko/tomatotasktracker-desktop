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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H


#include <QDialog>
class GetOpenFileNameWidget;
class QCheckBox;
class QSpinBox;


class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);

    bool isSaveChangesOnExit() const;
    void setSaveChangesOnExit(bool state);

    bool isSaveChangesPeriodically() const;
    void setSaveChangesPeriodically(bool state);

    int saveChangesInterval() const;
    void setSaveChangesInterval(int minutes);

    bool playWorkingFinishedSound() const;
    void setPlayWorkingFinishedSound(bool state);

    bool playRestingFinishedSound() const;
    void setPlayRestingFinishedSound(bool state);

    QString workingFinishedSound() const;
    void setWorkingFinishedSound(const QString &fileName);

    QString restingFinishedSound() const;
    void setRestingFinishedSound(const QString &fileName);

    bool isShowWorkingFinishedTrayNotify() const;
    void setShowWorkingFinishedTrayNotify(bool state);

    bool isShowRestingFinishedTrayNotify() const;
    void setShowRestingFinishedTrayNotify(bool state);

private:
    QCheckBox *m_saveChangesOnExitCheckBox;
    QCheckBox *m_saveChangesPeriodicallyCheckBox;
    QSpinBox *m_saveChangesIntervalSpinBox;
    QCheckBox *m_workingFinishedTrayNotifyCheckBox;
    QCheckBox *m_restingFinishedTrayNotifyCheckBox;
    QCheckBox *m_playWorkingFinishedSoundCheckBox;
    QCheckBox *m_playRestingFinishedSoundCheckBox;
    GetOpenFileNameWidget *m_workingFinishedSoundWidget;
    GetOpenFileNameWidget *m_restingFinishedSoundWidget;
};


#endif // SETTINGSDIALOG_H
