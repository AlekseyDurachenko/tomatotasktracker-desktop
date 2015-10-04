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
#include "taskeditdialog.h"
#include "aboutdialog.h"
#include "ui_mainwindow.h"
#include "theme.h"
#include "project.h"
#include "consts.h"
#include "version.h"
#include "settings.h"
#include "propertiesdialog.h"
#include "settingsdialog.h"
#include "taskfilterproxymodel.h"
#include "utils.h"
#include <QSettings>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSound>
#include <QCloseEvent>
#include <phonon/phonon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_tomato = new Tomato(this);
    connect(m_tomato, SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateTomatoActions()));
    connect(m_tomato, SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(updateTrayIcon()));
    connect(m_tomato, SIGNAL(stateChanged(Tomato::State)),
            this, SLOT(playSound(Tomato::State)));
    connect(m_tomato, SIGNAL(activeTaskChanged()),
            this, SLOT(updateTomatoActions()));
    connect(m_tomato, SIGNAL(activeTaskChanged()),
            this, SLOT(updateTaskActions()));

    m_project = new Project(m_tomato, this);
    connect(m_project, SIGNAL(changed()),
            this, SLOT(updateProjectActions()));

    TaskItemModel *taskItemModel = new TaskItemModel(m_tomato, this);
    m_taskFilterProxyModel = new TaskFilterProxyModel(m_tomato, this);
    m_taskFilterProxyModel->setSourceModel(taskItemModel);
    m_taskFilterProxyModel->setEnabled(false);
    m_taskFilterProxyModel->setDynamicSortFilter(true);
    //ui->task_treeView->setModel(new TaskItemModel(m_tomato, this));
    ui->task_treeView->setModel(m_taskFilterProxyModel);
    connect(ui->task_treeView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(updateTaskActions()));

    ui->tomato_widget->setTaskManager(m_tomato);
    ui->tomato_widget->setTomatoStartAction(ui->startTomato_action);
    ui->tomato_widget->setTomatoStopAction(ui->stopTomato_action);

    m_tomatoTimer = new QTimer(this);
    m_tomatoTimer->setInterval(500);
    m_tomatoTimer->setSingleShot(false);
    connect(m_tomatoTimer, SIGNAL(timeout()),
            m_tomato, SLOT(updateActiveTaskDisplay()));
    connect(m_tomatoTimer, SIGNAL(timeout()),
            ui->tomato_widget, SLOT(updateTomatoStatus()));    
    connect(m_tomatoTimer, SIGNAL(timeout()),
            this, SLOT(updateTrayStatusText()));
    ui->stopTomato_action->setIcon(Theme::icon(Theme::IconActionTomatoStop));

    m_trayStatusAction = new QAction(this);
    m_trayStatusAction->setEnabled(false);

    m_trayIcon = new QSystemTrayIcon(this);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIcon_activated(QSystemTrayIcon::ActivationReason)));
    m_trayIcon->setIcon(Theme::icon(Theme::IconApp));
    m_trayIcon->setVisible(true);
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_trayStatusAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(ui->startTomato_action);
    m_trayIconMenu->addAction(ui->stopTomato_action);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(ui->quit_action);
    m_trayIcon->setContextMenu(m_trayIconMenu);


    G_SETTINGS_INIT();
    QString projectFileName = settings.value(SettingsLastProject, "").toString();
    if (!projectFileName.isEmpty()) {
        QString errorString;
        if (!m_project->open(projectFileName, &errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
        }
    }

    restoreGeometry(settings.value(SettingsMainWindowGeometry, saveGeometry()).toByteArray());
    restoreState(settings.value(SettingsMainWindowState, saveState()).toByteArray());
    ui->task_treeView->header()->restoreState(settings.value(SettingsTaskViewHeaderState, ui->task_treeView->header()->saveState()).toByteArray());
    ui->hideFinishedTasks_action->setChecked(settings.value(SettingsHideFinishedTasks, ui->hideFinishedTasks_action->isChecked()).toBool());

    updateWindowTitle();
    updateProjectActions();
    updateTomatoActions();
    updateTaskActions();
    updateTrayIcon();
}

