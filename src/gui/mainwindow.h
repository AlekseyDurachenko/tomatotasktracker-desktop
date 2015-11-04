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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QSystemTrayIcon>
#include "project.h"
class SystemTrayIcon;
class TaskView;
class TomatoWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private slots:
    void quitAction_triggered();
    void settingsAction_triggered();
    void aboutAction_triggered();
    void aboutQtAction_triggered();
    void project_newAction_triggered();
    void project_openAction_triggered();
    void project_saveAction_triggered();
    void project_saveAsAction_triggered();
    void project_closeAction_triggered();
    void project_propertiesAction_triggered();
    void project_startAction_triggered();
    void project_stopAction_triggered();
    void trayIcon_activated(QSystemTrayIcon::ActivationReason reason);
    void tomatoStateChanged(Tomato::State state);

    void updateWindowTitle();
    void updateEnableState();
    void updateSaveChangesTimer();

    void trySaveProjectOnExit();
    void trySaveProjectOnPeriodically();

protected:
    void closeEvent(QCloseEvent *closeEvent);

private:
    void saveSettings();
    void loadSettings();
    void loadLastProject();

    void createProject();
    void createActions();
    void createSystemTrayIcon();
    void createTaskView();
    void createTomatoWidget();
    void createWindowMenu();
    void createStatusBar();
    void createSaveChangesTimer();

private:
    QAction *m_quitAction;
    QAction *m_settingsAction;
    QAction *m_aboutAction;
    QAction *m_aboutQtAction;

    SystemTrayIcon *m_trayIcon;
    TaskView *m_taskView;
    TomatoWidget *m_tomatoWidget;
    QStatusBar *m_statusBar;
    QTimer *m_saveChangesTimer;

    Project *m_project;
};


#endif // MAINWINDOW_H
