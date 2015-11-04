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
#include "playsound.h"
#include "resources.h"
#ifdef PLATFORM_USING_PHONON
    #include <phonon/phonon>
#elif PLATFORM_USING_MULTIMEDIA
    #include <QMediaPlayer>
#elif PLATFORM_USING_QSOUND
    #include <QSound>
#endif


void playSound(const QString &fileName)
{
#ifdef PLATFORM_USING_PHONON
    Phonon::MediaObject *notify = Phonon::createPlayer(Phonon::NotificationCategory);
    notify->setCurrentSource(QUrl::fromLocalFile(fileName));
    notify->play();
    QObject::connect(notify, SIGNAL(finished()), notify, SLOT(deleteLater()));
#elif PLATFORM_USING_MULTIMEDIA
    QMediaPlayer *notify = new QMediaPlayer;
    notify->setMedia(QUrl::fromLocalFile(fileName));
    notify->setVolume(100);
    notify->play();
    QObject::connect(notify, &QMediaPlayer::stateChanged, [=](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState) {
            notify->deleteLater();
        }
    });
#elif PLATFORM_USING_QSOUND
    QSound notify(fileName);
    notify.play();
#endif
}
