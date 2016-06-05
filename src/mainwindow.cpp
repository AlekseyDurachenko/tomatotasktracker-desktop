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
#include "mainwindow.h"
#include "aboutdialog.h"
#include "consts.h"
#include "playsound.h"
#include "project.h"
#include "propertiesdialog.h"
#include "resources.h"
#include "settingsdialog.h"
#include "settings.h"
#include "taskeditdialog.h"
#include "taskitemfilterproxymodel.h"
#include "taskitemmodel.h"
#include "theme.h"
#include "timestr.h"
#include "version.h"
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTreeView>
#include <QVBoxLayout>
#include <QToolButton>
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(theme::icon(theme::IconApp));
    resize(400, 500);

    createProject();
    createActions();
    createMenu();
    createStatusBar();
    createSystemTrayIcon();
    createLayout();
    createSaveTimer();
    createSyncTimer();

    readSettings();
    readCommandLineArguments();

    updateStartStopAction();
    updateWindowTitle();
    updateLayout();
    updateActions();
    updateStatusLabels();
    updateSystemTrayIcon();
    updateSystemTrayStatusAction();
    updateSaveTimer();
    updateSyncTimer();
}

MainWindow::~MainWindow()
{
    saveProjectOnExit();
    writeSettings();
}

void MainWindow::newAction_triggered()
{
    const QString proposedDatabaseFileName =
        appSettings().value(SettingsLastPath, QDir::homePath()).toString()
        + QDir::separator()
        + DefaultProjectFileName;

    const QString fileName =
        QFileDialog::getSaveFileName(
            this,
            tr("New"),
            proposedDatabaseFileName,
            tr("Project files (*.ttt)"));

    if (fileName.isEmpty()) {
        return;
    }

    QString errorString;
    if (!m_project->create(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }
    else {
        appSettings().setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }
}

void MainWindow::openAction_triggered()
{
    const QString fileName =
        QFileDialog::getOpenFileName(
            this,
            tr("Open"),
            appSettings().value(SettingsLastPath, QDir::homePath()).toString(),
            tr("Project files (*.ttt)"));

    if (fileName.isEmpty()) {
        return;
    }

    QString errorString;
    if (!m_project->open(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }
    else {
        appSettings().setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }
}

void MainWindow::saveAction_triggered()
{
    QString errorString;
    if (!m_project->save(&errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }
}

void MainWindow::saveAsAction_triggered()
{
    const QString proposedDatabaseFileName =
        appSettings().value(SettingsLastPath, QDir::homePath()).toString()
        + QDir::separator()
        + DefaultProjectFileName;

    const QString fileName =
        QFileDialog::getSaveFileName(
            this,
            tr("Save as"),
            proposedDatabaseFileName,
            tr("Project files (*.ttt)"));

    if (fileName.isEmpty()) {
        return;
    }

    QString errorString;
    if (!m_project->saveAs(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }
    else {
        appSettings().setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }
}

void MainWindow::closeAction_triggered()
{
    if (m_project->hasChanges()) {
        const int ret =
            QMessageBox::question(
                this,
                tr("Question"),
                tr("The project has been changed. "
                   "Do you want to save it before closing?"),
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

    m_project->close();
}

void MainWindow::propertiesAction_triggered()
{
    PropertiesDialog dialog(this);
    dialog.setWorkingTime(m_project->tomato()->workingTime());
    dialog.setRestingTime(m_project->tomato()->restingTime());
    if (dialog.exec() == QDialog::Accepted) {
        m_project->tomato()->setWorkingTime(dialog.workingTime());
        m_project->tomato()->setRestingTime(dialog.restingTime());
    }
}

void MainWindow::quitAction_triggered()
{
    if (m_project->hasChanges()
            && appSettings().value(SettingsSaveChangesOnExit, DefaultSettingsSavingChangesOnExit).toBool()) {
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

void MainWindow::startAction_triggered()
{
    if (m_project->tomato()->state() == Tomato::Idle
            || m_project->tomato()->state() == Tomato::Resting
            || m_project->tomato()->state() == Tomato::OverResting) {
        m_project->taskManager()->startWorking();
    }
    else {
        m_project->taskManager()->startResting();
    }
}

void MainWindow::stopAction_triggered()
{
    m_project->taskManager()->startIdle();
}

void MainWindow::activateAction_triggered()
{
    const QModelIndex firstSelectedIndex =
        m_taskView->selectionModel()->selectedRows(0).first();

    m_project->taskManager()->setActiveItem(
        m_project->taskManager()->findById(
            firstSelectedIndex.data(Qt::UserRole).toInt()));
}

void MainWindow::addAction_triggered()
{
    TaskItem *item = m_project->taskManager()->rootItem();
    if (m_taskView->selectionModel()->hasSelection()) {
        item = m_project->taskManager()->findById(
                   m_taskView->selectionModel()->selectedRows(0).first().data(
                       Qt::UserRole).toInt());
    }

    TaskEditDialog dialog(this);
    dialog.setWindowTitle(tr("Add"));
    if (dialog.exec() == QDialog::Accepted) {
        item->append(dialog.data(), dialog.times());
    }
}

void MainWindow::editAction_triggered()
{
    TaskItem *item = 0;
    if (m_taskView->selectionModel()->hasSelection()) {
        item = m_project->taskManager()->findById(
                   m_taskView->selectionModel()->selectedRows(0).first().data(
                       Qt::UserRole).toInt());
    }

    TaskEditDialog dialog(this);
    dialog.setWindowTitle(tr("Edit"));
    dialog.setData(item->data());
    dialog.setTimes(item->times());
    if (dialog.exec() == QDialog::Accepted) {
        item->setData(dialog.data(), dialog.times());
    }
}

void MainWindow::removeAction_triggered()
{
    if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Are you shure you want to remove the selected tasks?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    QList<int> ids;
    foreach (const QModelIndex &index, m_taskView->selectionModel()->selectedRows(0)) {
        ids.append(index.data(Qt::UserRole).toInt());
    }

    foreach (const int id, ids) {
        TaskItem *item = m_project->taskManager()->findById(id);
        if (item) {
            item->parent()->removeAt(item->calcIndex());
        }
    }
}

void MainWindow::removeAllAction_triggered()
{
    if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Are you shure you want to remove all tasks?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    m_project->taskManager()->rootItem()->removeAll();
}

void MainWindow::hideCompletedAction_toggled(bool hide)
{
    m_taskItemFilterProxyModel->setComplitedVisible(!hide);
}

void MainWindow::expandAllAction_triggered()
{
    m_taskView->expandAll();
}

void MainWindow::collapseAllAction_triggered()
{
    m_taskView->collapseAll();
}

void MainWindow::settingsAction_triggered()
{
    SettingsDialog dialog(this);
    dialog.setSaveChangesOnExit(appSettings().value(SettingsSaveChangesOnExit, DefaultSettingsSavingChangesOnExit).toBool());
    dialog.setSaveChangesPeriodically(appSettings().value(SettingsSaveChangesOnTimer, DefaultSettingsSavingChangesOnTimer).toBool());
    dialog.setSaveChangesInterval(appSettings().value(SettingsSaveChangesInterval, DefaultSettingsSaveChangesInterval).toInt());
    dialog.setShowWorkingFinishedTrayNotify(appSettings().value(SettingsShowWorkingFinishTrayNotify, DefaultSettingsShowWorkingFinishTrayNotify).toBool());
    dialog.setShowRestingFinishedTrayNotify(appSettings().value(SettingsShowRestingFinishTrayNotify, DefaultSettingsShowRestingFinishTrayNotify).toBool());
    dialog.setWorkingFinishedSound(appSettings().value(SettingsWorkingFinishedSoundFile).toString());
    dialog.setRestingFinishedSound(appSettings().value(SettingsRestingFinishedSoundFile).toString());
    dialog.setPlayWorkingFinishedSound(appSettings().value(SettingsPlayWorkingFinishedSound, DefaultSettingsPlayWorkingFinishedSound).toBool());
    dialog.setPlayRestingFinishedSound(appSettings().value(SettingsPlayRestingFinishedSound, DefaultSettingsPlayRestingFinishedSound).toBool());
    if (dialog.exec() == QDialog::Accepted) {
        appSettings().setValue(SettingsSaveChangesOnExit, dialog.isSaveChangesOnExit());
        appSettings().setValue(SettingsSaveChangesOnTimer, dialog.isSaveChangesPeriodically());
        appSettings().setValue(SettingsSaveChangesInterval, dialog.saveChangesInterval());
        appSettings().setValue(SettingsShowWorkingFinishTrayNotify, dialog.isShowWorkingFinishedTrayNotify());
        appSettings().setValue(SettingsShowRestingFinishTrayNotify, dialog.isShowRestingFinishedTrayNotify());
        appSettings().setValue(SettingsWorkingFinishedSoundFile, dialog.workingFinishedSound());
        appSettings().setValue(SettingsRestingFinishedSoundFile, dialog.restingFinishedSound());
        appSettings().setValue(SettingsPlayWorkingFinishedSound, dialog.playWorkingFinishedSound());
        appSettings().setValue(SettingsPlayRestingFinishedSound, dialog.playRestingFinishedSound());

        updateSaveTimer();
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

void MainWindow::updateStartStopAction()
{
    if (m_project->taskManager()->activeItem()) {
        m_startAction->setEnabled(true);
        m_stopAction->setEnabled(true);

        switch (m_project->tomato()->state()) {
        case Tomato::Idle:
            m_stopAction->setEnabled(false);
        case Tomato::Resting:
        case Tomato::OverResting:
            m_startAction->setText(tr("&Start working"));
            m_startAction->setIcon(theme::icon(theme::IconActionStartWorking));
            break;
        case Tomato::Working:
        case Tomato::OverWorking:
            m_startAction->setText(tr("&Start resting"));
            m_startAction->setIcon(theme::icon(theme::IconActionStartResting));
        }
    }
    else {
        m_startAction->setEnabled(false);
        m_startAction->setText(tr("&Start working"));
        m_startAction->setIcon(theme::icon(theme::IconActionStartWorking));
        m_stopAction->setEnabled(false);
    }
}

void MainWindow::updateWindowTitle()
{
    if (m_project->isOpen()) {
        if (m_project->hasChanges()) {
            setWindowTitle(
                QString("%1* - %2").arg(
                    m_project->fileName(), appShortName()));
        }
        else {
            setWindowTitle(
                QString("%1 - %2").arg(
                    m_project->fileName(), appShortName()));
        }
    }
    else {
        setWindowTitle(appName());
    }
}

void MainWindow::updateLayout()
{
    centralWidget()->setEnabled(m_project->isOpen());
}

void MainWindow::updateActions()
{
    const bool isOpen = m_project->isOpen();
    const bool hasChanges = m_project->hasChanges();
    const bool hasSelection = m_taskView->selectionModel()->hasSelection();
    const int selectedRows = m_taskView->selectionModel()->selectedRows().count();
    const bool isTaskExists = m_project->taskManager()->rootItem()->count();

    m_saveAction->setEnabled(hasChanges);
    m_saveAsAction->setEnabled(isOpen);
    m_closeAction->setEnabled(isOpen);
    m_propertiesAction->setEnabled(isOpen);
    m_activateAction->setEnabled(selectedRows == 1);
    m_addAction->setEnabled(isOpen && selectedRows < 2);
    m_editAction->setEnabled(selectedRows == 1);
    m_removeAction->setEnabled(hasSelection);
    m_removeAllAction->setEnabled(isTaskExists);
    m_hideCompletedAction->setEnabled(isOpen);
    m_expandAllAction->setEnabled(isTaskExists);
    m_collapseAllAction->setEnabled(isTaskExists);
}

void MainWindow::updateStatusLabels()
{
    switch (m_project->tomato()->state()) {
    case Tomato::Idle:
        m_statusFrame->setStyleSheet("");
        break;
    case Tomato::Working:
        m_statusFrame->setStyleSheet("background-color: red;");
        break;
    case Tomato::OverWorking:
        m_statusFrame->setStyleSheet("background-color: yellow;");
        break;
    case Tomato::Resting:
        m_statusFrame->setStyleSheet("background-color: lightGreen;");
        break;
    case Tomato::OverResting:
        m_statusFrame->setStyleSheet("background-color: yellow;");
        break;
    };

    m_statusLabel->setText(tomatoStatusText());
    m_timeLabel->setText(tomatoTimeText());
}

void MainWindow::updateSystemTrayIcon()
{
    if (!m_project->isOpen()) {
        m_trayIcon->setIcon(theme::icon(theme::IconStatusIdle));
        return;
    }

    switch (m_project->tomato()->state()) {
    case Tomato::Idle:
        m_trayIcon->setIcon(theme::icon(theme::IconStatusIdle));
        break;
    case Tomato::Working:
        m_trayIcon->setIcon(theme::icon(theme::IconStatusWorking));
        break;
    case Tomato::OverWorking:
        m_trayIcon->setIcon(theme::icon(theme::IconStatusTimeout));
        break;
    case Tomato::Resting:
        m_trayIcon->setIcon(theme::icon(theme::IconStatusResting));
        break;
    case Tomato::OverResting:
        m_trayIcon->setIcon(theme::icon(theme::IconStatusTimeout));
        break;
    }
}

void MainWindow::updateSystemTrayStatusAction()
{
    if (m_project->tomato()->state() == Tomato::Idle) {
        m_trayIcon->setToolTip(tomatoStatusText());
        m_statusAction->setText(tomatoStatusText());
        return;
    }

    const QString text = QString("%1: %2").arg(tomatoStatusText(), tomatoTimeText());
    m_trayIcon->setToolTip(text);
    m_statusAction->setText(text);
}

void MainWindow::updateSaveTimer()
{
    if (!m_project->hasChanges()) {
        m_saveTimer->stop();
        return;
    }

    const bool isEnabled = appSettings().value(SettingsSaveChangesOnTimer, DefaultSettingsSavingChangesOnTimer).toBool();
    const int interval = appSettings().value(SettingsSaveChangesInterval, DefaultSettingsSaveChangesInterval).toInt();

    if (isEnabled) {
        m_saveTimer->setInterval(60000 * interval); // 1m = 60s * 1000ms
        m_saveTimer->start();
    }
    else {
        m_saveTimer->stop();
    }
}

void MainWindow::updateSyncTimer()
{
    switch (m_project->tomato()->state()) {
    case Tomato::Idle:
        m_syncTimer->stop();
    case Tomato::Working:
    case Tomato::OverWorking:
    case Tomato::Resting:
    case Tomato::OverResting:
        m_syncTimer->start();
    }
}

void MainWindow::taskView_customContextMenu(QPoint pos)
{
    QMenu menu(this);
    menu.addAction(m_activateAction);
    menu.addSeparator();
    menu.addAction(m_addAction);
    menu.addAction(m_editAction);
    menu.addAction(m_removeAction);
    menu.addAction(m_removeAllAction);
    menu.addSeparator();
    menu.addAction(m_hideCompletedAction);
    menu.addSeparator();
    menu.addAction(m_expandAllAction);
    menu.addAction(m_collapseAllAction);
    menu.exec(m_taskView->viewport()->mapToGlobal(pos));
}

void MainWindow::taskView_itemActivated(const QModelIndex &index)
{
    m_project->taskManager()->setActiveItem(
        m_project->taskManager()->findById(
            index.data(Qt::UserRole).toInt()));
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

void MainWindow::saveProjectOnExit()
{
    if (!m_project->hasChanges()) {
        return;
    }

    if (appSettings().value(SettingsSaveChangesOnExit, DefaultSettingsSavingChangesOnExit).toBool()) {
        QString errorString;
        if (!m_project->save(&errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
        }
    }
}

void MainWindow::saveProjectOnTimer()
{
    if (m_project->hasChanges()) {
        QString errorString;
        if (!m_project->save(&errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
        }
    }
}

void MainWindow::notifyStateChanged(Tomato::State state)
{
    bool playWorkingFinishedSound = appSettings().value(SettingsPlayWorkingFinishedSound, DefaultSettingsPlayWorkingFinishedSound).toBool();
    bool playRestingFinishedSound = appSettings().value(SettingsPlayRestingFinishedSound, DefaultSettingsPlayRestingFinishedSound).toBool();

    QString workingFinishedSound = appSettings().value(SettingsWorkingFinishedSoundFile).toString();
    QString restingFinishedSound = appSettings().value(SettingsRestingFinishedSoundFile).toString();

    bool showWorkingTrayNotify = appSettings().value(SettingsShowWorkingFinishTrayNotify, DefaultSettingsShowWorkingFinishTrayNotify).toBool();
    bool showRestingTrayNotify = appSettings().value(SettingsShowRestingFinishTrayNotify, DefaultSettingsShowRestingFinishTrayNotify).toBool();

    if (state == Tomato::OverWorking
            && playWorkingFinishedSound
            && !workingFinishedSound.isEmpty()) {
        playSound(workingFinishedSound);
    }

    if (state == Tomato::OverResting
            && playRestingFinishedSound
            && !restingFinishedSound.isEmpty()) {
        playSound(restingFinishedSound);
    }

    if (state == Tomato::OverWorking
            && showWorkingTrayNotify) {
        m_trayIcon->showMessage(appName(), tr("It's time to rest"), QSystemTrayIcon::Information);
    }

    if (state == Tomato::OverResting
            && showRestingTrayNotify) {
        m_trayIcon->showMessage(appName(), tr("It's time to work"), QSystemTrayIcon::Information);
    }
}

void MainWindow::readSettings()
{
    restoreGeometry(appSettings().value(SettingsMainWindowGeometry, saveGeometry()).toByteArray());
    restoreState(appSettings().value(SettingsMainWindowState, saveState()).toByteArray());
    m_taskView->header()->restoreState(appSettings().value(SettingsTaskViewHeaderState, m_taskView->header()->saveState()).toByteArray());
    m_hideCompletedAction->setChecked(appSettings().value(SettingsHideCompletedTasks, m_hideCompletedAction->isChecked()).toBool());

    // set default sound on first run
    if (!appSettings().contains(SettingsWorkingFinishedSoundFile)) {
        appSettings().setValue(SettingsWorkingFinishedSoundFile, soundFileName(DefaultSettingsWorkingFinishedSound));
    }
    if (!appSettings().contains(SettingsRestingFinishedSoundFile)) {
        appSettings().setValue(SettingsRestingFinishedSoundFile, soundFileName(DefaultSettingsRestingFinishedSound));
    }
}

void MainWindow::writeSettings()
{
    appSettings().setValue(SettingsLastProject, m_project->fileName());
    appSettings().setValue(SettingsMainWindowGeometry, saveGeometry());
    appSettings().setValue(SettingsMainWindowState, saveState());
    appSettings().setValue(SettingsTaskViewHeaderState, m_taskView->header()->saveState());
    appSettings().setValue(SettingsHideCompletedTasks, m_hideCompletedAction->isChecked());
}

void MainWindow::readCommandLineArguments()
{
    if (qApp->arguments().count() == 1) {
        loadLastProject();
    }
    else if (qApp->arguments().count() != 2) {
        QMessageBox::information(
            this,
            tr("Usage"),
            tr("Usage: %1 <filename>").arg(appName()));
        qApp->quit();
    }
    else {
        QString projectFileName = qApp->arguments().at(1);
        QString errorString;
        if (!m_project->open(projectFileName, &errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
        }

        qApp->quit();
    }
}

void MainWindow::loadLastProject()
{
    QString projectFileName = appSettings().value(SettingsLastProject, "").toString();
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
    // stop/stop actions
    connect(m_project->taskManager(), SIGNAL(activeItemChanged()),
            this, SLOT(updateStartStopAction()));
    connect(m_project->tomato(), SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateStartStopAction()));
    // window title
    connect(m_project, SIGNAL(openStateChanged()),
            this, SLOT(updateWindowTitle()));
    connect(m_project, SIGNAL(saveStateChanged()),
            this, SLOT(updateWindowTitle()));
    // layout enabled
    connect(m_project, SIGNAL(openStateChanged()),
            this, SLOT(updateLayout()));
    // update actions
    connect(m_project, SIGNAL(openStateChanged()),
            this, SLOT(updateActions()));
    connect(m_project, SIGNAL(saveStateChanged()),
            this, SLOT(updateActions()));
    connect(m_project->taskManager(), SIGNAL(taskInserted(TaskItem *, int, int)),
            this, SLOT(updateActions()));
    connect(m_project->taskManager(), SIGNAL(taskRemoved()),
            this, SLOT(updateActions()));
    // update tomato status
    connect(m_project->tomato(), SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateStatusLabels()));
    connect(m_project->tomato(), SIGNAL(workingTimeChanged()),
            this, SLOT(updateStatusLabels()));
    connect(m_project->tomato(), SIGNAL(restingTimeChanged()),
            this, SLOT(updateStatusLabels()));
    // update tray icon
    connect(m_project, SIGNAL(openStateChanged()),
            this, SLOT(updateSystemTrayIcon()));
    connect(m_project->tomato(), SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateSystemTrayIcon()));
    connect(m_project, SIGNAL(openStateChanged()),
            this, SLOT(updateSystemTrayStatusAction()));
    connect(m_project->tomato(), SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateSystemTrayStatusAction()));
    // update save timer
    connect(m_project, SIGNAL(saveStateChanged()),
            this, SLOT(updateSaveTimer()));
    // update sync timer
    connect(m_project->tomato(), SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateSyncTimer()));
    // notify sound/tray icon message
    connect(m_project->tomato(), SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(notifyStateChanged(Tomato::State)));
}

void MainWindow::createActions()
{
    // File menu
    m_newAction = new QAction(this);
    m_newAction->setText(tr("&New..."));
    m_newAction->setIcon(theme::icon(theme::IconActionProjectNew));
    m_newAction->setShortcut(QKeySequence(tr("Ctrl+n")));
    connect(m_newAction, SIGNAL(triggered(bool)),
            this, SLOT(newAction_triggered()));

    m_openAction = new QAction(this);
    m_openAction->setText(tr("&Open..."));
    m_openAction->setIcon(theme::icon(theme::IconActionProjectOpen));
    m_openAction->setShortcut(QKeySequence(tr("Ctrl+o")));
    connect(m_openAction, SIGNAL(triggered(bool)),
            this, SLOT(openAction_triggered()));

    m_saveAction = new QAction(this);
    m_saveAction->setText(tr("&Save..."));
    m_saveAction->setIcon(theme::icon(theme::IconActionProjectSave));
    m_saveAction->setShortcut(QKeySequence(tr("Ctrl+s")));
    connect(m_saveAction, SIGNAL(triggered(bool)),
            this, SLOT(saveAction_triggered()));

    m_saveAsAction = new QAction(this);
    m_saveAsAction->setText(tr("Save &as..."));
    m_saveAsAction->setIcon(theme::icon(theme::IconActionProjectSaveAs));
    connect(m_saveAsAction, SIGNAL(triggered(bool)),
            this, SLOT(saveAsAction_triggered()));

    m_closeAction = new QAction(this);
    m_closeAction->setText(tr("Close..."));
    m_closeAction->setIcon(theme::icon(theme::IconActionProjectClose));
    connect(m_closeAction, SIGNAL(triggered(bool)),
            this, SLOT(closeAction_triggered()));

    m_propertiesAction = new QAction(this);
    m_propertiesAction->setText(tr("&Properties..."));
    m_propertiesAction->setIcon(theme::icon(theme::IconActionProjectProperties));
    connect(m_propertiesAction, SIGNAL(triggered(bool)),
            this, SLOT(propertiesAction_triggered()));

    m_quitAction = new QAction(this);
    m_quitAction->setText(tr("&Quit..."));
    m_quitAction->setIcon(theme::icon(theme::IconActionQuit));
    m_quitAction->setShortcut(QKeySequence(tr("Ctrl+q")));
    connect(m_quitAction, SIGNAL(triggered(bool)),
            this, SLOT(quitAction_triggered()));


    // State menu
    m_startAction = new QAction(this);
    connect(m_startAction, SIGNAL(triggered(bool)),
            this, SLOT(startAction_triggered()));

    m_stopAction = new QAction(this);
    m_stopAction->setText(tr("Sto&p"));
    m_stopAction->setIcon(theme::icon(theme::IconActionStop));
    connect(m_stopAction, SIGNAL(triggered(bool)),
            this, SLOT(stopAction_triggered()));

    // Task menu
    m_activateAction = new QAction(this);
    m_activateAction->setText(tr("Act&ivate"));
    connect(m_activateAction, SIGNAL(triggered(bool)),
            this, SLOT(activateAction_triggered()));

    m_addAction = new QAction(this);
    m_addAction->setText(tr("&Add..."));
    m_addAction->setIcon(theme::icon(theme::IconActionTaskAdd));
    m_addAction->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(m_addAction, SIGNAL(triggered(bool)),
            this, SLOT(addAction_triggered()));

    m_editAction = new QAction(this);
    m_editAction->setText(tr("&Edit..."));
    m_editAction->setIcon(theme::icon(theme::IconActionTaskEdit));
    connect(m_editAction, SIGNAL(triggered(bool)),
            this, SLOT(editAction_triggered()));

    m_removeAction = new QAction(this);
    m_removeAction->setText(tr("&Remove..."));
    m_removeAction->setIcon(theme::icon(theme::IconActionTaskRemove));
    m_removeAction->setShortcut(QKeySequence(Qt::Key_Delete));
    connect(m_removeAction, SIGNAL(triggered(bool)),
            this, SLOT(removeAction_triggered()));

    m_removeAllAction = new QAction(this);
    m_removeAllAction->setText(tr("Remove al&l..."));
    m_removeAllAction->setIcon(theme::icon(theme::IconActionTaskRemoveAll));
    connect(m_removeAllAction, SIGNAL(triggered(bool)),
            this, SLOT(removeAllAction_triggered()));

    m_hideCompletedAction = new QAction(this);
    m_hideCompletedAction->setText(tr("&Hide completed tasks"));
    m_hideCompletedAction->setCheckable(true);
    connect(m_hideCompletedAction, SIGNAL(toggled(bool)),
            this, SLOT(hideCompletedAction_toggled(bool)));

    m_expandAllAction = new QAction(this);
    m_expandAllAction->setText(tr("Expand all"));
    connect(m_expandAllAction, SIGNAL(triggered(bool)),
            this, SLOT(expandAllAction_triggered()));

    m_collapseAllAction = new QAction(this);
    m_collapseAllAction->setText(tr("Collapse all"));
    connect(m_collapseAllAction, SIGNAL(triggered(bool)),
            this, SLOT(collapseAllAction_triggered()));


    // Tool menu
    m_settingsAction = new QAction(this);
    m_settingsAction->setText(tr("&Settings..."));
    m_settingsAction->setIcon(theme::icon(theme::IconActionSettings));
    connect(m_settingsAction, SIGNAL(triggered(bool)),
            this, SLOT(settingsAction_triggered()));


    // Help menu
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


    // for tray icon menu
    m_statusAction = new QAction(this);
    m_statusAction->setEnabled(false);
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addAction(m_closeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_propertiesAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_quitAction);

    QMenu *stateMenu = new QMenu(tr("&State"), this);
    stateMenu->addAction(m_startAction);
    stateMenu->addAction(m_stopAction);

    QMenu *taskMenu = new QMenu(tr("&Task"), this);
    taskMenu->addAction(m_activateAction);
    taskMenu->addSeparator();
    taskMenu->addAction(m_addAction);
    taskMenu->addAction(m_editAction);
    taskMenu->addAction(m_removeAction);
    taskMenu->addAction(m_removeAllAction);
    taskMenu->addSeparator();
    taskMenu->addAction(m_hideCompletedAction);
    taskMenu->addSeparator();
    taskMenu->addAction(m_expandAllAction);
    taskMenu->addAction(m_collapseAllAction);

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
    setStatusBar(new QStatusBar(this));
}

void MainWindow::createSystemTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIcon_activated(QSystemTrayIcon::ActivationReason)));
    connect(m_trayIcon, SIGNAL(messageClicked()), this, SLOT(show()));

    QMenu *menu = new QMenu(this);
    menu->addAction(m_statusAction);
    menu->addSeparator();
    menu->addAction(m_startAction);
    menu->addAction(m_stopAction);
    menu->addSeparator();
    menu->addAction(m_quitAction);
    m_trayIcon->setContextMenu(menu);

    m_trayIcon->setIcon(theme::icon(theme::IconStatusIdle));
    m_trayIcon->setVisible(true);
}

void MainWindow::createLayout()
{
    m_taskItemModel = new TaskItemModel(m_project->taskManager(), this);

    m_taskItemFilterProxyModel = new TaskItemFilterProxyModel(this);
    connect(m_project->taskManager(), SIGNAL(activeItemChanged()),
            m_taskItemFilterProxyModel, SLOT(updateFilter()));
    m_taskItemFilterProxyModel->setDynamicSortFilter(true);
    m_taskItemFilterProxyModel->setSourceModel(m_taskItemModel);

    m_taskView = new QTreeView(this);
    m_taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_taskView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_taskView->setDragEnabled(true);
    m_taskView->setDragDropMode(QTreeView::DragDrop);
    m_taskView->setModel(m_taskItemFilterProxyModel);
    connect(m_taskView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(updateActions()));
    connect(m_taskView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(taskView_customContextMenu(QPoint)));
    connect(m_taskView, SIGNAL(activated(QModelIndex)),
            this, SLOT(taskView_itemActivated(QModelIndex)));

    m_startToolButton = new QToolButton(this);
    m_startToolButton->setIconSize(QSize(64, 64));
    m_startToolButton->setAutoRaise(true);
    m_startToolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    m_startToolButton->setDefaultAction(m_startAction);

    m_stopToolButton = new QToolButton(this);
    m_stopToolButton->setIconSize(QSize(64, 64));
    m_stopToolButton->setAutoRaise(true);
    m_stopToolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    m_stopToolButton->setDefaultAction(m_stopAction);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont timeLabelFont = m_timeLabel->font();
    timeLabelFont.setPointSize(36);
    m_timeLabel->setFont(timeLabelFont);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont statusLabelFont = m_timeLabel->font();
    statusLabelFont.setPointSize(14);
    statusLabelFont.setBold(true);
    m_statusLabel->setFont(statusLabelFont);

    m_statusFrame = new QFrame(this);
    m_statusFrame->setFrameShape(QFrame::WinPanel);
    m_statusFrame->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(m_timeLabel);
    vbox->addWidget(m_statusLabel);
    m_statusFrame->setLayout(vbox);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_statusFrame);
    hbox->addWidget(m_startToolButton);
    hbox->addWidget(m_stopToolButton);

    QFrame *tomatoFrame = new QFrame(this);
    tomatoFrame->setLayout(hbox);
    tomatoFrame->setFrameShape(QFrame::StyledPanel);
    tomatoFrame->setFrameShadow(QFrame::Raised);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_taskView);
    centralLayout->addWidget(tomatoFrame);
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::createSaveTimer()
{
    m_saveTimer = new QTimer(this);
    m_saveTimer->setSingleShot(true);
    connect(m_saveTimer, SIGNAL(timeout()), this, SLOT(saveProjectOnTimer()));
}

void MainWindow::createSyncTimer()
{
    m_syncTimer = new QTimer(this);
    m_syncTimer->setSingleShot(false);
    m_syncTimer->setInterval(500);
    connect(m_syncTimer, SIGNAL(timeout()),
            m_project, SLOT(sync()));
    connect(m_syncTimer, SIGNAL(timeout()),
            m_project->taskManager(), SLOT(sync()));
    connect(m_syncTimer, SIGNAL(timeout()),
            this, SLOT(updateStatusLabels()));
    connect(m_syncTimer, SIGNAL(timeout()),
            this, SLOT(updateSystemTrayStatusAction()));
}

QString MainWindow::tomatoStatusText() const
{
    switch (m_project->tomato()->state()) {
    case Tomato::Idle:
        return tr("IDLE");
    case Tomato::Working:
        return tr("WORKING");
    case Tomato::OverWorking:
        return tr("IT'S TIME TO REST");
    case Tomato::Resting:
        return tr("RESTING");
    case Tomato::OverResting:
        return tr("IT'S TIME TO WORK");
    };

    return QString();
}

QString MainWindow::tomatoTimeText() const
{
    switch (m_project->tomato()->state()) {
    case Tomato::Idle:
        return secsToTimeStr(m_project->tomato()->workingTime());
    case Tomato::Working:
        return secsToTimeStr(m_project->tomato()->workingTime()
                             - m_project->taskManager()->calcLastStateTime());
    case Tomato::OverWorking:
        return secsToTimeStr(m_project->taskManager()->calcLastStateTime()
                             - m_project->tomato()->workingTime());
    case Tomato::Resting:
        return secsToTimeStr(m_project->tomato()->restingTime()
                             - m_project->taskManager()->calcLastStateTime());
    case Tomato::OverResting:
        return secsToTimeStr(m_project->taskManager()->calcLastStateTime()
                             - m_project->tomato()->restingTime());
    };

    return QString();
}

void MainWindow::closeEvent(QCloseEvent *closeEvent)
{
    // if we ignore all close events, the window manager
    // will be aborted on closing the application window
    if (!closeEvent->spontaneous()) {
        closeEvent->accept();
    }
    else {
        hide();
        closeEvent->ignore();
    }
}
