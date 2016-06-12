win32 {
    docs.files =                                                    \
        $$PWD/ABOUT                                                 \
        $$PWD/AUTHORS                                               \
        $$PWD/CHANGELOG                                             \
        $$PWD/LIBRARIES                                             \
        $$PWD/LICENSE                                               \
        $$PWD/LICENSE.GPL-3+                                        \
        $$PWD/translations/docs/*                                   \

    sounds.files =                                                  \
        $$PWD/res/sounds/*.wav                                      \

    translations.files =                                            \
        $$PWD/translations/*.qm                                     \

    icons.files =                                                   \
        $$PWD/res/icons/hicolor/128x128/*                           \


    INSTALLS += target docs sounds translations icons

    target.path = /
    docs.path   = /
    sounds.path = /sounds
    icons.path  = /icons
    translations.path = /translations
}
