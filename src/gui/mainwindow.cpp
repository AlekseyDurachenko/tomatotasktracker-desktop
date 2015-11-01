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
#include "mainwindow.h"
#include "consts.h"
#include "version.h"
#include "settings.h"
#include "theme.h"
#include "resources.h"
#include "propertiesdialog.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "systemtrayicon.h"
#include "taskview.h"
#include "tomatowidget.h"
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QHeaderView>
#include <QApplication>
#include <QVBoxLayout>
#include <phonon/phonon>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(theme::icon(theme::IconApp));
    resize(400, 500);

    createProject();
    createActions();
    createSystemTrayIcon();
    createTaskView();
    createTomatoWidget();
    createWindowMenu();
    createStatusBar();

    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_taskView);
    layout->addWidget(m_tomatoWidget);
    widget->setLayout(layout);
    setCentralWidget(widget);

    loadSettings();
    loadLastProject();

    updateWindowTitle();
    updateEnableState();
}

MainWindow::~MainWindow()
{
    trySaveProjectOnExit();
    saveSettings();
}

void MainWindow::saveSettings()
{
    G_SETTINGS_INIT();

    settings.setValue(SettingsLastProject, m_project->fileName());
    settings.setValue(SettingsMainWindowGeometry, saveGeometry());
    settings.setValue(SettingsMainWindowState, saveState());
    settings.setValue(SettingsTaskViewHeaderState, m_taskView->header()->saveState());
    settings.setValue(SettingsTaskViewHideCompleted, m_taskView->hideCompletedAction()->isChecked());
}

void MainWindow::loadSettings()
{
    G_SETTINGS_INIT();

    restoreGeometry(settings.value(SettingsMainWindowGeometry, saveGeometry()).toByteArray());
    restoreState(settings.value(SettingsMainWindowState, saveState()).toByteArray());
    m_taskView->header()->restoreState(settings.value(SettingsTaskViewHeaderState, m_taskView->header()->saveState()).toByteArray());
    m_taskView->hideCompletedAction()->setChecked(settings.value(SettingsTaskViewHideCompleted, m_taskView->hideCompletedAction()->isChecked()).toBool());
}

