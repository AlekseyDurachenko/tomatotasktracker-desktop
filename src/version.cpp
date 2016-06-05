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


#ifndef APP_NAME
    #define APP_NAME "target"
#endif

#ifndef APP_MAJOR
    #define APP_MAJOR 0
#endif

#ifndef APP_MINOR
    #define APP_MINOR 0
#endif

#ifndef APP_PATCH
    #define APP_PATCH 0
#endif

#ifndef APP_VERSION
    #define APP_VERSION STR(APP_MAJOR) "." STR(APP_MINOR) "." STR(APP_PATCH)
#endif


const char *appOrgName()
{
#ifdef APP_ORG_NAME
    return STR(APP_ORG_NAME);
#else
    return "";
#endif
}

const char *appOrgDomain()
{
#ifdef APP_ORG_DOMAIN
    return STR(APP_ORG_DOMAIN);
#else
    return "";
#endif
}

const char *appName()
{
    return STR(APP_NAME);
}

const char *appShortName()
{
    return STR(APP_NAME);
}

const char *appFullName()
{
    return STR(APP_NAME);
}

const char *appVersion()
{
    return STR(APP_VERSION);
}

const char *appBuildNumber()
{
#ifdef APP_BUILD_NUMBER
    return STR(APP_BUILD_NUMBER);
#else
    return "0";
#endif
}

const char *appBuildDate()
{
#ifdef APP_BUILD_DATE
    return STR(APP_BUILD_DATE);
#else
    return "0000-00-00T00:00:00+0000";
#endif
}

const char *appRevision()
{
#ifdef APP_REVISION
    return STR(APP_REVISION);
#else
    return "0";
#endif
}

const char *appSources()
{
#ifdef APP_SOURCES
    return STR(APP_SOURCES);
#else
    return "";
#endif
}
