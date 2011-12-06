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
include(CheckIncludeFile)
CHECK_INCLUDE_FILE(numa.h HAVE_NUMA_H)
CHECK_INCLUDE_FILE(pthread.h HAVE_PTHREAD_H)
CHECK_INCLUDE_FILE(windows.h HAVE_WINDOWS_H)

if(HAVE_NUMA_H AND HAVE_PTHREAD_H)

    list(APPEND numanuma_sources libnuma_impl.c)
    list(APPEND numanuma_libs numa)

elseif(HAVE_WINDOWS_H)

    list(APPEND numanuma_sources windows_impl.c)

endif()

list(APPEND numanuma_sources cpp_wrapper.cpp)

########################################################################
# Build the library
########################################################################
add_library(numanuma SHARED ${numanuma_sources})
target_link_libraries(numanuma ${numanuma_libs})
set_target_properties(numanuma PROPERTIES DEFINE_SYMBOL "NUMANUMA_DLL_EXPORTS")

install(TARGETS numanuma
    LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT runtime # .so file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel   # .lib file
    RUNTIME DESTINATION bin              COMPONENT runtime # .dll file
)