MainWindow::~MainWindow()
{
    G_SETTINGS_INIT();

    if (m_project->hasChanges()
            && settings.value(SettingsPlayWorkingFinishSound, true).toBool()) {
        QString errorString;
        if (!m_project->save(&errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
        }
    }

    settings.setValue(SettingsLastProject, m_project->fileName());
    settings.setValue(SettingsMainWindowGeometry, saveGeometry());
    settings.setValue(SettingsMainWindowState, saveState());
    settings.setValue(SettingsTaskViewHeaderState, ui->task_treeView->header()->saveState());
    settings.setValue(SettingsHideFinishedTasks, ui->hideFinishedTasks_action->isChecked());

    delete ui;
}

void MainWindow::on_new_action_triggered()
{
    G_SETTINGS_INIT();

    QString proposedDatabaseFileName =
                settings.value(SettingsLastPath, QDir::homePath()).toString()
                + QDir::separator()
                + DefaultProjectFileName;

    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Create tomato task tracker project"),
                proposedDatabaseFileName,
                tr("Tomato task tracker files (*.ttt)"));

    if (fileName.isEmpty())
        return;

    QString errorString;
    if (!m_project->create(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    } else {
        settings.setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }

    updateWindowTitle();
    updateProjectActions();
    updateTomatoActions();
    updateTaskActions();
    updateTrayIcon();
}

void MainWindow::on_open_action_triggered()
{
    G_SETTINGS_INIT();

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open tomato task tracker project"),
                settings.value(SettingsLastPath, QDir::homePath()).toString(),
                tr("Tomato task tracker files (*.ttt)"));

    if (fileName.isEmpty())
        return;

    QString errorString;
    if (!m_project->open(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    } else {
        settings.setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }

    updateWindowTitle();
    updateProjectActions();
    updateTomatoActions();
    updateTaskActions();
    updateTrayIcon();
}

void MainWindow::on_save_action_triggered()
{
    QString errorString;
    if (!m_project->save(&errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    }

    updateWindowTitle();
    updateProjectActions();
    updateTomatoActions();
    updateTaskActions();
    updateTrayIcon();
}

void MainWindow::on_saveAs_action_triggered()
{
    G_SETTINGS_INIT();

    QString proposedDatabaseFileName =
            settings.value(SettingsLastPath, QDir::homePath()).toString()
            + QDir::separator()
            + DefaultProjectFileName;

    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save tomato task tracker project as"),
                proposedDatabaseFileName,
                tr("Tomato task tracker files (*.ttt)"));

    if (fileName.isEmpty())
        return;

    QString errorString;
    if (!m_project->saveAs(fileName, &errorString)) {
        QMessageBox::warning(this, tr("Warning"), errorString);
    } else {
        settings.setValue(SettingsLastPath, QFileInfo(fileName).absolutePath());
    }

    updateWindowTitle();
    updateProjectActions();
    updateTomatoActions();
    updateTaskActions();
    updateTrayIcon();
}

void MainWindow::on_close_action_triggered()
{
    if (m_project->hasChanges()) {
        int ret = QMessageBox::question(
                    this,
                    tr("Question"),
                    tr("The project has been changed. "
                       "Do you want to save it before closing?"),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

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

    updateWindowTitle();
    updateProjectActions();
    updateTomatoActions();
    updateTaskActions();
    updateTrayIcon();
}

void MainWindow::on_properties_action_triggered()
{
    PropertiesDialog propergiesDialog(this);
    propergiesDialog.setWorkingTime(m_tomato->workingTime());
    propergiesDialog.setRestingTime(m_tomato->restingTime());
    if (propergiesDialog.exec() == QDialog::Accepted) {
        m_tomato->setWorkingTime(propergiesDialog.workingTime());
        m_tomato->setRestingTime(propergiesDialog.restingTime());
    }
}

void MainWindow::on_quit_action_triggered()
{
    G_SETTINGS_INIT();

    if (m_project->hasChanges() && settings.value(SettingsPlayWorkingFinishSound, true).toBool()) {
        QString errorString;
        if (!m_project->save(&errorString)) {
            QMessageBox::warning(this, tr("Warning"), errorString);
            return;
        }
    } else if (m_project->hasChanges()) {
        int ret = QMessageBox::question(
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

void MainWindow::on_startTomato_action_triggered()
{
    if (m_tomato->state() == Tomato::Idle
            || m_tomato->state() == Tomato::Resting
            || m_tomato->state() == Tomato::OverResting)
        m_tomato->startWorking();
    else
        m_tomato->startResting();

    m_tomatoTimer->start();
}

void MainWindow::on_stopTomato_action_triggered()
{
    m_tomato->startIdle();
    m_tomatoTimer->stop();
}

void MainWindow::on_activateTask_action_triggered()
{
    if (ui->task_treeView->selectedTaskCount() != 1)
        return;

    m_tomato->setActiveTask(ui->task_treeView->selectedTasks().first());
}

void MainWindow::on_addTask_action_triggered()
{
    const int selectedTaskCount = ui->task_treeView->selectedTaskCount();
    if (selectedTaskCount > 1)
        return;

    Task *selectedTask = m_tomato->rootTask();
    if (selectedTaskCount == 1)
        selectedTask = ui->task_treeView->selectedTasks().first();

    TaskEditDialog dialog(this);
    dialog.setWindowTitle(tr("New task"));
    if (dialog.exec() == QDialog::Accepted)
        m_tomato->addChildTask(selectedTask, dialog.data());

    updateTaskActions();
}

void MainWindow::on_removeTask_action_triggered()
{
    if (ui->task_treeView->selectedTaskCount() == 0)
        return;

    if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Are you shure you want to remove the selected tasks?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    m_tomato->removeTasks(ui->task_treeView->selectedTasks());

    updateTaskActions();
}

void MainWindow::on_removeAllTasks_action_triggered()
{
    if (m_tomato->rootTask()->childCount() == 0)
        return;

    if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Are you shure you want to remove all tasks?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    m_tomato->removeAllTasks();

    updateTaskActions();
}

void MainWindow::on_hideFinishedTasks_action_toggled(bool hide)
{
    m_taskFilterProxyModel->setEnabled(hide);
}

void MainWindow::on_editTask_action_triggered()
{
    if (ui->task_treeView->selectedTaskCount() != 1)
        return;

    Task *selectedTask = ui->task_treeView->selectedTasks().first();

    TaskEditDialog dialog(this);
    dialog.setWindowTitle(tr("Edit task"));
    dialog.setData(selectedTask->data());
    if (dialog.exec() == QDialog::Accepted)
        m_tomato->setTaskData(selectedTask, dialog.data());

    updateTaskActions();
}

void MainWindow::on_expandAllTasks_action_triggered()
{
    ui->task_treeView->expandAll();
}

void MainWindow::on_collapseAllTasks_action_triggered()
{
    ui->task_treeView->collapseAll();
}

void MainWindow::on_settings_action_triggered()
{
    G_SETTINGS_INIT();

    SettingsDialog dialog(this);
    dialog.setPlayWorkingFinishSound(settings.value(SettingsPlayWorkingFinishSound, true).toBool());
    dialog.setPlayRestingFinishSound(settings.value(SettingsPlayRestingFinishSound, true).toBool());
    dialog.setSaveChangesOnExit(settings.value(SettingsSaveChangesOnExit, true).toBool());
    if (dialog.exec() == QDialog::Accepted) {
        settings.setValue(SettingsPlayWorkingFinishSound, dialog.playWorkingFinishSound());
        settings.setValue(SettingsPlayRestingFinishSound, dialog.playRestingFinishSound());
        settings.setValue(SettingsSaveChangesOnExit, dialog.saveChangesOnExit());
    }
}

void MainWindow::on_about_action_triggered()
{
    AboutDialog dialog(this);
    dialog.setPixmap(QPixmap::fromImage(Theme::appIconImage()));
    dialog.exec();
}

void MainWindow::on_aboutQt_action_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_task_treeView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(ui->activateTask_action);
    menu.addSeparator();
    menu.addAction(ui->addTask_action);
    menu.addAction(ui->editTask_action);
    menu.addAction(ui->removeTask_action);
    menu.addAction(ui->removeAllTasks_action);
    menu.addSeparator();
    menu.addAction(ui->expandAllTasks_action);
    menu.addAction(ui->collapseAllTasks_action);
    menu.exec(ui->task_treeView->viewport()->mapToGlobal(pos));
}

void MainWindow::on_task_treeView_activated(const QModelIndex &index)
{
    m_tomato->setActiveTask(Task::variantToPtr(index.data(Qt::UserRole)));
}

void MainWindow::trayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        if (isHidden())
            show();
        else
            hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *closeEvent)
{
    hide();
    closeEvent->ignore();
}

void MainWindow::updateWindowTitle()
{
    if (m_project->isOpen())
        setWindowTitle(QString("%1 - %2").arg(m_project->fileName(), appShortName()));
    else
        setWindowTitle(appName());
}

void MainWindow::updateProjectActions()
{
    ui->save_action->setEnabled(m_project->hasChanges());
    ui->saveAs_action->setEnabled(m_project->isOpen());
    ui->close_action->setEnabled(m_project->isOpen());
    ui->properties_action->setEnabled(m_project->isOpen());

    ui->centralwidget->setEnabled(m_project->isOpen());
}

void MainWindow::updateTomatoActions()
{
    if (!m_project->isOpen()) {
        ui->startTomato_action->setText(tr("S&tart working"));
        ui->startTomato_action->setToolTip(tr("Start working"));
        ui->startTomato_action->setIconText(tr("Start working"));
        ui->startTomato_action->setIcon(Theme::icon(Theme::IconActionTomatoStartWorking));
        ui->startTomato_action->setEnabled(m_tomato->activeTask());
        ui->stopTomato_action->setEnabled(false);
        return;
    }

    switch (m_tomato->state()) {
    case Tomato::Idle:
        ui->startTomato_action->setText(tr("S&tart working"));
        ui->startTomato_action->setToolTip(tr("Start working"));
        ui->startTomato_action->setIconText(tr("Start working"));
        ui->startTomato_action->setIcon(Theme::icon(Theme::IconActionTomatoStartWorking));
        ui->startTomato_action->setEnabled(m_tomato->activeTask());
        ui->stopTomato_action->setEnabled(false);
        break;
    case Tomato::Working:
        ui->startTomato_action->setText(tr("S&tart resting"));
        ui->startTomato_action->setToolTip(tr("Start resting"));
        ui->startTomato_action->setIconText(tr("Start resting"));
        ui->startTomato_action->setIcon(Theme::icon(Theme::IconActionTomatoStartResting));
        ui->startTomato_action->setEnabled(m_tomato->activeTask());
        ui->stopTomato_action->setEnabled(m_tomato->activeTask());
        break;
    case Tomato::OverWorking:
        ui->startTomato_action->setText(tr("S&tart resting"));
        ui->startTomato_action->setEnabled(m_tomato->activeTask());
        ui->startTomato_action->setToolTip(tr("Start resting"));
        ui->startTomato_action->setIconText(tr("Start resting"));
        ui->startTomato_action->setIcon(Theme::icon(Theme::IconActionTomatoStartResting));
        ui->startTomato_action->setEnabled(m_tomato->activeTask());
        ui->stopTomato_action->setEnabled(m_tomato->activeTask());
        break;
    case Tomato::Resting:
        ui->startTomato_action->setText(tr("S&tart working"));
        ui->startTomato_action->setToolTip(tr("Start working"));
        ui->startTomato_action->setIconText(tr("Start working"));
        ui->startTomato_action->setIcon(Theme::icon(Theme::IconActionTomatoStartWorking));
        ui->startTomato_action->setEnabled(m_tomato->activeTask());
        ui->stopTomato_action->setEnabled(m_tomato->activeTask());
        break;
    case Tomato::OverResting:
        ui->startTomato_action->setText(tr("S&tart working"));
        ui->startTomato_action->setToolTip(tr("Start working"));
        ui->startTomato_action->setIconText(tr("Start working"));
        ui->startTomato_action->setIcon(Theme::icon(Theme::IconActionTomatoStartWorking));
        ui->startTomato_action->setEnabled(m_tomato->activeTask());
        ui->stopTomato_action->setEnabled(m_tomato->activeTask());
        break;
    }
}

void MainWindow::updateTaskActions()
{
    if (!m_project->isOpen()) {
        ui->activateTask_action->setEnabled(false);
        ui->addTask_action->setEnabled(false);
        ui->editTask_action->setEnabled(false);
        ui->removeTask_action->setEnabled(false);
        ui->removeAllTasks_action->setEnabled(false);
        ui->expandAllTasks_action->setEnabled(false);
        ui->collapseAllTasks_action->setEnabled(false);
        return;
    }

    const int rootTaskCount = m_tomato->rootTask()->childCount();
    const int selectedTaskCount = ui->task_treeView->selectedTaskCount();
    const bool selectedTaskCountIsOne = (selectedTaskCount == 1);

    ui->activateTask_action->setEnabled(selectedTaskCountIsOne);
    ui->addTask_action->setEnabled(true);
    ui->editTask_action->setEnabled(selectedTaskCountIsOne);
    ui->removeTask_action->setEnabled(selectedTaskCount > 0);
    ui->removeAllTasks_action->setEnabled(rootTaskCount > 0);
    ui->expandAllTasks_action->setEnabled(rootTaskCount > 0);
    ui->collapseAllTasks_action->setEnabled(rootTaskCount > 0);
}

void MainWindow::updateTrayIcon()
{
    updateTrayStatusText();

    if (!m_project->isOpen()) {
        m_trayIcon->setIcon(Theme::icon(Theme::IconApp));
        return;
    }

    switch (m_tomato->state()) {
    case Tomato::Idle:
        m_trayIcon->setIcon(Theme::icon(Theme::IconApp));
        break;
    case Tomato::Working:
    case Tomato::OverWorking:
        m_trayIcon->setIcon(Theme::icon(Theme::IconActionTomatoStartWorking));
        break;
    case Tomato::Resting:
    case Tomato::OverResting:
        m_trayIcon->setIcon(Theme::icon(Theme::IconActionTomatoStartResting));
        break;
    }
}

void MainWindow::updateTrayStatusText()
{
    if (!m_project->isOpen()) {
        m_trayStatusAction->setText(tr("IDLE"));
        return;
    }

    switch (m_tomato->state()) {
    case Tomato::Idle:
        m_trayStatusAction->setText(tr("IDLE"));
        break;
    case Tomato::Working:
        m_trayStatusAction->setText(tr("WORKING"));
        break;
    case Tomato::OverWorking:
        m_trayStatusAction->setText(tr("OVERWORKING"));
        break;
    case Tomato::Resting:
        m_trayStatusAction->setText(tr("RESTING"));
        break;
    case Tomato::OverResting:
        m_trayStatusAction->setText(tr("OVERRESTING"));
        break;
    }

    qint64 time = 0;
    switch (m_tomato->state()) {
    case Tomato::Idle:
        time = m_tomato->workingTime();
        break;
    case Tomato::Working:
        time = m_tomato->workingTime() - m_tomato->calcTomatoTime();
        break;
    case Tomato::OverWorking:
        time = m_tomato->calcTomatoTime() - m_tomato->workingTime();
        break;
    case Tomato::Resting:
        time = m_tomato->restingTime() - m_tomato->calcTomatoTime();
        break;
    case Tomato::OverResting:
        time = m_tomato->calcTomatoTime() - m_tomato->restingTime();
        break;
    }

    m_trayStatusAction->setText(m_trayStatusAction->text() + ": " + secsToTimeStr(time));
    m_trayIcon->setToolTip(m_trayStatusAction->text());
}

void MainWindow::playSound(Tomato::State state)
{
    G_SETTINGS_INIT();

    bool playWorkingSound = settings.value(SettingsPlayWorkingFinishSound, true).toBool();
    bool playRestingSound = settings.value(SettingsPlayRestingFinishSound, true).toBool();

    if ((playWorkingSound && state == Tomato::OverWorking)
            || (playRestingSound && state == Tomato::OverResting)) {
        Phonon::MediaObject  *music = Phonon::createPlayer(
                    Phonon::MusicCategory,
                    Phonon::MediaSource(":/sound/sound.wav"));
        music->play();
        connect(music, SIGNAL(finished()), music, SLOT(deleteLater()));
    }
}
