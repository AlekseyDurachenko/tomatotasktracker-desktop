# release rules
build_pass:CONFIG(release, debug|release):win32 {
    LIBS +=
    CONFIG -= console
    PKGCONFIG +=
}


# debug rules
build_pass:CONFIG(debug, debug|release):win32 {
    LIBS +=
    CONFIG +=
    PKGCONFIG +=
}
