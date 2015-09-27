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


#include "tomato.h"
#include <QWidget>


namespace Ui {
class TomatoWidget;
}

class TomatoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TomatoWidget(QWidget *parent = 0, Tomato *tomato = 0);
    virtual ~TomatoWidget();

    void setTaskManager(Tomato *tomato);
    void setTomatoStartAction(QAction *tomatoStartAction);
    void setTomatoStopAction(QAction *tomatoStopAction);
public slots:
    void updateTomatoStatus();
private:
    Ui::TomatoWidget *ui;
    Tomato *m_tomato;
};


#endif // TOMATOWIDGET_H
