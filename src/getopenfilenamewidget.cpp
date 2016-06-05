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
#include "getopenfilenamewidget.h"
#include "settings.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSettings>
#include <QToolButton>


GetOpenFileNameWidget::GetOpenFileNameWidget(QWidget *parent)
    : QWidget(parent)
{
    m_lineEdit = new QLineEdit(this);
    connect(m_lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(on_lineEdit_textChanged(QString)));

    m_toolButton = new QToolButton(this);
    m_toolButton->setText("...");
    connect(m_toolButton, SIGNAL(clicked()),
            this, SLOT(on_toolButton_clicked()));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_lineEdit);
    hbox->addWidget(m_toolButton);
    hbox->setContentsMargins(0, 0, 0, 0);
    setLayout(hbox);

    m_fileMask = tr("All files (* *.*)");
    m_settingsKey = "";
}

QString GetOpenFileNameWidget::fileName() const
{
    return m_lineEdit->text();
}

bool GetOpenFileNameWidget::exists() const
{
    return QFileInfo(fileName()).exists();
}

void GetOpenFileNameWidget::setFileMask(const QString &mask)
{
    m_fileMask = mask;
}

void GetOpenFileNameWidget::setSettingsKey(const QString &key)
{
    m_settingsKey = key;
}

void GetOpenFileNameWidget::setFileName(const QString &fileName)
{
    m_lineEdit->setText(fileName);
}

void GetOpenFileNameWidget::on_toolButton_clicked()
{
    QString path;
    if (!m_settingsKey.isEmpty()) {
        path = appSettings().value(m_settingsKey, "").toString();
    }
    else if (!fileName().isEmpty()) {
        path = QFileInfo(fileName()).absolutePath();
    }

    QString fn = QFileDialog::getOpenFileName(
                     this, tr("Open file"), path, m_fileMask);

    if (!fn.isEmpty()) {
        if (!m_settingsKey.isEmpty()) {
            appSettings().setValue(m_settingsKey, QFileInfo(fn).absolutePath());
        }

        m_lineEdit->setText(fn);
    }
}

void GetOpenFileNameWidget::on_lineEdit_textChanged(const QString &)
{
    bool fileExist = exists();
    if (!fileExist) {
        QPalette palette = m_lineEdit->palette();
        palette.setColor(QPalette::Text, Qt::red);
        m_lineEdit->setPalette(palette);
    }
    else {
        m_lineEdit->setPalette(QPalette());
    }

    emit fileNameChanged(fileName());
    emit fileNameChanged(fileExist);
}
