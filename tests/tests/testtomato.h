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
#ifndef TESTTASKMANAGER_H
#define TESTTASKMANAGER_H


#include <QObject>
#include <QtTest/QtTest>
class Tomato;
class TaskManager;
class TaskItem;


class TestTomato : public QObject
{
    Q_OBJECT
public:
    explicit TestTomato(QObject *parent = 0);

private slots:
    void init();
    void cleanup();
    void testTaskManagerStateAfterCreating();
    void testTaskManagerFindById();
    void testTaskManagerSetActiveItem();
    void testTaskItemCount();
    void testTaskItemIndexOf();
    void testTaskItemRemoveAt();
    void testTaskItemRemoveAll();
    void testTaskItemRemoveAtWithActive();
    void testTaskItemRemoveAllWithActive();
    void testTaskItemSetDateAndTimes();
    void testTaskItemSetTimes();
    void testTaskItemAppendTimes();
    void testTaskItemCanMoveTo();
    void testTaskItemMoveTo1();
    void testTaskItemMoveTo2();
    void testTaskItemMoveTo3();
    void testTaskItemMoveTo4();
    void testTaskItemMoveTo5();
    void testTaskItemMoveTo6();
    void testTaskManagerStates();
    void testTaskManagerCalcLastStateTime();
    void testTaskManagerCalcTaskTime();
    void testTomatoTimestamp();
    void testTomatoAfterCreating();
    void testTomatoSwitchState();

private:
    Tomato *tomato;
    TaskManager *manager;
    TaskItem *item_0;
    TaskItem *item_0_0;
    TaskItem *item_0_1;
    TaskItem *item_0_1_0;
    TaskItem *item_0_1_1;
    TaskItem *item_0_2;
    TaskItem *item_1;
    TaskItem *item_1_0;
    TaskItem *item_1_0_0;
    TaskItem *item_1_1;
    TaskItem *item_1_1_0;
    TaskItem *item_2;
    TaskItem *item_2_0;
    TaskItem *item_2_0_0;
    TaskItem *item_3;
};


#endif // TESTTASKMANAGER_H
