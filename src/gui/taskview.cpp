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
#include "taskview.h"
#include "theme.h"
#include "task.h"
#include "project.h"
#include "taskitemmodel.h"
#include "taskfilterproxymodel.h"
#include "taskeditdialog.h"
#include <QAction>
#include <QMenu>
#include <QMessageBox>


TaskView::TaskView(Project *project, QWidget *parent)
    : QTreeView(parent)
    , m_project(project)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    setDragDropMode(DragDrop);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customContextMenu(QPoint)));

    m_itemModel = new TaskItemModel(project->tomato(), this);
    m_filterProxyModel = new TaskFilterProxyModel(project->tomato(), this);
    m_filterProxyModel->setDynamicSortFilter(true);
    m_filterProxyModel->setSourceModel(m_itemModel);
    setModel(m_filterProxyModel);

    connect(m_project, SIGNAL(openStateChanged()),
            this, SLOT(updateActions()));
    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(updateActions()));
    connect(this, SIGNAL(activated(QModelIndex)),
            this, SLOT(itemActivated(QModelIndex)));

    createActions();
    updateActions();
}

void TaskView::createActions()
{
    m_activateAction = new QAction(this);
    m_activateAction->setText(tr("Act&ivate..."));
    connect(m_activateAction, SIGNAL(triggered(bool)),
            this, SLOT(activateAction_triggered()));

    m_addAction = new QAction(this);
    m_addAction->setText(tr("&Add..."));
    m_addAction->setIcon(theme::icon(theme::IconActionTaskAdd));
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
}

QList<int> TaskView::selectedIds() const
{
    QList<int> ids;
    foreach (const QModelIndex &index, selectionModel()->selectedRows()) {
        ids.push_back(index.data(Qt::UserRole).toInt());
    }

    return ids;
}

void TaskView::activateAction_triggered()
{
    m_project->tomato()->setActiveTask(selectedIds().first());
}

void TaskView::addAction_triggered()
{
    int taskId = m_project->tomato()->rootTaskId();
    if (selectedIds().count() > 0) {
        taskId = selectedIds().first();
    }

    TaskEditDialog dialog(this);
    dialog.setWindowTitle(tr("New task"));
    if (dialog.exec() == QDialog::Accepted) {
        m_project->tomato()->addTask(taskId, dialog.data());
    }
}

void TaskView::editAction_triggered()
{
    const int taskId = selectedIds().first();

    TaskEditDialog dialog(this);
    dialog.setWindowTitle(tr("Edit task"));
    dialog.setData(m_project->tomato()->taskData(taskId));
    if (dialog.exec() == QDialog::Accepted) {
        m_project->tomato()->setTaskData(taskId, dialog.data());
    }
}

void TaskView::removeAction_triggered()
{
    if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Are you shure you want to remove the selected tasks?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    m_project->tomato()->removeTasks(selectedIds());
}

void TaskView::removeAllAction_triggered()
{
    if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Are you shure you want to remove all tasks?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    m_project->tomato()->removeAllTasks();
}

void TaskView::hideCompletedAction_toggled(bool hide)
{
    m_filterProxyModel->setEnabled(hide);
}

void TaskView::expandAllAction_triggered()
{
    expandAll();
}

void TaskView::collapseAllAction_triggered()
{
    collapseAll();
}

void TaskView::itemActivated(const QModelIndex &index)
{
    m_project->tomato()->setActiveTask(index.data(Qt::UserRole).toInt());
}

void TaskView::customContextMenu(const QPoint &pos)
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
    menu.exec(viewport()->mapToGlobal(pos));
}

void TaskView::updateActions()
{
    if (!m_project->isOpen()) {
        m_activateAction->setEnabled(false);
        m_addAction->setEnabled(false);
        m_editAction->setEnabled(false);
        m_removeAction->setEnabled(false);
        m_removeAllAction->setEnabled(false);
        m_hideCompletedAction->setEnabled(false);
        m_expandAllAction->setEnabled(false);
        m_collapseAllAction->setEnabled(false);
        return;
    }

    const int selectedCount = selectionModel()->selectedRows().count();
    const int rootItemCount = m_project->tomato()->rootTaskCount();;
    m_activateAction->setEnabled(selectedCount == 1);
    m_addAction->setEnabled(selectedCount <= 1);
    m_editAction->setEnabled(selectedCount == 1);
    m_removeAction->setEnabled(selectedCount > 0);
    m_removeAllAction->setEnabled(rootItemCount > 0);
    m_hideCompletedAction->setEnabled(true);
    m_expandAllAction->setEnabled(rootItemCount > 0);
    m_collapseAllAction->setEnabled(rootItemCount > 0);
}
