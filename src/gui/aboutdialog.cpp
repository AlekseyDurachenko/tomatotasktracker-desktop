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
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "settings.h"
#include "resources.h"
#include <QFile>


AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("About %1").arg(appShortName()));

    ui->programmTitle_label->setText(QString("%1 - %2").arg(appShortName(), appVersion()));

    ui->about_plainTextEdit->setPlainText(fromFile("ABOUT"));
    ui->authors_plainTextEdit->setPlainText(fromFile("AUTHORS"));
    ui->changelog_plainTextEdit->setPlainText(fromFile("CHANGELOG"));
    ui->license_plainTextEdit->setPlainText(fromFile("LICENSE"));
    ui->libraries_plainTextEdit->setPlainText(fromFile("LIBRARIES"));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::setPixmap(const QPixmap &pixmap)
{
    ui->icon_label->setPixmap(pixmap);
}

QString AboutDialog::fromFile(const QString &resourceName)
{
    QFile text(docResFileName(resourceName));
    if (text.open(QIODevice::ReadOnly))
        return QString::fromUtf8(text.readAll());

    return tr("Can't open file: %1").arg(docResFileName(resourceName));
}
