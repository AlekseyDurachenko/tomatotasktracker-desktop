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
#include "testdatatype.h"
#include "tasktime.h"
#include "taskdata.h"


TestDatatype::TestDatatype(QObject *parent)
    : QObject(parent)
{
}

void TestDatatype::TestTaskTime()
{
    TaskTime data1;
    QVERIFY(data1.isNull());

    TaskTime data2(-10, 10);
    QVERIFY(data2.isNull());

    TaskTime data3(10, -10);
    QVERIFY(data3.isNull());

    TaskTime data4(10, 22);
    QVERIFY(!data4.isNull());
    QVERIFY(data4.start() == 10);
    QVERIFY(data4.end() == 22);
    QVERIFY(data4.total() == 12);

    TaskTime data5(22, 10);
    QVERIFY(!data5.isNull());
    QVERIFY(data5.start() == 10);
    QVERIFY(data5.end() == 22);
    QVERIFY(data5.total() == 12);

    TaskTime data6(10, 22);
    QVERIFY(!(data3 == data4));
    QVERIFY(data1 == data2);
    QVERIFY(data5 == data6);
    QVERIFY(data3 != data4);
    QVERIFY(!(data1 != data2));
    QVERIFY(!(data5 != data6));
}

void TestDatatype::TestTaskData()
{
    TaskData data1;
    QVERIFY(data1.title().isEmpty());
    QVERIFY(data1.desc().isEmpty());
    QVERIFY(data1.timeLimit() == 0);
    QVERIFY(data1.isCompleted() == false);

    data1.setTimeLimit(-10);
    QVERIFY(data1.timeLimit() == 0);

    data1.setTitle("1");
    data1.setDesc("2");
    data1.setTimeLimit(10);
    data1.setCompleted(true);
    QVERIFY(data1.title() == "1");
    QVERIFY(data1.desc() == "2");
    QVERIFY(data1.timeLimit() == 10);
    QVERIFY(data1.isCompleted() == true);

    TaskData data2;
    data2.setTitle("1");
    data2.setDesc("2");
    data2.setTimeLimit(10);
    data2.setCompleted(true);

    TaskData data3;
    data3.setTitle("4");
    data3.setDesc("5");
    data3.setTimeLimit(1230);
    data3.setCompleted(false);

    QVERIFY(data1 == data2);
    QVERIFY(!(data1 == data3));
    QVERIFY(!(data1 != data2));
    QVERIFY(data1 != data3);
}
