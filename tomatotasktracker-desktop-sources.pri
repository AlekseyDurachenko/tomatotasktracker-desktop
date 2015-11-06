INCLUDEPATH    +=                                                       \
    $$PWD/src                                                           \
    $$PWD/src/core                                                      \
    $$PWD/src/gui                                                       \
    $$PWD/src/theme                                                     \
    $$PWD/src/tray                                                      \
    $$PWD/src/util                                                      \


HEADERS        +=                                                       \
    $$PWD/src/consts.h                                                  \
    $$PWD/src/settings.h                                                \
    $$PWD/src/version.h                                                 \
    $$PWD/src/core/project.h                                            \
    $$PWD/src/core/projectxml.h                                         \
    $$PWD/src/core/task.h                                               \
    $$PWD/src/core/taskdata.h                                           \
    $$PWD/src/core/tasktime.h                                           \
    $$PWD/src/core/tomato.h                                             \
    $$PWD/src/core/taskfilterproxymodel.h                               \
    $$PWD/src/core/taskitemmodel.h                                      \
    $$PWD/src/gui/aboutdialog.h                                         \
    $$PWD/src/gui/mainwindow.h                                          \
    $$PWD/src/gui/getopenfilenamewidget.h                               \
    $$PWD/src/gui/propertiesdialog.h                                    \
    $$PWD/src/gui/settingsdialog.h                                      \
    $$PWD/src/gui/taskeditdialog.h                                      \
    $$PWD/src/gui/tasktimeeditdialog.h                                  \
    $$PWD/src/gui/taskview.h                                            \
    $$PWD/src/gui/tomatowidget.h                                        \
    $$PWD/src/theme/theme.h                                             \
    $$PWD/src/tray/systemtrayicon.h                                     \
    $$PWD/src/util/checkptr.h                                           \
    $$PWD/src/util/resources.h                                          \
    $$PWD/src/util/timestr.h                                            \
    $$PWD/src/util/playsound.h                                          \


SOURCES        +=                                                       \
    $$PWD/src/version.cpp                                               \
    $$PWD/src/core/project.cpp                                          \
    $$PWD/src/core/projectxml.cpp                                       \
    $$PWD/src/core/task.cpp                                             \
    $$PWD/src/core/taskdata.cpp                                         \
    $$PWD/src/core/tasktime.cpp                                         \
    $$PWD/src/core/tomato.cpp                                           \
    $$PWD/src/core/taskfilterproxymodel.cpp                             \
    $$PWD/src/core/taskitemmodel.cpp                                    \
    $$PWD/src/gui/aboutdialog.cpp                                       \
    $$PWD/src/gui/mainwindow.cpp                                        \
    $$PWD/src/gui/getopenfilenamewidget.cpp                             \
    $$PWD/src/gui/propertiesdialog.cpp                                  \
    $$PWD/src/gui/settingsdialog.cpp                                    \
    $$PWD/src/gui/taskeditdialog.cpp                                    \
    $$PWD/src/gui/tasktimeeditdialog.cpp                                \
    $$PWD/src/gui/taskview.cpp                                          \
    $$PWD/src/gui/tomatowidget.cpp                                      \
    $$PWD/src/theme/theme.cpp                                           \
    $$PWD/src/util/checkptr.cpp                                         \
    $$PWD/src/util/resources.cpp                                        \
    $$PWD/src/util/timestr.cpp                                          \
    $$PWD/src/util/playsound.cpp                                        \
    $$PWD/src/tray/systemtrayicon.cpp                                   \


FORMS          +=                                                       \
    $$PWD/src/gui/aboutdialog.ui                                        \
    $$PWD/src/gui/propertiesdialog.ui                                   \
    $$PWD/src/gui/taskeditdialog.ui                                     \
    $$PWD/src/gui/tasktimeeditdialog.ui                                 \


TRANSLATIONS   +=                                                       \
    $$PWD/ts/tomatotasktracker-desktop_ru.ts                            \


RESOURCES      +=                                                       \
    $$PWD/qrc/icons.qrc                                                 \


OTHER_FILES    +=                                                       \
    $$PWD/ABOUT                                                         \
    $$PWD/AUTHORS                                                       \
    $$PWD/CHANGELOG                                                     \
    $$PWD/LIBRARIES                                                     \
    $$PWD/LICENSE                                                       \
    $$PWD/LICENSE.GPL-3+                                                \
    $$PWD/README.md                                                     \


!contains(QT, testlib) {
    HEADERS   +=                                                        \

    SOURCES   +=                                                        \
        $$PWD/src/main.cpp                                              \

}
