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
#include "testtomato.h"
#include "tasktime.h"
#include "taskdata.h"
#include "taskmanager.h"
#include "tomato.h"
#include "consts.h"


TestTomato::TestTomato(QObject *parent)
    : QObject(parent)
{
}

void TestTomato::init()
{
    manager = new TaskManager(0);
    tomato = new Tomato(manager, 0);

    QList<TaskTime> taskTimes;
    taskTimes.append(TaskTime(1, 2));

    item_0 = manager->rootItem()->append(TaskData(), taskTimes);
    item_0_0 = manager->rootItem()->value(0)->append(TaskData(), taskTimes);
    item_0_1 = manager->rootItem()->value(0)->append(TaskData(), taskTimes);
    item_0_1_0 = manager->rootItem()->value(0)->value(1)->append(TaskData(), taskTimes);
    item_0_1_1 = manager->rootItem()->value(0)->value(1)->append(TaskData(), taskTimes);
    item_0_2 = manager->rootItem()->value(0)->append(TaskData(), taskTimes);
    item_1 = manager->rootItem()->append(TaskData(), taskTimes);
    item_1_0 = manager->rootItem()->value(1)->append(TaskData(), taskTimes);
    item_1_0_0 = manager->rootItem()->value(1)->value(0)->append(TaskData(), taskTimes);
    item_1_1 = manager->rootItem()->value(1)->append(TaskData(), taskTimes);
    item_1_1_0 = manager->rootItem()->value(1)->value(1)->append(TaskData(), taskTimes);
    item_2 = manager->rootItem()->append(TaskData(), taskTimes);
    item_2_0 = manager->rootItem()->value(2)->append(TaskData(), taskTimes);
    item_2_0_0 = manager->rootItem()->value(2)->value(0)->append(TaskData(), taskTimes);
    item_3 = manager->rootItem()->append(TaskData(), taskTimes);
}

void TestTomato::cleanup()
{
    delete manager;
}

void TestTomato::testTaskManagerStateAfterCreating()
{
    QVERIFY(manager->rootItem());
    QVERIFY(manager->activeItem() == 0);

    QVERIFY(manager->rootItem()->at(0) == item_0);
    QVERIFY(manager->rootItem()->at(0)->at(0) == item_0_0);
    QVERIFY(manager->rootItem()->at(0)->at(1) == item_0_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0) == item_0_1_0);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1) == item_0_1_1);
    QVERIFY(manager->rootItem()->at(0)->at(2) == item_0_2);
    QVERIFY(manager->rootItem()->at(1) == item_1);
    QVERIFY(manager->rootItem()->at(1)->at(0) == item_1_0);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0) == item_1_0_0);
    QVERIFY(manager->rootItem()->at(1)->at(1) == item_1_1);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0) == item_1_1_0);
    QVERIFY(manager->rootItem()->at(2) == item_2);
    QVERIFY(manager->rootItem()->at(2)->at(0) == item_2_0);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0) == item_2_0_0);
    QVERIFY(manager->rootItem()->at(3) == item_3);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);

    QVERIFY(manager->rootItem()->taskTime() == 0);
    QVERIFY(manager->rootItem()->totalTime() == 15);
    QVERIFY(manager->rootItem()->at(1)->taskTime() == 1);
    QVERIFY(manager->rootItem()->at(1)->totalTime() == 5);
    QVERIFY(manager->rootItem()->at(3)->taskTime() == 1);
    QVERIFY(manager->rootItem()->at(3)->totalTime() == 1);
}

void TestTomato::testTaskManagerFindById()
{
    QVERIFY(manager->findById(item_0->id()) == item_0);
    QVERIFY(manager->findById(item_0_1_0->id()) == item_0_1_0);
    QVERIFY(manager->findById(item_2_0->id()) == item_2_0);
}

void TestTomato::testTaskManagerSetActiveItem()
{
    // test 1
    manager->setActiveItem(item_0);
    QVERIFY(manager->rootItem()->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Active);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);

    // test 2
    manager->setActiveItem(item_0_1_1);
    QVERIFY(manager->rootItem()->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1)->activeState() == TaskItem::Active);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);

    // test 3
    manager->setActiveItem(item_2_0);
    QVERIFY(manager->rootItem()->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Active);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);

    // test 3
    manager->setActiveItem(0);
    QVERIFY(manager->rootItem()->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);
}

void TestTomato::testTaskItemCount()
{
    QVERIFY(manager->rootItem()->count() == 4);
}

void TestTomato::testTaskItemIndexOf()
{
    QVERIFY(manager->rootItem()->at(0)->indexOf(item_0_1) == 1);
}

