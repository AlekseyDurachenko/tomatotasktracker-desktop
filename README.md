# TomatoTaskTracker-Desktop

The personal task tracker with the Pomodoro Technique.

Homepage: http://alekseydurachenko.github.io/tomatotasktracker-desktop/

## Build && Install
```bash
lrelease tomatotasktracker-desktop.pro
qmake
make -f Makefile.Release
make install
```

## Build linux portable version
```bash
lrelease tomatotasktracker-desktop.pro
qmake "DEFINES += APP_PORTABLE"
make -f Makefile.Release
make INSTALL_ROOT=`pwd`/portable-build install
```
