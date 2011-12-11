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
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(NumaNuma)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall)
endif()

########################################################################
# Build some tests
########################################################################
add_executable(simple_test_c simple_test.c)
target_link_libraries(simple_test_c ${NUMANUMA_LIBRARIES})

add_executable(simple_test_cpp simple_test.cpp)
target_link_libraries(simple_test_cpp ${NUMANUMA_LIBRARIES})
