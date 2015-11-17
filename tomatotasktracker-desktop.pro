# Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
TEMPLATE        = app
TARGET          = tomatotasktracker-desktop
APP_MAJOR       = 0
APP_MINOR       = 1
APP_PATCH       = 1
VERSION         = "$${APP_MAJOR}.$${APP_MINOR}.$${APP_PATCH}"
CONFIG          += console debug_and_release
QT              += core gui xml
greaterThan(QT_MAJOR_VERSION, 4) {
    QT          += widgets

    unix {
        QT      += multimedia
        DEFINES += PLATFORM_USING_MULTIMEDIA
    }
    win32 {
        DEFINES += PLATFORM_USING_QSOUND
    }
}
else {
    unix {
        QT      += phonon
        DEFINES += PLATFORM_USING_PHONON
    }
    win32 {
        DEFINES += PLATFORM_USING_QSOUND
    }
}


# enable c++11 features
QMAKE_CXXFLAGS += -std=c++11


# application version for using inside source code
DEFINES += "APP_NAME=$${TARGET}"
DEFINES += "APP_MAJOR=$${APP_MAJOR}"
DEFINES += "APP_MINOR=$${APP_MINOR}"
DEFINES += "APP_PATCH=$${APP_PATCH}"

# if you want specify the application build number you can use:
# qmake "DEFINES += APP_BUILD_NUMBER=2"
# if you want specify the application build date you can use:
# qmake "DEFINES += APP_BUILD_DATE='2015-11-15T09:29:26+0000'"
# if you want specify the application VCS revision you can use:
# qmake "DEFINES += APP_REVISION='123456'"
# if you want specify the source code link you can use:
# qmake "DEFINES += APP_SOURCES='https://github.com/path/to/project'"
# if you want specify application version you can use:
# qmake "DEFINES += APP_VERSION='0.1.0-5-g27626de'"
C = $$find(DEFINES, "APP_VERSION=")
isEmpty(C) {
    DEFINES += "APP_VERSION='$${APP_MAJOR}.$${APP_MINOR}.$${APP_PATCH}'"
}


# uncomment this line if you want to build the test
# or use: qmake "QT += testlib"
#QT      += testlib


# uncomment this line if you want to buid portable application
# or use: qmake "DEFINES += APP_PORTABLE"
#DEFINES += APP_PORTABLE


# uncomment this line if you want to deploy the application
# or use: qmake "DEFINES += APP_DEPLOY"
#DEFINES += APP_DEPLOY


# release build options
release:DESTDIR     = bin
release:OBJECTS_DIR = build/release/obj
release:MOC_DIR     = build/release/moc
release:RCC_DIR     = build/release/rcc
release:UI_DIR      = build/release/ui
# debug build options
debug:DESTDIR       = bin
debug:OBJECTS_DIR   = build/debug/obj
debug:MOC_DIR       = build/debug/moc
debug:RCC_DIR       = build/debug/rcc
debug:UI_DIR        = build/debug/ui


# current project (.pri)
include($$PWD/tomatotasktracker-desktop-sources.pri)
include($$PWD/tomatotasktracker-desktop-tests.pri)
include($$PWD/tomatotasktracker-desktop-unixinstall.pri)
include($$PWD/tomatotasktracker-desktop-unixrules.pri)


# the 3rd libraries (.pri)
