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

#ifndef INCLUDED_NUMANUMA_HPP
#define INCLUDED_NUMANUMA_HPP

#include <numanuma.h>

namespace numanuma{

    //! Wrapper to get the number of NUMA nodes
    NUMANUMA_API size_t get_num_nodes(void);

    //! Wrapper to allocate and manage NUMA memory
    struct NUMANUMA_API mem{
        static mem *make(const int node, const size_t size);
        virtual void *get(void) = 0;
        virtual size_t len(void) = 0;
    };

    //! Wrapper to get the available node memory
    NUMANUMA_API unsigned long long get_mem_size(const int node);

    //! Wrapper to set the caller thread's affinity
    NUMANUMA_API void set_affinity(const int node);

} //namespace numanuma

#endif /*INCLUDED_NUMANUMA_HPP*/
