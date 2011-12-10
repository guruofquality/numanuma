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
#include <mach/mach.h> //thread policy
#include <mach/mach_time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdlib.h> //malloc/free
#include <unistd.h> //getpagesize

/*
 * http://developer.apple.com/library/mac/#releasenotes/Performance/RN-AffinityAPI/_index.html
 * BSD does not have a true node-aware NUMA like linux and windows.
 * A node consists of a group of CPUs that share an L2 cache.
 * Outside of cache, main memory is conceptualized as uniform.
 */

inline int numanuma__get_num_nodes(void){
    int64_t npackages;
    size_t len = sizeof(npackages);
    if (sysctlbyname("hw.packages", &npackages, &len, NULL, 0) == 0) return npackages;
    return -1;
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
    //divide by num nodes to emulate node-specific memory
    const int num_nodes = numanuma__get_num_nodes();
    if (num_nodes < 0) return -1;

    //query the size of the cachesize entries
    size_t size = 0;
    if (sysctlbyname("hw.cachesize", NULL, &size, NULL, 0) != 0) return -1;

    //query the cachesizes
    uint64_t cachesize[size/sizeof(uint64_t)];
    const int ret = sysctlbyname("hw.cachesize", cachesize, &size, NULL, 0);
    if (ret == 0 && size >= sizeof(uint64_t)) return cachesize[0]/num_nodes;
    return -1;
}

inline int numanuma__set_thread_affinity(const int node){
    #ifdef THREAD_AFFINITY_POLICY
    struct thread_affinity_policy ap;
    ap.affinity_tag = node;
    if (thread_policy_set(
        mach_thread_self(),
        THREAD_AFFINITY_POLICY,
        (thread_policy_t) &ap,
        THREAD_AFFINITY_POLICY_COUNT
    ) == 0) return 0;
    #endif
    return -1;
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
    return mach_absolute_time();
}

inline long long numanuma__get_time_tps(void){
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);
    return (info.numer*1000000000ULL)/info.denom;
}

#endif /*INCLUDED_NUMANUMA_BSD_IMPL_H*/
