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
#ifndef TOMATOWIDGET_H
#define TOMATOWIDGET_H


#include <QWidget>
class Project;
class QToolButton;
class QLabel;
class QFrame;


class TomatoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TomatoWidget(Project *project, QWidget *parent = 0);

private slots:
    void updateAll();

private:
    QToolButton *m_startToolButton;
    QToolButton *m_stopToolButton;
    QLabel *m_timeLabel;
    QLabel *m_statusLabel;
    QFrame *m_statusFrame;
    Project *m_project;
};


#endif // TOMATOWIDGET_H
