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
#include "timestr.h"
#include "project.h"
#include <QLabel>
#include <QToolButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>


TomatoWidget::TomatoWidget(Project *project, QWidget *parent)
    : QWidget(parent)
    , m_project(project)
{
    m_startToolButton = new QToolButton(this);
    m_startToolButton->setIconSize(QSize(64, 64));
    m_startToolButton->setAutoRaise(true);
    m_startToolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    m_stopToolButton = new QToolButton(this);
    m_stopToolButton->setIconSize(QSize(64, 64));
    m_stopToolButton->setAutoRaise(true);
    m_stopToolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont timeLabelFont = m_timeLabel->font();
    timeLabelFont.setPointSize(36);
    m_timeLabel->setFont(timeLabelFont);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont statusLabelFont = m_timeLabel->font();
    statusLabelFont.setPointSize(14);
    statusLabelFont.setBold(true);
    m_statusLabel->setFont(statusLabelFont);

    m_statusFrame = new QFrame(this);
    m_statusFrame->setFrameShape(QFrame::WinPanel);
    m_statusFrame->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(m_timeLabel);
    vbox->addWidget(m_statusLabel);
    m_statusFrame->setLayout(vbox);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_statusFrame);
    hbox->addWidget(m_startToolButton);
    hbox->addWidget(m_stopToolButton);
    setLayout(hbox);

    connect(m_project->tomato(), SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateAll()));
    connect(m_project->tomato(), SIGNAL(workingTimeChanged()),
            this, SLOT(updateAll()));
    connect(m_project->tomato(), SIGNAL(restingTimeChanged()),
            this, SLOT(updateAll()));
    connect(m_project->tomato(), SIGNAL(reseted()),
            this, SLOT(updateAll()));
    connect(m_project->tomato(), SIGNAL(timeSyncTimeout()),
            this, SLOT(updateAll()));

    m_startToolButton->setDefaultAction(m_project->startAction());
    m_stopToolButton->setDefaultAction(m_project->stopAction());

    updateAll();
}

void TomatoWidget::updateAll()
{
    switch (m_project->tomato()->state()) {
    case Tomato::Idle:
        m_statusFrame->setStyleSheet("");
        break;
    case Tomato::Working:
        m_statusFrame->setStyleSheet("background-color: red;");
        break;
    case Tomato::OverWorking:
        m_statusFrame->setStyleSheet("background-color: yellow;");
        break;
    case Tomato::Resting:
        m_statusFrame->setStyleSheet("background-color: lightGreen;");
        break;
    case Tomato::OverResting:
        m_statusFrame->setStyleSheet("background-color: yellow;");
        break;
    };

    qint64 time = 0;
    switch (m_project->tomato()->state()) {
    case Tomato::Idle:
        time = m_project->tomato()->workingTime();
        break;
    case Tomato::Working:
        time = m_project->tomato()->workingTime()
               - m_project->tomato()->calcTomatoTime();
        break;
    case Tomato::OverWorking:
        time = m_project->tomato()->calcTomatoTime()
               - m_project->tomato()->workingTime();
        break;
    case Tomato::Resting:
        time = m_project->tomato()->restingTime()
               - m_project->tomato()->calcTomatoTime();
        break;
    case Tomato::OverResting:
        time = m_project->tomato()->calcTomatoTime()
               - m_project->tomato()->restingTime();
        break;
    }

    m_timeLabel->setText(secsToTimeStr(time));
    m_statusLabel->setText(Tomato::stateName(m_project->tomato()->state()));
}
