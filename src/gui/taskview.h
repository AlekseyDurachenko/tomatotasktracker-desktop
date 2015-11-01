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
#ifndef TASKVIEW_H
#define TASKVIEW_H


#include <QTreeView>
class Task;
class Project;
class TaskItemModel;
class TaskFilterProxyModel;


class TaskView : public QTreeView
{
    Q_OBJECT
public:
    explicit TaskView(Project *project, QWidget *parent = 0);

    inline QAction *activateAction() const;
    inline QAction *addAction() const;
    inline QAction *editAction() const;
    inline QAction *removeAction() const;
    inline QAction *removeAllAction() const;
    inline QAction *hideCompletedAction() const;
    inline QAction *expandAllAction() const;
    inline QAction *collapseAllAction() const;

private slots:
    void activateAction_triggered();
    void addAction_triggered();
    void editAction_triggered();
    void removeAction_triggered();
    void removeAllAction_triggered();
    void hideCompletedAction_toggled(bool hide);
    void expandAllAction_triggered();
    void collapseAllAction_triggered();
    void itemActivated(const QModelIndex &index);
    void customContextMenu(const QPoint &pos);

private slots:
    void updateActions();

private:
    void createActions();
    QList<int> selectedIds() const;

private:
    Project *m_project;

    TaskItemModel *m_itemModel;
    TaskFilterProxyModel *m_filterProxyModel;

    QAction *m_activateAction;
    QAction *m_addAction;
    QAction *m_editAction;
    QAction *m_removeAction;
    QAction *m_removeAllAction;
    QAction *m_hideCompletedAction;
    QAction *m_expandAllAction;
    QAction *m_collapseAllAction;
};

QAction *TaskView::activateAction() const
{
    return m_activateAction;
}

QAction *TaskView::addAction() const
{
    return m_addAction;
}

QAction *TaskView::editAction() const
{
    return m_editAction;
}

QAction *TaskView::removeAction() const
{
    return m_removeAction;
}

QAction *TaskView::removeAllAction() const
{
    return m_removeAllAction;
}

QAction *TaskView::hideCompletedAction() const
{
    return m_hideCompletedAction;
}

QAction *TaskView::expandAllAction() const
{
    return m_expandAllAction;
}

QAction *TaskView::collapseAllAction() const
{
    return m_collapseAllAction;
}


#endif // TASKVIEW_H
