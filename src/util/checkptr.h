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
#ifndef CHECKALLOC_H
#define CHECKALLOC_H


#define CHECK_PTR(ptr) {                                                    \
        if ((ptr) == 0) {                                                   \
            fprintf(stderr,                                                 \
                    "null pointer detected in: %s (%s:%d)\n" ,              \
                    __PRETTY_FUNCTION__,                                    \
                    strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1     \
                                           : __FILE__,                      \
                    __LINE__);                                              \
            exit(-1);                                                       \
        }                                                                   \
    }

#define CHECK_ALLOC(ptr) {                                                  \
        if ((ptr) == 0) {                                                   \
            fprintf(stderr,                                                 \
                    "couldn't allocate enough memory in: %s (%s:%d)\n" ,    \
                    __PRETTY_FUNCTION__,                                    \
                    strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1     \
                                           : __FILE__,                      \
                    __LINE__);                                              \
            exit(-1);                                                       \
        }                                                                   \
    }


#endif // CHECKALLOC_H
