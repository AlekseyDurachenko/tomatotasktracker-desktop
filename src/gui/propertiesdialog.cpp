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
#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"
#include <QPushButton>


PropertiesDialog::PropertiesDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::PropertiesDialog)
{    
    ui->setupUi(this);

    setWindowTitle(tr("Project properties"));

    connect(ui->workingHour_spinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateAcceptedButton()));
    connect(ui->workingMin_spinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateAcceptedButton()));
    connect(ui->workingSec_spinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateAcceptedButton()));

    connect(ui->restingHour_spinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateAcceptedButton()));
    connect(ui->restingMin_spinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateAcceptedButton()));
    connect(ui->restingSec_spinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateAcceptedButton()));

    updateAcceptedButton();
}

PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}

qint64 PropertiesDialog::workingTime() const
{
    return ui->workingHour_spinBox->value() * 3600
            + ui->workingMin_spinBox->value() * 60
            + ui->workingSec_spinBox->value();
}

void PropertiesDialog::setWorkingTime(qint64 seconds)
{
    ui->workingHour_spinBox->setValue(seconds / 3600);
    ui->workingMin_spinBox->setValue((seconds % 3600) / 60);
    ui->workingSec_spinBox->setValue(seconds % 60);
}

qint64 PropertiesDialog::restingTime() const
{
    return ui->restingHour_spinBox->value() * 3600
            + ui->restingMin_spinBox->value() * 60
            + ui->restingSec_spinBox->value();
}

void PropertiesDialog::setRestingTime(qint64 seconds)
{
    ui->restingHour_spinBox->setValue(seconds / 3600);
    ui->restingMin_spinBox->setValue((seconds % 3600) / 60);
    ui->restingSec_spinBox->setValue(seconds % 60);
}

void PropertiesDialog::updateAcceptedButton()
{
    if (workingTime() > 0 && restingTime() > 0)
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}
