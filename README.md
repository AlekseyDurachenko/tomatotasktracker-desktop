TomatoTaskTracker-Desktop
=========================

Tomato Task Tracker is your simple task tracker with added Pomodoro technique


Build && Install
----------------

```bash
lrelease tomatotasktracker-desktop.pro
qmake
make
make install
```

Build linux portable version
----------------------------

```bash
lrelease tomatotasktracker-desktop.pro
qmake "DEFINES += APP_PORTABLE"
make
make INSTALL_ROOT=`pwd`/portable-build install
```
