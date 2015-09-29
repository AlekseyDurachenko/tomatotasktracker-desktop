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
#include "taskeditdialog.h"
#include "consts.h"
#include "theme.h"
#include "tasktimeeditdialog.h"
#include "ui_taskeditdialog.h"
#include <QMessageBox>
#include <QMenu>


TaskEditDialog::TaskEditDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::TaskEditDialog)
{
    ui->setupUi(this);
    initActions();
    initToolButtons();

    connect(ui->taskTime_treeWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(updateActions()));
    connect(ui->taskTime_treeWidget, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(editTaskTime(QTreeWidgetItem*)));

    updateActions();
}

TaskEditDialog::~TaskEditDialog()
{
    delete ui;
}

TaskData TaskEditDialog::data() const
{
    TaskData data;
    data.setTitle(ui->title_lineEdit->text());
    data.setDesc(ui->desc_plainTextEdit->toPlainText());
    if (ui->useTimeLimit_checkBox->isChecked()) {
        data.setTimeLimit(ui->hour_spinBox->value() * 3600
                          + ui->min_spinBox->value() * 60
                          + ui->sec_spinBox->value());
    } else {
        data.setTimeLimit(0);
    }
    data.setDone(ui->done_checkBox->isChecked());

    QList<TaskTime> taskTimeList;
    for (int i = 0; i < ui->taskTime_treeWidget->topLevelItemCount(); ++i) {
        const QString c0 = ui->taskTime_treeWidget->topLevelItem(i)->text(0);
        const QString c1 = ui->taskTime_treeWidget->topLevelItem(i)->text(1);
        const qint64 startTime = QDateTime::fromString(c0, DisplayTaskTimeFormat).toTime_t();
        const qint64 endTime = QDateTime::fromString(c1, DisplayTaskTimeFormat).toTime_t();
        taskTimeList.push_back(TaskTime(startTime, endTime));
    }
    data.setTimes(taskTimeList);

    return data;
}

void TaskEditDialog::setData(const TaskData &taskData)
{
    ui->title_lineEdit->setText(taskData.title());
    ui->desc_plainTextEdit->setPlainText(taskData.desc());
    if (taskData.timeLimit()) {
        ui->useTimeLimit_checkBox->setChecked(true);
        ui->hour_spinBox->setValue(taskData.timeLimit() / 3600);
        ui->min_spinBox->setValue((taskData.timeLimit() % 3600) / 60);
        ui->sec_spinBox->setValue(taskData.timeLimit() % 60);
    }
    ui->done_checkBox->setChecked(taskData.isDone());

    foreach (const TaskTime &taskTime, taskData.times()) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, QDateTime::fromTime_t(taskTime.start()).toString(DisplayTaskTimeFormat));
        item->setText(1, QDateTime::fromTime_t(taskTime.end()).toString(DisplayTaskTimeFormat));
        ui->taskTime_treeWidget->addTopLevelItem(item);
    }
}

void TaskEditDialog::on_add_action_triggered()
{
    TaskTimeEditDialog dialog(this);
    dialog.setWindowTitle(tr("Add task time interval"));
    if (dialog.exec() == QDialog::Accepted) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, dialog.startDateTime().toString(DisplayTaskTimeFormat));
        item->setText(1, dialog.endDateTime().toString(DisplayTaskTimeFormat));
        ui->taskTime_treeWidget->addTopLevelItem(item);
    }
}

void TaskEditDialog::on_edit_action_triggered()
{
    editTaskTime(ui->taskTime_treeWidget->selectedItems().first());
}

void TaskEditDialog::on_remove_action_triggered()
{
    if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Are you sure you want to delete selected intervals?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }

    qDeleteAll(ui->taskTime_treeWidget->selectedItems());
}

void TaskEditDialog::on_removeAll_action_triggered()
{
    if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Are you sure you want to delete all time intervals?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }

    ui->taskTime_treeWidget->clear();
}

void TaskEditDialog::on_taskTime_treeWidget_customContextMenuRequested(
        const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(ui->add_action);
    menu.addAction(ui->edit_action);
    menu.addAction(ui->remove_action);
    menu.addAction(ui->removeAll_action);
    menu.exec(ui->taskTime_treeWidget->viewport()->mapToGlobal(pos));
}

void TaskEditDialog::updateActions()
{
    const int count = ui->taskTime_treeWidget->topLevelItemCount();
    const int selectedCount = ui->taskTime_treeWidget->selectedItems().count();

    ui->edit_action->setEnabled(selectedCount == 1);
    ui->remove_action->setEnabled(selectedCount);
    ui->removeAll_action->setEnabled(count);
}

void TaskEditDialog::editTaskTime(QTreeWidgetItem *item)
{
    TaskTimeEditDialog dialog(this);
    dialog.setWindowTitle(tr("Edit task time interval"));
    dialog.setStartDateTime(QDateTime::fromString(item->text(0), DisplayTaskTimeFormat));
    dialog.setEndDateTime(QDateTime::fromString(item->text(1), DisplayTaskTimeFormat));
    if (dialog.exec() == QDialog::Accepted) {
        item->setText(0, dialog.startDateTime().toString(DisplayTaskTimeFormat));
        item->setText(1, dialog.endDateTime().toString(DisplayTaskTimeFormat));
    }
}

void TaskEditDialog::initActions()
{
    ui->add_action->setIcon(Theme::icon(Theme::IconActionTaskTimeAdd));
    ui->edit_action->setIcon(Theme::icon(Theme::IconActionTaskTimeEdit));
    ui->remove_action->setIcon(Theme::icon(Theme::IconActionTaskTimeRemove));
    ui->removeAll_action->setIcon(Theme::icon(Theme::IconActionTaskTimeRemoveAll));
}

void TaskEditDialog::initToolButtons()
{
    ui->add_toolButton->setDefaultAction(ui->add_action);
    ui->edit_toolButton->setDefaultAction(ui->edit_action);
    ui->remove_toolButton->setDefaultAction(ui->remove_action);
    ui->removeAll_toolButton->setDefaultAction(ui->removeAll_action);
}
