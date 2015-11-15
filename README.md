TomatoTaskTracker-Desktop
=========================

Tomato Task Tracker is your personal simple task tracker
with added Pomodoro technique


Build && Install
----------------

```bash
lrelease tomatotasktracker-desktop.pro
qmake
make -f Makefile.Release
make install
```

Build linux portable version
----------------------------

```bash
lrelease tomatotasktracker-desktop.pro
qmake "DEFINES += APP_PORTABLE"
make -f Makefile.Release
make INSTALL_ROOT=`pwd`/portable-build install
```