void MainWindow::trySaveProjectOnExit()
{
    G_SETTINGS_INIT();

    if (m_project->hasChanges()
            && settings.value(SettingsSaveChangesOnExit, true).toBool()) {
        QString errorString;
        if (!m_project->save(&errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
        }
    }
}

void MainWindow::loadLastProject()
{
    G_SETTINGS_INIT();

    QString projectFileName = settings.value(SettingsLastProject, "").toString();
    if (!projectFileName.isEmpty()) {
        QString errorString;
        if (!m_project->open(projectFileName, &errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
        }
    }
}

void MainWindow::createProject()
{
    m_project = new Project(this);
    connect(m_project->newAction(), SIGNAL(triggered(bool)),
            this, SLOT(project_newAction_triggered()));
    connect(m_project->openAction(), SIGNAL(triggered(bool)),
            this, SLOT(project_openAction_triggered()));
    connect(m_project->saveAction(), SIGNAL(triggered(bool)),
            this, SLOT(project_saveAction_triggered()));
    connect(m_project->saveAsAction(), SIGNAL(triggered(bool)),
            this, SLOT(project_saveAsAction_triggered()));
    connect(m_project->closeAction(), SIGNAL(triggered(bool)),
            this, SLOT(project_closeAction_triggered()));
    connect(m_project->propertiesAction(), SIGNAL(triggered(bool)),
            this, SLOT(project_propertiesAction_triggered()));
    connect(m_project->startAction(), SIGNAL(triggered(bool)),
            this, SLOT(project_startAction_triggered()));
    connect(m_project->stopAction(), SIGNAL(triggered(bool)),
            this, SLOT(project_stopAction_triggered()));

    connect(m_project, SIGNAL(openStateChanged()),
            this, SLOT(updateWindowTitle()));
    connect(m_project, SIGNAL(openStateChanged()),
            this, SLOT(updateEnableState()));

    connect(m_project->tomato(), SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(tomatoStateChanged(Tomato::State)));
}

void MainWindow::createActions()
{
    m_quitAction = new QAction(this);
    m_quitAction->setText(tr("&Quit..."));
    m_quitAction->setIcon(theme::icon(theme::IconActionQuit));
    connect(m_quitAction, SIGNAL(triggered(bool)),
            this, SLOT(quitAction_triggered()));

    m_settingsAction = new QAction(this);
    m_settingsAction->setText(tr("&Settings..."));
    m_settingsAction->setIcon(theme::icon(theme::IconActionSettings));
    connect(m_settingsAction, SIGNAL(triggered(bool)),
            this, SLOT(settingsAction_triggered()));

    m_aboutAction = new QAction(this);
    m_aboutAction->setText(tr("&About..."));
    m_aboutAction->setIcon(theme::icon(theme::IconActionAbout));
    connect(m_aboutAction, SIGNAL(triggered(bool)),
            this, SLOT(aboutAction_triggered()));

    m_aboutQtAction = new QAction(this);
    m_aboutQtAction->setText(tr("About &Qt..."));
    m_aboutQtAction->setIcon(theme::icon(theme::IconActionAboutQt));
    connect(m_aboutQtAction, SIGNAL(triggered(bool)),
            this, SLOT(aboutQtAction_triggered()));
}

void MainWindow::createSystemTrayIcon()
{
    m_trayIcon = new SystemTrayIcon(m_project, this);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIcon_activated(QSystemTrayIcon::ActivationReason)));
    connect(m_trayIcon, SIGNAL(messageClicked()), this, SLOT(show()));

    QMenu *trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(m_trayIcon->statusAction());
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(m_project->startAction());
    trayIconMenu->addAction(m_project->stopAction());
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(m_quitAction);
    m_trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::createTaskView()
{
    m_taskView = new TaskView(m_project, this);
}

void MainWindow::createTomatoWidget()
{
    m_tomatoWidget = new TomatoWidget(m_project, this);
}

void MainWindow::createWindowMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(m_project->newAction());
    fileMenu->addAction(m_project->openAction());
    fileMenu->addAction(m_project->saveAction());
    fileMenu->addAction(m_project->saveAsAction());
    fileMenu->addAction(m_project->closeAction());
    fileMenu->addSeparator();
    fileMenu->addAction(m_project->propertiesAction());
    fileMenu->addSeparator();
    fileMenu->addAction(m_quitAction);

    QMenu *stateMenu = new QMenu(tr("&State"), this);
    stateMenu->addAction(m_project->startAction());
    stateMenu->addAction(m_project->stopAction());

    QMenu *taskMenu = new QMenu(tr("&Task"), this);
    taskMenu->addAction(m_taskView->activateAction());
    taskMenu->addSeparator();
    taskMenu->addAction(m_taskView->addAction());
    taskMenu->addAction(m_taskView->editAction());
    taskMenu->addAction(m_taskView->removeAction());
    taskMenu->addAction(m_taskView->removeAllAction());
    taskMenu->addSeparator();
    taskMenu->addAction(m_taskView->hideCompletedAction());
    taskMenu->addSeparator();
    taskMenu->addAction(m_taskView->expandAllAction());
    taskMenu->addAction(m_taskView->collapseAllAction());

    QMenu *toolsMenu = new QMenu(tr("T&ools"), this);
    toolsMenu->addAction(m_settingsAction);

    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(m_aboutAction);
    helpMenu->addAction(m_aboutQtAction);

    setMenuBar(new QMenuBar(this));
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(stateMenu);
    menuBar()->addMenu(taskMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createStatusBar()
{
    m_statusBar = new QStatusBar(this);
    setStatusBar(m_statusBar);
}

void MainWindow::quitAction_triggered()
{
    G_SETTINGS_INIT();

    if (m_project->hasChanges()
            && settings.value(SettingsSaveChangesOnExit, true).toBool()) {
        QString errorString;
        if (!m_project->save(&errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
            return;
        }
    }
    else if (m_project->hasChanges()) {
        const int ret = QMessageBox::question(
                            this,
                            tr("Question"),
                            tr("The project has been changed. "
                               "Do you want to save it before exiting?"),
                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (ret == QMessageBox::Cancel) {
            return;
        }

        if (ret == QMessageBox::Yes) {
            QString errorString;
            if (!m_project->save(&errorString)) {
                QMessageBox::warning(this, tr("Warning"), errorString);
                return;
            }
        }
    }

    qApp->quit();
}

void MainWindow::settingsAction_triggered()
{
    G_SETTINGS_INIT();

    SettingsDialog dialog(this);
    dialog.setPlayWorkingFinishSound(settings.value(SettingsPlayWorkingFinishSound, true).toBool());
    dialog.setPlayRestingFinishSound(settings.value(SettingsPlayRestingFinishSound, true).toBool());
    dialog.setShowWorkingFinishedTrayNotify(settings.value(SettingsShowWorkingFinishTrayNotify, true).toBool());
    dialog.setShowRestingFinishedTrayNotify(settings.value(SettingsShowRestingFinishTrayNotify, true).toBool());
    dialog.setSaveChangesOnExit(settings.value(SettingsSaveChangesOnExit, true).toBool());
    if (dialog.exec() == QDialog::Accepted) {
        settings.setValue(SettingsPlayWorkingFinishSound, dialog.isPlayWorkingFinishSound());
        settings.setValue(SettingsPlayRestingFinishSound, dialog.isPlayRestingFinishSound());
        settings.setValue(SettingsShowWorkingFinishTrayNotify, dialog.isShowWorkingFinishedTrayNotify());
        settings.setValue(SettingsShowRestingFinishTrayNotify, dialog.isShowRestingFinishedTrayNotify());
        settings.setValue(SettingsSaveChangesOnExit, dialog.isSaveChangesOnExit());
    }
}

void MainWindow::aboutAction_triggered()
{
    AboutDialog dialog(this);
    dialog.setPixmap(QPixmap::fromImage(theme::appImage()));
    dialog.exec();
}

void MainWindow::aboutQtAction_triggered()
{
    qApp->aboutQt();
}

void MainWindow::project_newAction_triggered()
{
    G_SETTINGS_INIT();

    const QString proposedDatabaseFileName =
        settings.value(SettingsLastPath, QDir::homePath()).toString()
        + QDir::separator()
        + DefaultProjectFileName;

    const QString fileName = QFileDialog::getSaveFileName(
                                 this,
                                 tr("Create project"),
                                 proposedDatabaseFileName,
                                 tr("Project files (*.ttt)"));

    if (fileName.isEmpty())
        return;

    QString errorString;
    if (!m_project->create(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }
    else {
        settings.setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }
}

void MainWindow::project_openAction_triggered()
{
    G_SETTINGS_INIT();

    const QString fileName = QFileDialog::getOpenFileName(
                                 this,
                                 tr("Open project"),
                                 settings.value(SettingsLastPath,
                                                QDir::homePath()).toString(),
                                 tr("Project files (*.ttt)"));

    if (fileName.isEmpty())
        return;

    QString errorString;
    if (!m_project->open(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }
    else {
        settings.setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }
}

void MainWindow::project_saveAction_triggered()
{
    QString errorString;
    if (!m_project->save(&errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }

}

void MainWindow::project_saveAsAction_triggered()
{
    G_SETTINGS_INIT();

    const QString proposedDatabaseFileName =
        settings.value(SettingsLastPath, QDir::homePath()).toString()
        + QDir::separator()
        + DefaultProjectFileName;

    const QString fileName = QFileDialog::getSaveFileName(
                                 this,
                                 tr("Save project as"),
                                 proposedDatabaseFileName,
                                 tr("Project files (*.ttt)"));

    if (fileName.isEmpty())
        return;

    QString errorString;
    if (!m_project->saveAs(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }
    else {
        settings.setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }

}

void MainWindow::project_closeAction_triggered()
{
    if (m_project->hasChanges()) {
        const int ret = QMessageBox::question(
                            this,
                            tr("Question"),
                            tr("The project has been changed. "
                               "Do you want to save it before closing?"),
                            QMessageBox::Yes
                            | QMessageBox::No
                            | QMessageBox::Cancel);

        if (ret == QMessageBox::Cancel)
            return;

        if (ret == QMessageBox::Yes) {
            QString errorString;
            if (!m_project->save(&errorString)) {
                QMessageBox::warning(this, tr("Warning"), errorString);
                return;
            }
        }
    }

    m_project->close();
}

void MainWindow::project_propertiesAction_triggered()
{
    PropertiesDialog dialog(this);
    dialog.setWorkingTime(m_project->tomato()->workingTime());
    dialog.setRestingTime(m_project->tomato()->restingTime());
    if (dialog.exec() == QDialog::Accepted) {
        m_project->tomato()->setWorkingTime(dialog.workingTime());
        m_project->tomato()->setRestingTime(dialog.restingTime());
    }
}

void MainWindow::project_startAction_triggered()
{
    if (m_project->tomato()->state() == Tomato::Idle
            || m_project->tomato()->state() == Tomato::Resting
            || m_project->tomato()->state() == Tomato::OverResting) {
        m_project->tomato()->startWorking();
    }
    else {
        m_project->tomato()->startResting();
    }
}

void MainWindow::project_stopAction_triggered()
{
    m_project->tomato()->startIdle();
}

void MainWindow::trayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        if (isHidden()) {
            show();
        }
        else {
            hide();
        }
    }
}

void MainWindow::tomatoStateChanged(Tomato::State state)
{
    G_SETTINGS_INIT();

    bool playWorkingSound = settings.value(SettingsPlayWorkingFinishSound, true).toBool();
    bool playRestingSound = settings.value(SettingsPlayRestingFinishSound, true).toBool();

    bool showWorkingTrayNotify = settings.value(SettingsShowWorkingFinishTrayNotify, true).toBool();
    bool showRestingTrayNotify = settings.value(SettingsShowRestingFinishTrayNotify, true).toBool();

    if ((playWorkingSound && state == Tomato::OverWorking)
            || (playRestingSound && state == Tomato::OverResting)) {
        Phonon::MediaObject *notify = Phonon::createPlayer(Phonon::NotificationCategory);
        notify->setCurrentSource(QUrl::fromLocalFile(soundResFileName("notify.wav")));
        notify->play();
        connect(notify, SIGNAL(finished()), notify, SLOT(deleteLater()));
    }

    if (showWorkingTrayNotify && state == Tomato::OverWorking) {
        m_trayIcon->showWorkingTimeoutMessage();
    }

    if (showRestingTrayNotify && state == Tomato::OverResting) {
        m_trayIcon->showRestingTimeoutMessage();
    }
}

void MainWindow::updateWindowTitle()
{
    if (m_project->isOpen()) {
        setWindowTitle(QString("%1 - %2").arg(m_project->fileName(), appShortName()));
    }
    else {
        setWindowTitle(appName());
    }
}

void MainWindow::updateEnableState()
{
    centralWidget()->setEnabled(m_project->isOpen());
}

void MainWindow::closeEvent(QCloseEvent *closeEvent)
{
    hide();
    closeEvent->ignore();
}
