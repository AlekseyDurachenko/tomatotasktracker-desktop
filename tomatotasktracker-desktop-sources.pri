INCLUDEPATH    +=                                                       \
    $$PWD/src                                                           \
    $$PWD/src/core                                                      \
    $$PWD/src/gui                                                       \
    $$PWD/src/theme                                                     \


HEADERS        +=                                                       \
    $$PWD/src/consts.h                                                  \
    $$PWD/src/version.h                                                 \
    $$PWD/src/settings.h                                                \
    $$PWD/src/core/task.h                                               \
    $$PWD/src/core/taskdata.h                                           \
    $$PWD/src/core/tasktime.h                                           \
    $$PWD/src/gui/mainwindow.h                                          \
    $$PWD/src/gui/taskview.h                                            \
    $$PWD/src/gui/taskitemmodel.h                                       \
    $$PWD/src/gui/taskeditdialog.h                                      \
    $$PWD/src/gui/tasktimeeditdialog.h                                  \
    $$PWD/src/theme/theme.h                                             \
    $$PWD/src/core/utils.h \
    $$PWD/src/gui/tomatowidget.h \
    $$PWD/src/gui/aboutdialog.h \
    $$PWD/src/core/project.h \
    $$PWD/src/core/projectxml.h \
    $$PWD/src/core/tomato.h \
    $$PWD/src/gui/propertiesdialog.h \
    $$PWD/src/gui/settingsdialog.h

SOURCES        +=                                                       \
    $$PWD/src/version.cpp                                               \
    $$PWD/src/core/task.cpp                                             \
    $$PWD/src/core/taskdata.cpp                                         \
    $$PWD/src/core/tasktime.cpp                                         \
    $$PWD/src/gui/mainwindow.cpp                                        \
    $$PWD/src/gui/taskview.cpp                                          \
    $$PWD/src/gui/taskitemmodel.cpp                                     \
    $$PWD/src/gui/taskeditdialog.cpp                                    \
    $$PWD/src/gui/tasktimeeditdialog.cpp                                \
    $$PWD/src/theme/theme.cpp                                           \
    $$PWD/src/core/utils.cpp \
    $$PWD/src/gui/tomatowidget.cpp \
    $$PWD/src/gui/aboutdialog.cpp \
    $$PWD/src/core/project.cpp \
    $$PWD/src/core/projectxml.cpp \
    $$PWD/src/core/tomato.cpp \
    $$PWD/src/gui/propertiesdialog.cpp \
    $$PWD/src/gui/settingsdialog.cpp


FORMS          +=                                                       \
    $$PWD/src/gui/mainwindow.ui                                         \
    $$PWD/src/gui/taskeditdialog.ui                                     \
    $$PWD/src/gui/tasktimeeditdialog.ui \
    $$PWD/src/gui/tomatowidget.ui \
    $$PWD/src/gui/aboutdialog.ui \
    $$PWD/src/gui/propertiesdialog.ui \
    $$PWD/src/gui/settingsdialog.ui


RESOURCES      +=                                                       \
    $$PWD/qrc/about.qrc                                                 \
    $$PWD/qrc/icons.qrc                                                 \
    $$PWD/qrc/sound.qrc


OTHER_FILES    +=                                                       \
    $$PWD/ABOUT                                                         \
    $$PWD/AUTHORS                                                       \
    $$PWD/CHANGELOG                                                     \
    $$PWD/LIBRARIES                                                     \
    $$PWD/LICENSE                                                       \
    $$PWD/LICENSE.GPL-3+                                                \
    $$PWD/README.md


!contains(QT, testlib) {
    HEADERS   +=                                                        \

    SOURCES   +=                                                        \
        $$PWD/src/main.cpp                                              \

}

DISTFILES += \
    $$PWD/DEVDOCS
