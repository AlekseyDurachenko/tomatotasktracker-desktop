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


class Project : public QObject
{
    Q_OBJECT
public:
    Project(Tomato *tomato, QObject *parent = 0);

    inline bool isOpen() const;
    inline bool hasChanges() const;
    inline const QString &fileName() const;

    bool create(const QString &fileName, QString *reason = 0);
    bool open(const QString &fileName, QString *reason = 0);
    bool save(QString *reason = 0);
    bool saveAs(const QString &fileName, QString *reason = 0);
    void close();
signals:
    void changed();
private slots:
    void tomatoSavingDataChanged();
private:
    Tomato *m_tomato;

    bool m_hasChanges;
    QString m_fileName;
};

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
