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
#ifndef TASKTIMEEDITDIALOG_H
#define TASKTIMEEDITDIALOG_H


#include <QDateTime>
#include <QDialog>


namespace Ui {
class TaskTimeEditDialog;
}

class TaskTimeEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskTimeEditDialog(QWidget *parent = 0);
    virtual ~TaskTimeEditDialog();

    QDateTime startDateTime() const;
    void setStartDateTime(const QDateTime &dateTime);

    QDateTime endDateTime() const;
    void setEndDateTime(const QDateTime &dateTime);

protected:
    virtual void accept();

private:
    Ui::TaskTimeEditDialog *ui;
};


#endif // TASKTIMEEDITDIALOG_H
