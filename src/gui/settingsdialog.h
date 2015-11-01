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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H


#include <QDialog>


namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    virtual ~SettingsDialog();

    bool isPlayWorkingFinishSound() const;
    void setPlayWorkingFinishSound(bool state);

    bool isPlayRestingFinishSound() const;
    void setPlayRestingFinishSound(bool state);

    bool isShowWorkingFinishedTrayNotify() const;
    void setShowWorkingFinishedTrayNotify(bool state);

    bool isShowRestingFinishedTrayNotify() const;
    void setShowRestingFinishedTrayNotify(bool state);

    bool isSaveChangesOnExit() const;
    void setSaveChangesOnExit(bool state);

private:
    Ui::SettingsDialog *ui;
};


#endif // SETTINGSDIALOG_H
