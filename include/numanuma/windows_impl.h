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

#ifndef INCLUDED_NUMANUMA_WINDOWS_IMPL_H
#define INCLUDED_NUMANUMA_WINDOWS_IMPL_H

#include <windows.h>
#include <stdlib.h> //malloc/free

__inline int numanuma__get_num_nodes(void){
    ULONG num;
    if (GetNumaHighestNodeNumber(&num)) return num + 1;
    return -1;
}

struct numanuma__mem_t{
    void *mem;
    size_t size;
};

__inline void *numanuma__mem_alloc(const int node, const size_t size, numanuma__mem_handle *hp){
    #if _WIN32_WINNT >= 0x0600
    void *mem = VirtualAllocExNuma(
        GetCurrentProcess(),
        NULL,
        size,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE,
        node
    );
    if (mem == NULL) return NULL;

    *hp = (struct numanuma__mem_t *)malloc(sizeof(struct numanuma__mem_t));
    if (*hp == NULL) return NULL;
    (*hp)->mem = mem;
    (*hp)->size = size;
    return (*hp)->mem;
    #else
    return NULL;
    #endif
}

__inline void numanuma__mem_free(numanuma__mem_handle *hp){
   VirtualFree((*hp)->mem, (*hp)->size, MEM_RELEASE);
   free(*hp);
}

__inline size_t numanuma__get_page_size(void){
    SYSTEM_INFO SystemInfo;
    GetSystemInfo (&SystemInfo);
    return SystemInfo.dwPageSize;
}

__inline long long numanuma__get_mem_size(const int node){
    ULONGLONG avail;
    #if _WIN32_WINNT >= 0x0601
    if (GetNumaAvailableMemoryNodeEx(node, &avail)) return avail;
    #else
    if (GetNumaAvailableMemoryNode(node, &avail)) return avail;
    #endif
    return -1;
}

__inline int numanuma__set_thread_affinity(const int node){
#if _WIN32_WINNT >= 0x0601
    GROUP_AFFINITY groupAffinity;
    if (GetNumaNodeProcessorMaskEx(static_cast<USHORT>(node), &groupAffinity))
    {
        if (SetThreadAffinityMask(GetCurrentThread(), static_cast<DWORD_PTR>(groupAffinity.Mask)))
        {
            return 0;
        }
    }
#else
    ULONGLONG processorMask(0);
    if (GetNumaNodeProcessorMask(static_cast<UCHAR>(node), &processorMask))
    {
        if (SetThreadAffinityMask(GetCurrentThread(), static_cast<DWORD_PTR>(processorMask)))
        {
            return 0;
        }
    }
#endif
    return -1;
}

__inline int numanuma__set_thread_priority(const double prio){
    int thread_prio = THREAD_PRIORITY_NORMAL;

    if (prio > 0){
        if      (prio > +0.75) thread_prio = THREAD_PRIORITY_TIME_CRITICAL;
        else if (prio > +0.50) thread_prio = THREAD_PRIORITY_HIGHEST;
        else if (prio > +0.25) thread_prio = THREAD_PRIORITY_ABOVE_NORMAL;
        else                   thread_prio = THREAD_PRIORITY_NORMAL;
    }

    else{
        if      (prio < -0.75) thread_prio = THREAD_PRIORITY_IDLE;
        else if (prio < -0.50) thread_prio = THREAD_PRIORITY_LOWEST;
        else if (prio < -0.25) thread_prio = THREAD_PRIORITY_BELOW_NORMAL;
        else                   thread_prio = THREAD_PRIORITY_NORMAL;
    }

    if (SetThreadPriority(GetCurrentThread(), thread_prio)) return 0;
    return -1;
}

__inline long long numanuma__get_time_now(void){
    LARGE_INTEGER counts;
    QueryPerformanceCounter(&counts);
    return counts.QuadPart;
}

__inline long long numanuma__get_time_tps(void){
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return freq.QuadPart;
}

#endif /*INCLUDED_NUMANUMA_WINDOWS_IMPL_H*/
