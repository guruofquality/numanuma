#
# Copyright 2011 Josh Blum
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

########################################################################
# Check for support
########################################################################
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    add_definitions(-Wall)
    list(APPEND numanuma_libs numa pthread rt)

endif()

if(APPLE)

    add_definitions(-Wall)
    list(APPEND numanuma_libs pthread)

endif()

########################################################################
# Build some tests
########################################################################
add_executable(simple_test_c simple_test.c)
target_link_libraries(simple_test_c ${numanuma_libs})

add_executable(simple_test_cpp simple_test.cpp)
target_link_libraries(simple_test_cpp ${numanuma_libs})
