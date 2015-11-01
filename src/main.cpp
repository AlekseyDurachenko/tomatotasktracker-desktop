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
#include "theme.h"
#include "version.h"
#include "resources.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    // Qt translator
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    // application translator
    QTranslator myappTranslator;
    myappTranslator.load(appName() + QString("_") + QLocale::system().name(),
                         languagesPath());
    app.installTranslator(&myappTranslator);


    theme::init();


    MainWindow window;
    window.show();
    int ret = app.exec();


    return ret;
}
