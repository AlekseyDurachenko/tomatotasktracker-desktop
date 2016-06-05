// Copyright 2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include <QMainWindow>
#include <QModelIndex>
#include <QSystemTrayIcon>
class Project;
class TaskItemFilterProxyModel;
class TaskItemModel;
class QFrame;
class QLabel;
class QToolButton;
class QTreeView;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private slots:
    void newAction_triggered();
    void openAction_triggered();
    void saveAction_triggered();
    void saveAsAction_triggered();
    void closeAction_triggered();
    void propertiesAction_triggered();
    void quitAction_triggered();

    void startAction_triggered();
    void stopAction_triggered();

    void activateAction_triggered();
    void addAction_triggered();
    void editAction_triggered();
    void removeAction_triggered();
    void removeAllAction_triggered();
    void hideCompletedAction_toggled(bool hide);
    void expandAllAction_triggered();
    void collapseAllAction_triggered();

    void settingsAction_triggered();

    void aboutAction_triggered();
    void aboutQtAction_triggered();

    void updateStartStopAction();
    void updateWindowTitle();
    void updateLayout();
    void updateActions();
    void updateStatusLabels();
    void updateSystemTrayIcon();
    void updateSystemTrayStatusAction();
    void updateSaveTimer();
    void updateSyncTimer();

    void taskView_customContextMenu(QPoint pos);
    void taskView_itemActivated(const QModelIndex &index);

    void trayIcon_activated(QSystemTrayIcon::ActivationReason reason);

    void saveProjectOnExit();
    void saveProjectOnTimer();

    void notifyStateChanged(Tomato::State state);

private:
    void readSettings();
    void writeSettings();

    void readCommandLineArguments();
    void loadLastProject();

    void createProject();
    void createActions();
    void createMenu();
    void createStatusBar();
    void createSystemTrayIcon();
    void createLayout();
    void createSaveTimer();
    void createSyncTimer();

    QString tomatoStatusText() const;
    QString tomatoTimeText() const;

protected:
    virtual void closeEvent(QCloseEvent *closeEvent);

private:
    Project *m_project;

    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_closeAction;
    QAction *m_propertiesAction;
    QAction *m_quitAction;

    QAction *m_startAction;
    QAction *m_stopAction;

    QAction *m_activateAction;
    QAction *m_addAction;
    QAction *m_editAction;
    QAction *m_removeAction;
    QAction *m_removeAllAction;
    QAction *m_hideCompletedAction;
    QAction *m_expandAllAction;
    QAction *m_collapseAllAction;

    QAction *m_settingsAction;

    QAction *m_aboutAction;
    QAction *m_aboutQtAction;

    QSystemTrayIcon *m_trayIcon;
    QAction *m_statusAction;

    TaskItemModel *m_taskItemModel;
    TaskItemFilterProxyModel *m_taskItemFilterProxyModel;
    QTreeView *m_taskView;

    QToolButton *m_startToolButton;
    QToolButton *m_stopToolButton;
    QLabel *m_timeLabel;
    QLabel *m_statusLabel;
    QFrame *m_statusFrame;

    QTimer *m_saveTimer;
    QTimer *m_syncTimer;
};


#endif // MAINWINDOW_H
