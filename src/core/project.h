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
#ifndef PROJECT_H
#define PROJECT_H


#include "tomato.h"
#include <QAction>


class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);

    inline Tomato *tomato() const;

    inline QAction *newAction() const;
    inline QAction *openAction() const;
    inline QAction *saveAction() const;
    inline QAction *saveAsAction() const;
    inline QAction *closeAction() const;

    inline QAction *propertiesAction() const;

    inline QAction *startAction() const;
    inline QAction *stopAction() const;

    inline bool isOpen() const;
    inline bool hasChanges() const;
    inline const QString &fileName() const;

    bool create(const QString &fileName, QString *reason = 0);
    bool open(const QString &fileName, QString *reason = 0);
    bool save(QString *reason = 0);
    bool saveAs(const QString &fileName, QString *reason = 0);
    void close();

signals:
    void saveStateChanged();
    void openStateChanged();

private slots:
    void savingDataChanged();
    void updateActions();
    void syncTimeout();

private:
    void createActions();

private:
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_closeAction;

    QAction *m_propertiesAction;

    QAction *m_startAction;
    QAction *m_stopAction;

    Tomato *m_tomato;

    bool m_hasChanges;
    QString m_fileName;
};

Tomato *Project::tomato() const
{
    return m_tomato;
}

QAction *Project::newAction() const
{
    return m_newAction;
}

QAction *Project::openAction() const
{
    return m_openAction;
}

QAction *Project::saveAction() const
{
    return m_saveAction;
}

QAction *Project::saveAsAction() const
{
    return m_saveAsAction;
}

QAction *Project::closeAction() const
{
    return m_closeAction;
}

QAction *Project::propertiesAction() const
{
    return m_propertiesAction;
}

QAction *Project::startAction() const
{
    return m_startAction;
}

QAction *Project::stopAction() const
{
    return m_stopAction;
}

bool Project::isOpen() const
{
    return (!m_fileName.isEmpty());
}

bool Project::hasChanges() const
{
    return m_hasChanges;
}

const QString &Project::fileName() const
{
    return m_fileName;
}


#endif // PROJECT_H
