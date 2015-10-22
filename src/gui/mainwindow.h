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

#include "tomato.h"
#include "project.h"
#include "taskitemmodel.h"
#include "systemtrayicon.h"
#include <QMainWindow>
class TaskFilterProxyModel;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private slots:
    void on_new_action_triggered();
    void on_open_action_triggered();
    void on_save_action_triggered();
    void on_saveAs_action_triggered();
    void on_close_action_triggered();
    void on_properties_action_triggered();
    void on_quit_action_triggered();

    void on_startTomato_action_triggered();
    void on_stopTomato_action_triggered();

    void on_activateTask_action_triggered();
    void on_addTask_action_triggered();
    void on_editTask_action_triggered();
    void on_removeTask_action_triggered();
    void on_removeAllTasks_action_triggered();
    void on_hideFinishedTasks_action_toggled(bool hide);
    void on_expandAllTasks_action_triggered();
    void on_collapseAllTasks_action_triggered();

    void on_settings_action_triggered();

    void on_about_action_triggered();
    void on_aboutQt_action_triggered();

    void on_task_treeView_customContextMenuRequested(const QPoint &pos);
    void on_task_treeView_activated(const QModelIndex &index);

    void trayIcon_activated(QSystemTrayIcon::ActivationReason reason);

protected:
    void closeEvent(QCloseEvent *closeEvent);

private slots:
    void updateWindowTitle();
    void updateProjectActions();
    void updateTomatoActions();
    void updateTaskActions();
    void playSound(Tomato::State state);

private slots:
    void updateSystemTrayIcon();

private:
    void createSystemTrayIcon();

private:
    Ui::MainWindow *ui;
    TaskFilterProxyModel *m_taskFilterProxyModel;
    QTimer *m_tomatoTimer;
    Project *m_project;
    Tomato *m_tomato;

    SystemTrayIcon *m_trayIcon;
};


#endif // MAINWINDOW_H
