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

#ifndef INCLUDED_NUMANUMA_LINUX_IMPL_H
#define INCLUDED_NUMANUMA_LINUX_IMPL_H

#include <numa.h>
#include <pthread.h>
#include <time.h>

inline int numanuma__get_num_nodes(void){
    if (numa_available() == -1) return -1;
    return numa_max_node() + 1;
}

struct numanuma__mem_t{
    int node;
    void *mem;
    size_t size;
};

inline void *numanuma__mem_alloc(const int node, const size_t size, numanuma__mem_handle *hp){
    if (numa_available() == -1) return NULL;
    void *mem = numa_alloc_onnode(size, node);

    if (mem == NULL) return NULL;
    *hp = (struct numanuma__mem_t *)malloc(sizeof(struct numanuma__mem_t));
    if (*hp == NULL) return NULL;
    (*hp)->node = node;
    (*hp)->mem = mem;
    (*hp)->size = size;
    return (*hp)->mem;
}

inline void numanuma__mem_free(numanuma__mem_handle *hp){
    numa_free((*hp)->mem, (*hp)->size);
    free(*hp);
}

inline long long numanuma__get_mem_size(const int node){
    if (numa_available() == -1) return -1;
    return numa_node_size64(node, NULL);
}

inline int numanuma__set_thread_affinity(const int node){
    return numa_run_on_node(node);
}

inline int numanuma__set_thread_priority(const double prio){
    const int policy = (prio <= 0.0)? SCHED_OTHER : SCHED_RR;
    const int min_pri = sched_get_priority_min(policy);
    const int max_pri = sched_get_priority_max(policy);
    if (min_pri == -1 || max_pri == -1) return -1;

    struct sched_param sp;
    sp.sched_priority = (int)(prio*(max_pri - min_pri)) + min_pri;
    return pthread_setschedparam(pthread_self(), policy, &sp);
}

inline long long numanuma__get_time_now(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec*numanuma__get_time_tps() + ts.tv_nsec;
}

inline long long numanuma__get_time_tps(void){
    return 1000000000UL;
}

#endif /*INCLUDED_NUMANUMA_LINUX_IMPL_H*/
