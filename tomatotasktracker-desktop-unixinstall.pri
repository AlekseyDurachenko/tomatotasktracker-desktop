unix {
    # MAKE VARIABLES
    isEmpty(PREFIX) : PREFIX = /usr
    BINDIR    = $$PREFIX/bin
    DATADIR   = $$PREFIX/share

    DEFINES  += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

    # MAKE INSTALL
    INSTALLS += target docs
    target.path = $$BINDIR

    docs.path = $$DATADIR/doc/$$TARGET
    docs.files +=                       \

}
