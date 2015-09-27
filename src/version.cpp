// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "version.h"


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifndef APP_MAJOR
    #define APP_MAJOR 0
#endif

#ifndef APP_MINOR
    #define APP_MINOR 0
#endif

#ifdef APP_REV
    #ifdef APP_BUILD_NUMBER
        #define APP_VERSION STR(APP_MAJOR) "." STR(APP_MINOR) "." STR(APP_PATCH) "." STR(APP_REV) "." STR(APP_BUILD_NUMBER)
    #else
        #define APP_VERSION STR(APP_MAJOR) "." STR(APP_MINOR) "." STR(APP_PATCH) "." STR(APP_REV)
    #endif
#else
    #define APP_VERSION STR(APP_MAJOR) "." STR(APP_MINOR) "." STR(APP_PATCH)
#endif

const char *appVersion()
{
    return APP_VERSION;
}

const char *appBuildDate()
{
#ifdef APP_BUILD_DATE
    return STR(APP_BUILD_DATE);
#else
    return 0;
#endif
}

const char *appName()
{
#ifdef APP_NAME
    return STR(APP_NAME);
#else
    return "tomatotasktracker-desktop";
#endif
}

const char *appShortName()
{
    return appName(); // type here short name of the app
}

const char *appFullName()
{
    return appName(); // type here full name of the app
}
