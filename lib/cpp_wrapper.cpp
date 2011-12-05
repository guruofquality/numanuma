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

#include <numanuma.hpp>
#include <stdexcept>

size_t numanuma::get_num_nodes(void){
    const int ret = numanuma__get_num_nodes();
    if (ret == -1) throw std::runtime_error("numanuma::get_num_nodes fail");
    return ret;
}

struct mem_impl : public numanuma::mem{
    mem_impl(const int node, const size_t size){
        _ptr = numanuma__mem_alloc(node, size, &handle);
        _len = size;
    }

    ~mem_impl(void){
        if (_ptr != NULL) numanuma__mem_free(&handle);
    }

    void *get(void){
        return _ptr;
    }

    size_t len(void){
        return _len;
    }

    void *_ptr;
    size_t _len;
    numanuma__mem_handle handle;
};

numanuma::mem *numanuma::mem::make(const int node, const size_t size){
    return new mem_impl(node, size);
}

unsigned long long numanuma::get_mem_size(const int node){
    long long ret = numanuma__get_mem_size(node);
    if (ret == -1) throw std::runtime_error("numanuma::get_mem_size fail");
    return ret;
}

void numanuma::set_affinity(const int node){
    const int ret = numanuma__set_affinity(node);
    if (ret == -1) throw std::runtime_error("numanuma::set_affinity fail");
}
