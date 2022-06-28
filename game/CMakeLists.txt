# SPDX-License-Identifier: MPL-2.0
# 
# Copyright (c), 2022, Voxelius Team.
# Created: Tue Jun 28 2022 23:00:55.
# Author: Kirill GPRB.
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(shared)

## Development builds
option(DEV_BUILD "Build a development-friendly version" ON)

## Client-side application
add_executable(vcl "${CMAKE_CURRENT_LIST_DIR}/main.cpp")
target_compile_definitions(vcl PRIVATE V_CLIENT)
if(DEV_BUILD)
    target_compile_definitions(vcl PRIVATE V_DEVBUILD)
endif()
target_include_directories(vcl PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(vcl PRIVATE vclient)

## Server-side application
#add_executable(vds "${CMAKE_CURRENT_LIST_DIR}/main.cpp")
#target_compile_definitions(vds PRIVATE V_DEDICATED)
#if(DEV_BUILD)
#    target_compile_definitions(vds PRIVATE V_DEVBUILD)
#endif()
#target_include_directories(vds PUBLIC "${CMAKE_SOURCE_DIR}")
#target_link_libraries(vds PRIVATE vserver)
