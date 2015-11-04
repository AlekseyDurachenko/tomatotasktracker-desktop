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
        tmp = $$find(DEFINES, "APP_RESOURCE_PREFIX=")
        APP_RESOURCE_PREFIX = $$replace(tmp, "APP_RESOURCE_PREFIX=", "")

        INSTALLS += target docs sounds langs desktopfiles appicons
        target.path = /usr/bin/

        docs.path = $$APP_RESOURCE_PREFIX
        docs.files = $$PWD/ABOUT $$PWD/AUTHORS $$PWD/CHANGELOG $$PWD/LIBRARIES $$PWD/LICENSE $$PWD/LICENSE.GPL-3+

        sounds.path = $$APP_RESOURCE_PREFIX/sound
        sounds.files = $$PWD/res/sound/*.wav

        langs.path = $$APP_RESOURCE_PREFIX/lang
        langs.files = $$PWD/ts/*.qm

        desktopfiles.path = /usr/share/applications
        desktopfiles.files = $$PWD/res/desktopfiles/tomatotasktracker-desktop.desktop

        appicons.path = /usr/share/icons
        appicons.files = $$PWD/res/icons/hicolor
    }
}