void TestTomato::testTaskItemRemoveAt()
{
    const int id_0_1_1 = item_0_1_1->id();
    manager->rootItem()->value(0)->value(1)->removeAt(1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->count() == 1);
    QVERIFY(manager->rootItem()->totalTime() == 14);
    QVERIFY(manager->findById(id_0_1_1) == false);

    const int id_2 = item_2->id();
    const int id_2_0 = item_2_0->id();
    const int id_2_0_0 = item_2_0_0->id();
    manager->rootItem()->removeAt(2);
    QVERIFY(manager->rootItem()->count() == 3);
    QVERIFY(manager->rootItem()->totalTime() == 11);
    QVERIFY(manager->findById(id_2) == false);
    QVERIFY(manager->findById(id_2_0) == false);
    QVERIFY(manager->findById(id_2_0_0) == false);
}

void TestTomato::testTaskItemRemoveAll()
{
    manager->rootItem()->value(1)->removeAll();
    QVERIFY(manager->rootItem()->count() == 4);
    QVERIFY(manager->rootItem()->totalTime() == 11);

    manager->rootItem()->removeAll();
    QVERIFY(manager->rootItem()->count() == 0);
    QVERIFY(manager->rootItem()->totalTime() == 0);
}

void TestTomato::testTaskItemRemoveAtWithActive()
{
    manager->setActiveItem(item_0_1_0);
    manager->rootItem()->value(0)->value(1)->removeAt(1);
    QVERIFY(manager->activeItem() == item_0_1_0);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Active);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);

    manager->setActiveItem(item_0_1_0);
    manager->rootItem()->value(0)->value(1)->removeAt(0);
    QVERIFY(manager->activeItem() == 0);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);

    manager->setActiveItem(item_1_1);
    manager->rootItem()->value(1)->value(1)->removeAt(0);
    QVERIFY(manager->activeItem() == item_1_1);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Active);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);
}

void TestTomato::testTaskItemRemoveAllWithActive()
{
    manager->setActiveItem(item_0_1_0);
    manager->rootItem()->value(0)->value(1)->removeAll();
    QVERIFY(manager->activeItem() == 0);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);

    manager->setActiveItem(item_0);
    manager->rootItem()->value(0)->removeAll();
    QVERIFY(manager->activeItem() == item_0);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Active);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);

    manager->setActiveItem(item_0);
    manager->rootItem()->removeAll();
    QVERIFY(manager->activeItem() == 0);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::Inactive);
}

void TestTomato::testTaskItemSetDateAndTimes()
{
    QList<TaskTime> taskTimes;
    taskTimes.append(TaskTime(10, 20));
    taskTimes.append(TaskTime(20, 30));

    manager->rootItem()->value(1)->value(1)->setData(TaskData(), taskTimes);
    QVERIFY(manager->rootItem()->at(1)->at(1)->taskTime() == 20);
    QVERIFY(manager->rootItem()->at(1)->at(1)->totalTime() == 21);
    QVERIFY(manager->rootItem()->taskTime() == 0);
    QVERIFY(manager->rootItem()->totalTime() == 34);
}

void TestTomato::testTaskItemSetTimes()
{
    QList<TaskTime> taskTimes;
    taskTimes.append(TaskTime(10, 20));
    taskTimes.append(TaskTime(20, 30));

    manager->rootItem()->value(2)->value(0)->value(0)->setTimes(taskTimes);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->taskTime() == 20);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->totalTime() == 20);
    QVERIFY(manager->rootItem()->taskTime() == 0);
    QVERIFY(manager->rootItem()->totalTime() == 34);
}

void TestTomato::testTaskItemAppendTimes()
{
    manager->rootItem()->value(2)->value(0)->appendTime(TaskTime(10, 30));
    QVERIFY(manager->rootItem()->at(2)->at(0)->taskTime() == 21);
    QVERIFY(manager->rootItem()->at(2)->at(0)->totalTime() == 22);
    QVERIFY(manager->rootItem()->taskTime() == 0);
    QVERIFY(manager->rootItem()->totalTime() == 35);
}

