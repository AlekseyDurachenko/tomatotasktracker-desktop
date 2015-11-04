TomatoTaskTracker-Desktop
=========================

The simple task tracker with the pomodoro technique.


Build linux portable version
----------------------------

```bash
lrelease tomatotasktracker-desktop.pro
qmake "DEFINES += APP_PORTABLE"
make
make INSTALL_ROOT=`pwd`/portable-build install
```


Build && install
----------------

```bash
lrelease tomatotasktracker-desktop.pro
qmake
make
make install
```
