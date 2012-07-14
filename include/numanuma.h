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

#ifndef INCLUDED_NUMANUMA_H
#define INCLUDED_NUMANUMA_H

#include <stddef.h> //size_t

#ifdef __cplusplus
extern "C" {
#endif

//! No thread affinity for a node, used to reset
#define NUMANUMA_NODE_NONE ((int)-1)

/*!
 * Query the number of nodes on the current system.
 * \return number of nodes or -1 for error
 */
static int numanuma__get_num_nodes(void);

//! Opaque handle for the memory alloc/free
typedef struct numanuma__mem_t *numanuma__mem_handle;

/*!
 * Allocate memory for a specific node.
 * \param node the index of a node
 * \param size the size of the memory in bytes
 * \param hp a handle pointer to use with free
 * \return a pointer to the memory or NULL
 */
static void *numanuma__mem_alloc(const int node, const size_t size, numanuma__mem_handle *hp);

/*!
 * Free memory allocated by alloc.
 * \param hp a handle pointer created by alloc
 */
static void numanuma__mem_free(numanuma__mem_handle *hp);

/*!
 * Get the page size of system memory.
 * \return the number of bytes in a page
 */
static size_t numanuma__get_page_size(void);

/*!
 * Get the memory size of a node.
 * \param node the index of a node
 * \return memory size in bytes, -1 for failure
 */
static long long numanuma__get_mem_size(const int node);

/*!
 * Set the affinity for the calling thread.
 * To reset the affinity, pass NUMANUMA_NODE_NONE.
 * \param node the index of a node
 * \return -1 for error, 0 for success
 */
static int numanuma__set_thread_affinity(const int node);

/*!
 * Set the scheduling priority for the calling thread.
 * A priority of zero is considered to be default.
 * \param prio a priority between -1.0 and 1.0
 * \return -1 for error, 0 for success
 */
static int numanuma__set_thread_priority(const double prio);

//! Get the current time now in ticks
static long long numanuma__get_time_now(void);

//! Get the number of ticks per second
static long long numanuma__get_time_tps(void);

#ifdef __cplusplus
}
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
    #include <numanuma/linux_impl.h>
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #include <numanuma/windows_impl.h>
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    #include <numanuma/osx_impl.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #include <numanuma/bsd_impl.h>
#endif

#endif /*INCLUDED_NUMANUMA_H*/