void TestTomato::testTaskItemCanMoveTo()
{
    QVERIFY(item_0->canMoveTo(item_0_0, -1) == false);
    QVERIFY(item_0->canMoveTo(item_0_0, 0) == false);
    QVERIFY(item_0->canMoveTo(item_1, -1) == true);
    QVERIFY(item_0->canMoveTo(item_1, 0) == true);
    QVERIFY(item_0->canMoveTo(item_1, 1) == true);
    QVERIFY(item_0->canMoveTo(item_1, 2) == true);
    QVERIFY(item_0->canMoveTo(item_1, 3) == false);
    QVERIFY(item_1_1->canMoveTo(item_1, 0) == true);
    QVERIFY(item_1_1->canMoveTo(item_1, 1) == false);
    QVERIFY(item_1_1->canMoveTo(item_1, 2) == false);
    QVERIFY(item_1->canMoveTo(manager->rootItem(), 0) == true);
    QVERIFY(item_1->canMoveTo(manager->rootItem(), 1) == false);
    QVERIFY(item_1->canMoveTo(manager->rootItem(), 2) == false);
    QVERIFY(item_1->canMoveTo(manager->rootItem(), 3) == true);
    QVERIFY(item_1->canMoveTo(manager->rootItem(), 4) == true);
    QVERIFY(item_1->canMoveTo(manager->rootItem(), 5) == false);
}

void TestTomato::testTaskItemMoveTo1()
{
    QVERIFY(item_0_1_0->moveTo(item_0_1_1));

    QVERIFY(manager->rootItem()->at(0) == item_0);
    QVERIFY(manager->rootItem()->at(0)->at(0) == item_0_0);
    QVERIFY(manager->rootItem()->at(0)->at(1) == item_0_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0) == item_0_1_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->at(0) == item_0_1_0);
    QVERIFY(manager->rootItem()->at(0)->at(2) == item_0_2);
    QVERIFY(manager->rootItem()->at(1) == item_1);
    QVERIFY(manager->rootItem()->at(1)->at(0) == item_1_0);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0) == item_1_0_0);
    QVERIFY(manager->rootItem()->at(1)->at(1) == item_1_1);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0) == item_1_1_0);
    QVERIFY(manager->rootItem()->at(2) == item_2);
    QVERIFY(manager->rootItem()->at(2)->at(0) == item_2_0);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0) == item_2_0_0);
    QVERIFY(manager->rootItem()->at(3) == item_3);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(3)->activeState() == TaskItem::Inactive);
}

void TestTomato::testTaskItemMoveTo2()
{
    QVERIFY(item_3->moveTo(item_0_1));
    QVERIFY(manager->rootItem()->count() == 3);

    QVERIFY(manager->rootItem()->at(0) == item_0);
    QVERIFY(manager->rootItem()->at(0)->at(0) == item_0_0);
    QVERIFY(manager->rootItem()->at(0)->at(1) == item_0_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0) == item_0_1_0);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1) == item_0_1_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(2) == item_3);
    QVERIFY(manager->rootItem()->at(0)->at(2) == item_0_2);
    QVERIFY(manager->rootItem()->at(1) == item_1);
    QVERIFY(manager->rootItem()->at(1)->at(0) == item_1_0);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0) == item_1_0_0);
    QVERIFY(manager->rootItem()->at(1)->at(1) == item_1_1);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0) == item_1_1_0);
    QVERIFY(manager->rootItem()->at(2) == item_2);
    QVERIFY(manager->rootItem()->at(2)->at(0) == item_2_0);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0) == item_2_0_0);
}

void TestTomato::testTaskItemMoveTo3()
{
    QVERIFY(item_3->moveTo(item_0_1, 0));
    QVERIFY(manager->rootItem()->count() == 3);

    QVERIFY(manager->rootItem()->at(0) == item_0);
    QVERIFY(manager->rootItem()->at(0)->at(0) == item_0_0);
    QVERIFY(manager->rootItem()->at(0)->at(1) == item_0_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0) == item_3);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1) == item_0_1_0);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(2) == item_0_1_1);
    QVERIFY(manager->rootItem()->at(0)->at(2) == item_0_2);
    QVERIFY(manager->rootItem()->at(1) == item_1);
    QVERIFY(manager->rootItem()->at(1)->at(0) == item_1_0);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0) == item_1_0_0);
    QVERIFY(manager->rootItem()->at(1)->at(1) == item_1_1);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0) == item_1_1_0);
    QVERIFY(manager->rootItem()->at(2) == item_2);
    QVERIFY(manager->rootItem()->at(2)->at(0) == item_2_0);
    QVERIFY(manager->rootItem()->at(2)->at(0)->at(0) == item_2_0_0);
}

