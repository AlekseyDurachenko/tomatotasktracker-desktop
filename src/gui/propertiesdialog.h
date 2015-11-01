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
#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H


#include <QDialog>


namespace Ui {
class PropertiesDialog;
}

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesDialog(QWidget *parent = 0);
    virtual ~PropertiesDialog();

    int workingTime() const;
    void setWorkingTime(int seconds);

    int restingTime() const;
    void setRestingTime(int seconds);

private slots:
    void updateAcceptedButton();

private:
    Ui::PropertiesDialog *ui;
};


#endif // PROPERTIESDIALOG_H
