# release rules
build_pass:CONFIG(release, debug|release):unix {
    LIBS +=
    CONFIG +=
    PKGCONFIG +=
}


# debug rules
build_pass:CONFIG(debug, debug|release):unix {
    LIBS +=
    CONFIG +=
    PKGCONFIG +=
}
