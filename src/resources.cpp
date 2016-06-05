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
#include "resources.h"
#include <QCoreApplication>
#include <QDir>


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


QString resPath()
{
#if defined(APP_PORTABLE) || !defined(APP_RESOURCES_PREFIX)
    return QCoreApplication::applicationDirPath();
#else
    return STR(APP_RESOURCES_PREFIX);
#endif
}

QString docsPath()
{
    return resPath();
}

QString soundsPath()
{
    return resPath() + QDir::separator() + "sounds";
}

QString langsPath()
{
    return resPath() + QDir::separator() + "translations";
}

QString iconPath()
{
    return resPath() + QDir::separator() + "icons";
}

QString docFileName(const QString &fileName)
{
    return docsPath() + QDir::separator() + fileName;
}

QString soundFileName(const QString &fileName)
{
    return soundsPath() + QDir::separator() + fileName;
}

QString langFileName(const QString &fileName)
{
    return langsPath() + QDir::separator() + fileName;
}

QString iconFileName(const QString &fileName)
{
    return iconPath() + QDir::separator() + fileName;
}
