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
#include "projectxml.h"
#include "consts.h"
#include "tomato.h"
#include <QDomDocument>
#include <QFile>


const char *nsDocType   = "tomatotaskstracker-1.0";
const char *nsProject   = "TomatoTaskTracker";
const char *nsSettings      = "Settings";
const char *nsRootTask      = "RootTask";
const char *nsTask              = "Task";
const char *nsTaskTime              = "Time";

const char *attrWorkingTime = "workingtime";
const char *attrRestingTime = "restingtime";
const char *attrStartTime   = "starttime";
const char *attrEndTime     = "endtime";
const char *attrTitle       = "title";
const char *attrDesc        = "desc";
const char *attrTimeLimit   = "timelimit";
const char *attrIsComplited = "completed";


static QDomElement createSettingsElem(QDomDocument doc, const Tomato *tomato)
{
    QDomElement elem = doc.createElement(nsSettings);
    elem.setAttribute(attrWorkingTime, tomato->workingTime());
    elem.setAttribute(attrRestingTime, tomato->restingTime());

    return elem;
}

static QDomElement createTaskElem(QDomDocument doc,
                                  const Tomato *tomato,
                                  const TaskItem *task)
{
    QDomElement elem = doc.createElement(nsTask);
    elem = doc.createElement(nsTask);
    elem.setAttribute(attrTitle, task->data().title());
    elem.setAttribute(attrDesc, task->data().desc());
    elem.setAttribute(attrTimeLimit, task->data().timeLimit());
    elem.setAttribute(attrIsComplited, task->data().isCompleted());

    foreach (const TaskTime &taskTime, task->times()) {
        QDomElement taskTimeElem = doc.createElement(nsTaskTime);
        taskTimeElem.setAttribute(attrStartTime, taskTime.start());
        taskTimeElem.setAttribute(attrEndTime, taskTime.end());
        elem.appendChild(taskTimeElem);
    }

    if (task->activeState() == TaskItem::Active
            && (tomato->state() == Tomato::Working
                || tomato->state() == Tomato::OverWorking)) {
        const TaskTime taskTime = task->manager()->calcLastStateTaskTime();
        if (!taskTime.isNull()) {
            QDomElement taskTimeElem = doc.createElement(nsTaskTime);
            taskTimeElem.setAttribute(attrStartTime, taskTime.start());
            taskTimeElem.setAttribute(attrEndTime, taskTime.end());
            elem.appendChild(taskTimeElem);
        }
    }

    for (int i = 0; i < task->count(); ++i) {
        elem.appendChild(createTaskElem(doc, tomato, task->at(i)));
    }

    return elem;
}

static QDomElement createRootTaskElem(QDomDocument doc,
                                      const Tomato *tomato,
                                      TaskItem *rootTask)
{
    QDomElement rootTaskElem = doc.createElement(nsRootTask);
    for (int i = 0; i < rootTask->count(); ++i) {
        rootTaskElem.appendChild(createTaskElem(doc, tomato, rootTask->at(i)));
    }

    return rootTaskElem;
}

static QList<TaskTime> taskTimesFromElem(const QDomElement &taskElem)
{
    QList<TaskTime> taskTimes;

    QDomNode taskTimeNode = taskElem.firstChild();
    while (!taskTimeNode.isNull()) {
        if (taskTimeNode.nodeName() == nsTaskTime) {
            const QDomElement elem = taskTimeNode.toElement();
            const qint64 startTime = elem.attribute(attrStartTime, "0").toLongLong();
            const qint64 endTime = elem.attribute(attrEndTime, "0").toLongLong();
            if (startTime > 0 && endTime > 0) {
                taskTimes.push_back(TaskTime(startTime, endTime));
            }
        }

        taskTimeNode = taskTimeNode.nextSibling();
    }

    return taskTimes;
}

static TaskData taskDataFromElem(const QDomElement &taskElem)
{
    TaskData taskData;
    taskData.setTitle(taskElem.attribute(attrTitle, ""));
    taskData.setDesc(taskElem.attribute(attrDesc, ""));
    taskData.setTimeLimit(taskElem.attribute(attrTimeLimit, 0).toLongLong());
    taskData.setCompleted(taskElem.attribute(attrIsComplited, 0).toInt());

    return taskData;
}

static void parceTaskNode(QDomNode node, TaskItem *task)
{
    TaskItem *child = task->append(taskDataFromElem(node.toElement()),
                                   taskTimesFromElem(node.toElement()));

    node = node.firstChild();
    while (!node.isNull()) {
        if (node.nodeName() == nsTask) {
            parceTaskNode(node, child);
        }

        node = node.nextSibling();
    }
}

bool saveProjectToXml(const QString &fileName,
                      const Tomato *tomato,
                      const TaskManager *taskManager,
                      QString *reason)
{
    try {
        QFile output(fileName);
        if (!output.open(QIODevice::WriteOnly)) {
            throw output.errorString();
        }

        QDomDocument doc(nsDocType);
        QDomElement projectElem = doc.createElement(nsProject);
        projectElem.appendChild(createSettingsElem(doc, tomato));
        projectElem.appendChild(createRootTaskElem(doc, tomato, taskManager->rootItem()));
        doc.appendChild(projectElem);

        QByteArray xmlData = doc.toByteArray();
        if (output.write(xmlData) != xmlData.size()) {
            throw output.errorString();
        }

        return true;
    }
    catch (const QString &error) {
        if (reason) {
            *reason = error;
        }
    }

    return false;
}

bool loadProjectFromXml(const QString &fileName,
                        Tomato *tomato,
                        TaskManager *taskManager,
                        QString *reason)
{
    try {
        QFile input(fileName);
        if (!input.open(QIODevice::ReadOnly)) {
            throw input.errorString();
        }

        QDomDocument doc(nsDocType);
        if (!doc.setContent(&input, reason)) {
            return false;
        }

        if (doc.documentElement().nodeName() != nsProject) {
            throw QObject::tr("incorrect file format");
        }

        QDomNode projectNode = doc.documentElement().firstChild();
        while (!projectNode.isNull()) {
            if (projectNode.nodeName() == nsSettings) {
                const QDomElement settingsElem = projectNode.toElement();
                tomato->setWorkingTime(settingsElem.attribute(attrWorkingTime, QString::number(DefaultWorkingTime)).toInt());
                tomato->setRestingTime(settingsElem.attribute(attrRestingTime, QString::number(DefaultRestingTime)).toInt());
            }
            else if (projectNode.nodeName() == nsRootTask) {
                QDomNode taskRootNode = projectNode.firstChild();
                while (!taskRootNode.isNull()) {
                    if (taskRootNode.nodeName() == nsTask) {
                        parceTaskNode(taskRootNode, taskManager->rootItem());
                    }

                    taskRootNode = taskRootNode.nextSibling();
                }
            }

            projectNode = projectNode.nextSibling();
        }

        return true;
    }
    catch (const QString &error) {
        if (reason) {
            *reason = error;
        }
    }

    return false;
}
