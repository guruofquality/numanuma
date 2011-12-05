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

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define NUMANUMA_HELPER_DLL_IMPORT __declspec(dllimport)
  #define NUMANUMA_HELPER_DLL_EXPORT __declspec(dllexport)
#else
  #if __GNUC__ >= 4
    #define NUMANUMA_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define NUMANUMA_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define NUMANUMA_HELPER_DLL_IMPORT
    #define NUMANUMA_HELPER_DLL_EXPORT
  #endif
#endif

#ifdef NUMANUMA_DLL_EXPORTS // defined if we are building the NUMANUMA DLL (instead of using it)
  #define NUMANUMA_API NUMANUMA_HELPER_DLL_EXPORT
#else
  #define NUMANUMA_API NUMANUMA_HELPER_DLL_IMPORT
#endif // NUMANUMA_DLL_EXPORTS

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Query the number of nodes on the current system.
 * \return number of nodes or -1 for zero
 */
extern NUMANUMA_API int numanuma__get_num_nodes(void);

//! Opaque handle for the memory alloc/free
typedef struct numanuma__mem_t *numanuma__mem_handle;

/*!
 * Allocate memory for a specific node.
 * \param node the index of a node
 * \param size the size of the memory in bytes
 * \param hp a handle pointer to use with free
 * \return a pointer to the memory or NULL
 */
extern NUMANUMA_API void *numanuma__mem_alloc(const int node, const size_t size, numanuma__mem_handle *hp);

/*!
 * Free memory allocated by alloc.
 * \param hp a handle pointer created by alloc
 */
extern NUMANUMA_API void numanuma__mem_free(numanuma__mem_handle *hp);

/*!
 * Get the memory size of a node.
 * \param node the index of a node
 * \return memory size in bytes, -1 for failure
 */
extern NUMANUMA_API long long numanuma__get_mem_size(const int node);

/*!
 * Set the affinity for the calling thread.
 * \param node the index of a node
 * \return -1 for error, 0 for success
 */
extern NUMANUMA_API int numanuma__set_affinity(const int node);

//thread scheduling

//cpus per node

//get my node

//move thread to node

//mem available

//mem free

//page size

#ifdef __cplusplus
}
#endif

#endif /*INCLUDED_NUMANUMA_H*/
