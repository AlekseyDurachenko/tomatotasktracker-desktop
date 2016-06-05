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
#include "mainwindow.h"
#include "resources.h"
#include "theme.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>


void installTranslators(QCoreApplication *app)
{
    // Qt translator
    static QTranslator qtTr;
    qtTr.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app->installTranslator(&qtTr);

    // application translator
    static QTranslator myTr;
    myTr.load(QLocale::system().name(), langsPath());
    app->installTranslator(&myTr);
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    installTranslators(&app);
    theme::init();

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
