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
#ifndef GETOPENFILENAMEWIDGET_H
#define GETOPENFILENAMEWIDGET_H


#include <QWidget>
class QLineEdit;
class QToolButton;


class GetOpenFileNameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GetOpenFileNameWidget(QWidget *parent = 0);

    QString fileName() const;
    bool exists() const;

    inline const QString &fileMask() const;
    void setFileMask(const QString &mask);

    inline const QString &settingsKey() const;
    void setSettingsKey(const QString &key);

signals:
    void fileNameChanged(const QString &fileName);
    void fileNameChanged(bool exists);

public slots:
    void setFileName(const QString &fileName);

private slots:
    void on_toolButton_clicked();
    void on_lineEdit_textChanged(const QString &text);

private:
    QLineEdit *m_lineEdit;
    QToolButton *m_toolButton;
    QString m_fileMask;
    QString m_settingsKey;
};

const QString &GetOpenFileNameWidget::fileMask() const
{
    return m_fileMask;
}

const QString &GetOpenFileNameWidget::settingsKey() const
{
    return m_settingsKey;
}


#endif // GETOPENFILENAMEWIDGET_H
