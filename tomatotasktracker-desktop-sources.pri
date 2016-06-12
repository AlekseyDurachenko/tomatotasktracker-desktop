INCLUDEPATH    +=                                                   \
    $$PWD/src                                                       \


HEADERS        +=                                                   \
    $$PWD/src/aboutdialog.h                                         \
    $$PWD/src/consts.h                                              \
    $$PWD/src/getopenfilenamewidget.h                               \
    $$PWD/src/mainwindow.h                                          \
    $$PWD/src/playsound.h                                           \
    $$PWD/src/project.h                                             \
    $$PWD/src/projectxml.h                                          \
    $$PWD/src/propertiesdialog.h                                    \
    $$PWD/src/resources.h                                           \
    $$PWD/src/settingsdialog.h                                      \
    $$PWD/src/settings.h                                            \
    $$PWD/src/taskdata.h                                            \
    $$PWD/src/taskeditdialog.h                                      \
    $$PWD/src/taskitemfilterproxymodel.h                            \
    $$PWD/src/taskitem.h                                            \
    $$PWD/src/taskitemmodel.h                                       \
    $$PWD/src/taskmanager.h                                         \
    $$PWD/src/tasktimeeditdialog.h                                  \
    $$PWD/src/tasktime.h                                            \
    $$PWD/src/theme.h                                               \
    $$PWD/src/timestr.h                                             \
    $$PWD/src/tomato.h                                              \
    $$PWD/src/version.h                                             \


SOURCES        +=                                                   \
    $$PWD/src/aboutdialog.cpp                                       \
    $$PWD/src/getopenfilenamewidget.cpp                             \
    $$PWD/src/mainwindow.cpp                                        \
    $$PWD/src/playsound.cpp                                         \
    $$PWD/src/project.cpp                                           \
    $$PWD/src/projectxml.cpp                                        \
    $$PWD/src/propertiesdialog.cpp                                  \
    $$PWD/src/resources.cpp                                         \
    $$PWD/src/settings.cpp                                          \
    $$PWD/src/settingsdialog.cpp                                    \
    $$PWD/src/taskdata.cpp                                          \
    $$PWD/src/taskeditdialog.cpp                                    \
    $$PWD/src/taskitem.cpp                                          \
    $$PWD/src/taskitemfilterproxymodel.cpp                          \
    $$PWD/src/taskitemmodel.cpp                                     \
    $$PWD/src/taskmanager.cpp                                       \
    $$PWD/src/tasktime.cpp                                          \
    $$PWD/src/tasktimeeditdialog.cpp                                \
    $$PWD/src/theme.cpp                                             \
    $$PWD/src/timestr.cpp                                           \
    $$PWD/src/tomato.cpp                                            \
    $$PWD/src/version.cpp                                           \


FORMS          +=                                                   \
    $$PWD/src/aboutdialog.ui                                        \
    $$PWD/src/propertiesdialog.ui                                   \
    $$PWD/src/taskeditdialog.ui                                     \
    $$PWD/src/tasktimeeditdialog.ui                                 \


TRANSLATIONS   +=                                                   \
    $$PWD/translations/ru.ts                                        \


RESOURCES      +=                                                   \


OTHER_FILES    +=                                                   \
    $$PWD/ABOUT                                                     \
    $$PWD/AUTHORS                                                   \
    $$PWD/CHANGELOG                                                 \
    $$PWD/LIBRARIES                                                 \
    $$PWD/LICENSE                                                   \
    $$PWD/LICENSE.GPL-3+                                            \
    $$PWD/README.md                                                 \
    $$PWD/translations/docs/ABOUT_ru                                \
    $$PWD/translations/docs/AUTHORS_ru                              \
    $$PWD/translations/docs/CHANGELOG_ru                            \
    $$PWD/translations/docs/LIBRARIES_ru                            \


!contains(QT, testlib) {
    HEADERS   +=                                                    \

    SOURCES   +=                                                    \
        $$PWD/src/main.cpp                                          \

}

win32 {
    RC_FILE     += res/windows/rc.rc
}
