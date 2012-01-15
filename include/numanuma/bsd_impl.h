//
// Copyright 2011 Josh Blum
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef INCLUDED_NUMANUMA_BSD_IMPL_H
#define INCLUDED_NUMANUMA_BSD_IMPL_H

#include <stddef.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <time.h>
#include <stdlib.h> //malloc/free
#include <unistd.h> //getpagesize

inline int numanuma__get_num_nodes(void){
    return 1;
}

struct numanuma__mem_t{
    void *mem;
};

inline void *numanuma__mem_alloc(const int node, const size_t size, numanuma__mem_handle *hp){
    void *mem;
    if (posix_memalign(&mem, getpagesize(), size) != 0) return NULL;
    if (mem == NULL) return NULL;

    *hp = (struct numanuma__mem_t *)malloc(sizeof(struct numanuma__mem_t));
    if (*hp == NULL) return NULL;
    (*hp)->mem = mem;
    return (*hp)->mem;
}

inline void numanuma__mem_free(numanuma__mem_handle *hp){
    free((*hp)->mem);
    free(*hp);
}

inline size_t numanuma__get_page_size(void){
    return getpagesize();
}

inline long long numanuma__get_mem_size(const int node){
    return -1;
}

inline int numanuma__set_thread_affinity(const int node){
    return -1;
}

inline int numanuma__set_thread_priority(const double prio){
    return -1;
}

inline long long numanuma__get_time_now(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec*numanuma__get_time_tps() + ts.tv_nsec;
}

inline long long numanuma__get_time_tps(void){
    return 1000000000UL;
}

#endif /*INCLUDED_NUMANUMA_BSD_IMPL_H*/