void TestTomato::testTaskItemMoveTo4()
{
    QVERIFY(item_2->moveTo(item_0_2));
    QVERIFY(manager->rootItem()->count() == 3);

    QVERIFY(manager->rootItem()->at(0) == item_0);
    QVERIFY(manager->rootItem()->at(0)->at(0) == item_0_0);
    QVERIFY(manager->rootItem()->at(0)->at(1) == item_0_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0) == item_0_1_0);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1) == item_0_1_1);
    QVERIFY(manager->rootItem()->at(0)->at(2) == item_0_2);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0) == item_2);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0) == item_2_0);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0)->at(0) == item_2_0_0);
    QVERIFY(manager->rootItem()->at(1) == item_1);
    QVERIFY(manager->rootItem()->at(1)->at(0) == item_1_0);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0) == item_1_0_0);
    QVERIFY(manager->rootItem()->at(1)->at(1) == item_1_1);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0) == item_1_1_0);
    QVERIFY(manager->rootItem()->at(2) == item_3);
}

void TestTomato::testTaskItemMoveTo5()
{
    manager->setActiveItem(item_2);

    QVERIFY(item_2->moveTo(item_0_2));
    QVERIFY(manager->rootItem()->count() == 3);

    QVERIFY(manager->rootItem()->at(0) == item_0);
    QVERIFY(manager->rootItem()->at(0)->at(0) == item_0_0);
    QVERIFY(manager->rootItem()->at(0)->at(1) == item_0_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0) == item_0_1_0);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1) == item_0_1_1);
    QVERIFY(manager->rootItem()->at(0)->at(2) == item_0_2);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0) == item_2);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0) == item_2_0);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0)->at(0) == item_2_0_0);
    QVERIFY(manager->rootItem()->at(1) == item_1);
    QVERIFY(manager->rootItem()->at(1)->at(0) == item_1_0);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0) == item_1_0_0);
    QVERIFY(manager->rootItem()->at(1)->at(1) == item_1_1);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0) == item_1_1_0);
    QVERIFY(manager->rootItem()->at(2) == item_3);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->activeState() == TaskItem::Active);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
}

void TestTomato::testTaskItemMoveTo6()
{
    manager->setActiveItem(item_2_0_0);

    QVERIFY(item_2->moveTo(item_0_2));
    QVERIFY(manager->rootItem()->count() == 3);

    QVERIFY(manager->rootItem()->at(0) == item_0);
    QVERIFY(manager->rootItem()->at(0)->at(0) == item_0_0);
    QVERIFY(manager->rootItem()->at(0)->at(1) == item_0_1);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0) == item_0_1_0);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1) == item_0_1_1);
    QVERIFY(manager->rootItem()->at(0)->at(2) == item_0_2);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0) == item_2);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0) == item_2_0);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0)->at(0) == item_2_0_0);
    QVERIFY(manager->rootItem()->at(1) == item_1);
    QVERIFY(manager->rootItem()->at(1)->at(0) == item_1_0);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0) == item_1_0_0);
    QVERIFY(manager->rootItem()->at(1)->at(1) == item_1_1);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0) == item_1_1_0);
    QVERIFY(manager->rootItem()->at(2) == item_3);

    QVERIFY(manager->rootItem()->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0)->activeState() == TaskItem::SubActive);
    QVERIFY(manager->rootItem()->at(0)->at(2)->at(0)->at(0)->at(0)->activeState() == TaskItem::Active);
    QVERIFY(manager->rootItem()->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(0)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(1)->at(1)->at(0)->activeState() == TaskItem::Inactive);
    QVERIFY(manager->rootItem()->at(2)->activeState() == TaskItem::Inactive);
}

void TestTomato::testTaskManagerStates()
{
    QVERIFY(manager->state() == TaskManager::Idle);


    manager->startWorking();
    QVERIFY(manager->state() == TaskManager::Idle);
    manager->startResting();
    QVERIFY(manager->state() == TaskManager::Idle);
    manager->startIdle();
    QVERIFY(manager->state() == TaskManager::Idle);


    manager->setActiveItem(item_0_1_0);
    QVERIFY(manager->state() == TaskManager::Idle);

    manager->startWorking();
    QVERIFY(manager->state() == TaskManager::Working);
    manager->startResting();
    QVERIFY(manager->state() == TaskManager::Resting);
    manager->startIdle();
    QVERIFY(manager->state() == TaskManager::Idle);


    manager->startWorking();
    manager->rootItem()->value(0)->value(1)->removeAt(0);
    QVERIFY(manager->state() == TaskManager::Idle);

    manager->setActiveItem(item_1_1_0);
    manager->startWorking();
    manager->rootItem()->value(1)->removeAt(1);
    QVERIFY(manager->state() == TaskManager::Idle);

}

