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
#ifndef RESOURCES_H
#define RESOURCES_H


#include <QString>


QString resPath();

QString docsPath();
QString soundsPath();
QString langsPath();
QString iconPath();

QString docFileName(const QString &fileName);
QString soundFileName(const QString &fileName);
QString langFileName(const QString &fileName);
QString iconFileName(const QString &fileName);


#endif // RESOURCES_H
