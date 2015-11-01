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
#include "tasktimeeditdialog.h"
#include "ui_tasktimeeditdialog.h"
#include "consts.h"
#include <QMessageBox>


TaskTimeEditDialog::TaskTimeEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TaskTimeEditDialog)
{
    ui->setupUi(this);
    ui->startTime_dateTimeEdit->setDisplayFormat(DisplayTaskTimeFormat);
    ui->endTime_dateTimeEdit->setDisplayFormat(DisplayTaskTimeFormat);

    setStartDateTime(QDateTime::currentDateTime());
    setEndDateTime(QDateTime::currentDateTime());
}

TaskTimeEditDialog::~TaskTimeEditDialog()
{
    delete ui;
}

QDateTime TaskTimeEditDialog::startDateTime() const
{
    return ui->startTime_dateTimeEdit->dateTime();
}

void TaskTimeEditDialog::setStartDateTime(const QDateTime &dateTime)
{
    ui->startTime_dateTimeEdit->setDateTime(dateTime);
}

QDateTime TaskTimeEditDialog::endDateTime() const
{
    return ui->endTime_dateTimeEdit->dateTime();
}

void TaskTimeEditDialog::setEndDateTime(const QDateTime &dateTime)
{
    ui->endTime_dateTimeEdit->setDateTime(dateTime);
}

void TaskTimeEditDialog::accept()
{
    if (startDateTime() < endDateTime()) {
        QDialog::accept();
    } else {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Please enter a correct time interval"));
    }
}