void TestTomato::testTaskManagerCalcLastStateTime()
{
    QVERIFY(manager->calcLastStateTime() == 0);


    qint64 total = item_0_1_0->totalTime();
    manager->setActiveItem(item_0_1_0);
    manager->startWorking();
    QTest::qWait(1400);
    QVERIFY(manager->calcLastStateTime() >= 1 && manager->calcLastStateTime() <= 2);
    manager->startWorking();
    QVERIFY(item_0_1_0->totalTime() >= total + 1 && item_0_1_0->totalTime() <= total + 2);
    total = item_0_1_0->totalTime();
    QTest::qWait(1400);
    QVERIFY(manager->calcLastStateTime() >= 1 && manager->calcLastStateTime() <= 2);
    manager->startResting();
    QVERIFY(item_0_1_0->totalTime() >= total + 1 && item_0_1_0->totalTime() <= total + 2);
    QTest::qWait(1400);
    QVERIFY(manager->calcLastStateTime() >= 1 && manager->calcLastStateTime() <= 2);
    manager->startIdle();
    QVERIFY(manager->calcLastStateTime() == 0);
    manager->startResting();
    QTest::qWait(1400);
    QVERIFY(manager->calcLastStateTime() >= 1 && manager->calcLastStateTime() <= 2);
    manager->startResting();
    QTest::qWait(1400);
    QVERIFY(manager->calcLastStateTime() >= 1 && manager->calcLastStateTime() <= 2);
}

void TestTomato::testTaskManagerCalcTaskTime()
{
    QVERIFY(manager->calcTaskTime(0) == 0);


    QVERIFY(manager->calcTaskTime(item_0_1_0) == 1);
    manager->setActiveItem(item_1_0_0);
    manager->startWorking();
    QTest::qWait(1400);
    QVERIFY(manager->calcTaskTime(item_0_1_0) == 1);
    manager->setActiveItem(item_0_1_0);
    QVERIFY(manager->calcTaskTime(item_0_1_0) >= 1 && manager->calcTaskTime(item_0_1_0) <= 2);
    QTest::qWait(1400);
    QVERIFY(manager->calcTaskTime(item_0_1_0) >= 2 && manager->calcTaskTime(item_0_1_0) <= 3);
    QVERIFY(manager->calcTaskTime(item_0_1) >= 4 && manager->calcTaskTime(item_0_1) <= 5);
}

void TestTomato::testTomatoTimestamp()
{
    QVERIFY(qAbs(Tomato::timestamp() - QDateTime::currentDateTimeUtc().toTime_t()) <= 1);
}

void TestTomato::testTomatoAfterCreating()
{
    QVERIFY(tomato->state() == Tomato::Idle);
    QVERIFY(tomato->workingTime() == DefaultWorkingTime);
    QVERIFY(tomato->restingTime() == DefaultRestingTime);

    tomato->setWorkingTime(10);
    QVERIFY(tomato->workingTime() == 10);

    tomato->setRestingTime(111);
    QVERIFY(tomato->restingTime() == 111);
}

void TestTomato::testTomatoSwitchState()
{
    QVERIFY(tomato->state() == Tomato::Idle);
    manager->setActiveItem(item_0);
    manager->startWorking();
    QVERIFY(tomato->state() == Tomato::Working);
    manager->startResting();
    QVERIFY(tomato->state() == Tomato::Resting);
    manager->startIdle();
    QVERIFY(tomato->state() == Tomato::Idle);
    manager->startResting();
    QVERIFY(tomato->state() == Tomato::Resting);
    manager->startIdle();
    QVERIFY(tomato->state() == Tomato::Idle);

    tomato->setWorkingTime(1);
    tomato->setRestingTime(1);
    QVERIFY(tomato->state() == Tomato::Idle);
    manager->startWorking();
    QVERIFY(tomato->state() == Tomato::Working);
    QTest::qWait(1500);
    QVERIFY(tomato->state() == Tomato::OverWorking);
    tomato->setWorkingTime(10);
    QVERIFY(tomato->state() == Tomato::Working);
    manager->startResting();
    QVERIFY(tomato->state() == Tomato::Resting);
    QTest::qWait(1500);
    QVERIFY(tomato->state() == Tomato::OverResting);
    tomato->setRestingTime(10);
    QVERIFY(tomato->state() == Tomato::Resting);

    manager->startIdle();
    tomato->setWorkingTime(1);
    tomato->setRestingTime(1);
    QVERIFY(tomato->state() == Tomato::Idle);
    manager->startWorking();
    QVERIFY(tomato->state() == Tomato::Working);
    tomato->setWorkingTime(10);
    QTest::qWait(1500);
    QVERIFY(tomato->state() == Tomato::Working);
    manager->startResting();
    QVERIFY(tomato->state() == Tomato::Resting);
    QTest::qWait(1500);
    tomato->setRestingTime(10);
    QVERIFY(tomato->state() == Tomato::Resting);
}
