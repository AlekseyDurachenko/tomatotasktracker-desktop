unix {
    contains(DEFINES, APP_PORTABLE) {
        INSTALLS += target docs sounds langs
        target.path = /

        docs.path = /
        docs.files = $$PWD/ABOUT $$PWD/AUTHORS $$PWD/CHANGELOG $$PWD/LIBRARIES $$PWD/LICENSE $$PWD/LICENSE.GPL-3+

        sounds.path = /sound
        sounds.files = $$PWD/res/sound/*.wav

        langs.path = /lang
        langs.files = $$PWD/ts/*.qm
    }

    !contains(DEFINES, APP_PORTABLE) {
        INSTALLS += target docs sounds langs desktopfiles appicons
        target.path = /usr/bin/

        docs.path = /usr/share/$${TARGET}
        docs.files = $$PWD/ABOUT $$PWD/AUTHORS $$PWD/CHANGELOG $$PWD/LIBRARIES $$PWD/LICENSE $$PWD/LICENSE.GPL-3+

        sounds.path = /usr/share/$${TARGET}/sound
        sounds.files = $$PWD/res/sound/*.wav

        langs.path = /usr/share/$${TARGET}/lang
        langs.files = $$PWD/ts/*.qm

        desktopfiles.path = /usr/share/applications/
        desktopfiles.files = $$PWD/res/desktopfiles/tomatotasktracker-desktop.desktop

        appicons.path = /usr/share/icons/
        appicons.files = $$PWD/res/icons/hicolor
    }
}
