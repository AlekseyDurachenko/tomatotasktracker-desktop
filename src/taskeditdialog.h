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
#ifndef TASKEDITDIALOG_H
#define TASKEDITDIALOG_H


#include "taskdata.h"
#include "tasktime.h"
#include <QDialog>
class QTreeWidgetItem;


namespace Ui {
class TaskEditDialog;
}

class TaskEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TaskEditDialog(QWidget *parent = 0);
    virtual ~TaskEditDialog();

    TaskData data() const;
    void setData(const TaskData &data);

    QList<TaskTime> times() const;
    void setTimes(const QList<TaskTime> &times);

private slots:
    void on_add_action_triggered();
    void on_edit_action_triggered();
    void on_remove_action_triggered();
    void on_removeAll_action_triggered();
    void on_taskTime_treeWidget_customContextMenuRequested(const QPoint &pos);

    void openTimeEditDialog(QTreeWidgetItem *item);
    void updateTimeEditActions();

private:
    void initActions();
    void initToolButtons();

private:
    Ui::TaskEditDialog *ui;
};


#endif // TASKEDITDIALOG_H
