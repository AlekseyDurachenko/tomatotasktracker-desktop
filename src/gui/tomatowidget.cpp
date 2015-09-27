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
#include "tomatowidget.h"
#include "ui_tomatowidget.h"
#include "utils.h"


TomatoWidget::TomatoWidget(QWidget *parent, Tomato *tomato) :
    QWidget(parent), ui(new Ui::TomatoWidget)
{
    ui->setupUi(this);
    setTaskManager(tomato);
}

TomatoWidget::~TomatoWidget()
{
    delete ui;
}

void TomatoWidget::setTaskManager(Tomato *tomato)
{
    m_tomato = tomato;
    if (m_tomato) {
        connect(m_tomato, SIGNAL(stateChanged(Tomato::State)),
                this, SLOT(updateTomatoStatus()));
        connect(m_tomato, SIGNAL(workingTimeChanged()),
                this, SLOT(updateTomatoStatus()));
        connect(m_tomato, SIGNAL(restingTimeChanged()),
                this, SLOT(updateTomatoStatus()));
        connect(m_tomato, SIGNAL(reseted()),
                this, SLOT(updateTomatoStatus()));
    }

    updateTomatoStatus();
}

void TomatoWidget::setTomatoStartAction(QAction *tomatoStartAction)
{
    ui->tomatoStart_toolButton->setDefaultAction(tomatoStartAction);
}

void TomatoWidget::setTomatoStopAction(QAction *tomatoStopAction)
{
    ui->tomatoStop_toolButton->setDefaultAction(tomatoStopAction);
}

void TomatoWidget::updateTomatoStatus()
{
    if (!m_tomato) {
        ui->tomatoTime_label->clear();
        ui->tomatoStatus_label->clear();
        ui->tomatoStatus_frame->setBackgroundRole(QPalette::Window);
        return;
    }

    switch (m_tomato->state()) {
    case Tomato::Idle:
        ui->tomatoStatus_frame->setStyleSheet("");
        break;
    case Tomato::Working:
        ui->tomatoStatus_frame->setStyleSheet("background-color: yellow;");
        break;
    case Tomato::OverWorking:
        ui->tomatoStatus_frame->setStyleSheet("background-color: red;");
        break;
    case Tomato::Resting:
        ui->tomatoStatus_frame->setStyleSheet("background-color: lightGreen;");
        break;
    case Tomato::OverResting:
        ui->tomatoStatus_frame->setStyleSheet("background-color: yellow;");
        break;
    };

    qint64 time;
    switch (m_tomato->state()) {
    case Tomato::Idle:
        time = m_tomato->workingTime();
        break;
    case Tomato::Working:
        time = m_tomato->workingTime() - m_tomato->calcTomatoTime();
        break;
    case Tomato::OverWorking:
        time = m_tomato->calcTomatoTime() - m_tomato->workingTime();
        break;
    case Tomato::Resting:
        time = m_tomato->restingTime() - m_tomato->calcTomatoTime();
        break;
    case Tomato::OverResting:
        time = m_tomato->calcTomatoTime() - m_tomato->restingTime();
        break;
    }

    ui->tomatoTime_label->setText(secsToTimeStr(time));
    ui->tomatoStatus_label->setText(m_tomato->stateText());
}
