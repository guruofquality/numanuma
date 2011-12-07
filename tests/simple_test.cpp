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
#include <iostream>

void test_numa(void){
    const size_t num_nodes = numanuma::get_num_nodes();
    std::cout << "num nodes " << num_nodes << std::endl;
    for (size_t n = 0; n < num_nodes; n++){
        std::cout << "node " << n << std::endl;
        std::cout << "  mem size " << numanuma::get_mem_size(n) << " bytes" << std::endl;
        std::cout << "  allocate some mem ";
        numanuma::mem *m = numanuma::mem::make(n, 1);
        std::cout << size_t(m) << std::endl;
        delete m;
        std::cout << "  setting thread affinity: ";
        try{
            numanuma::set_thread_affinity(n);
            std::cout << "pass" << std::endl;
        }
        catch(const std::exception &e){
            std::cout << "fail " << e.what() << std::endl;
        }
    }
}

int main(void){
    try{
        numanuma::get_num_nodes();
        test_numa();
    }
    catch(const std::exception &e){
        std::cout << "error " << e.what() << std::endl;
    }

    std::cout << "setting thread priority: ";
    try{
        numanuma::set_thread_priority(1.0);
        std::cout << "pass" << std::endl;
    }
    catch(const std::exception &e){
        std::cout << "fail " << e.what() << std::endl;
    }

    std::cout << "time now " << numanuma::get_time_now() << std::endl;
    std::cout << "time tps " << numanuma::get_time_tps() << std::endl;

    return 0;
}
