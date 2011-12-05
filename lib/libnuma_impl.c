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

#include <numanuma.h>
#include <numa.h> //libnuma implementation

int numanuma__get_num_nodes(void){
    if (numa_available() == -1) return -1;
    return numa_max_node();
}

struct numanuma__mem_t{
    int node;
    void *mem;
    size_t size;
};

void *numanuma__mem_alloc(const int node, const size_t size, numanuma__mem_handle *hp){
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

void numanuma__mem_free(numanuma__mem_handle *hp){
    numa_free((*hp)->mem, (*hp)->size);
    free(*hp);
}

long long numanuma__get_mem_size(const int node){
    if (numa_available() == -1) return -1;
    return numa_node_size64(node, NULL);
}

int numanuma__set_affinity(const int node){
    return numa_run_on_node(node);
}
