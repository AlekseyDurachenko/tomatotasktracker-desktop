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
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "version.h"
#include "resources.h"
#include <QFile>


AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("About %1").arg(appShortName()));

    // application name font
    QFont appNameFont = font();
    appNameFont.setPixelSize(20);
    appNameFont.setBold(true);

    // normal text font
    QFont appInfoFont = font();
    appInfoFont.setPixelSize(14);

    // set application name
    ui->programmTitle_label->setFont(appNameFont);
    ui->programmTitle_label->setText(appName());

    // application version
    QLabel *versionLabel = new QLabel(this);
    versionLabel->setFont(appInfoFont);
#ifdef APP_REVISION
    versionLabel->setText(tr("<b>Version:</b> %1 (rev: %2)")
                          .arg(appVersion(), appRevision()));
#else
    versionLabel->setText(tr("<b>Version:</b> %1").arg(appVersion()));
#endif
    ui->headLayout->addWidget(versionLabel);

    // application build number
#ifdef APP_BUILD_NUMBER
    QLabel *buildNumberLabel = new QLabel(this);
    buildNumberLabel->setFont(appInfoFont);
    buildNumberLabel->setText(tr("<b>Build number:</b> %1").arg(appBuildNumber()));
    ui->headLayout->addWidget(buildNumberLabel);
#endif

    // application build date
#ifdef APP_BUILD_DATE
    QLabel *buildDateLabel = new QLabel(this);
    buildDateLabel->setFont(appInfoFont);
    buildDateLabel->setText(tr("<b>Build date:</b> %1").arg(appBuildDate()));
    ui->headLayout->addWidget(buildDateLabel);
#endif

    ui->headLayout->addItem(new QSpacerItem(1, 1,
                                            QSizePolicy::Expanding,
                                            QSizePolicy::Expanding));

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
    QFile lcLext(docFileName(resourceName) + "_" + QLocale::system().name());
    if (lcLext.open(QIODevice::ReadOnly)) {
        return QString::fromUtf8(lcLext.readAll());
    }

    QFile lLext(docFileName(resourceName) + "_" + QLocale::system().name().split("_").first());
    if (lLext.open(QIODevice::ReadOnly)) {
        return QString::fromUtf8(lLext.readAll());
    }

    QFile text(docFileName(resourceName));
    if (text.open(QIODevice::ReadOnly)) {
        return QString::fromUtf8(text.readAll());
    }

    return tr("Can't open file: %1").arg(docFileName(resourceName));
}
