## SPDX-License-Identifier: MPL-2.0
## 
## Copyright (c), 2022, Voxelius Team.
## Created: Tue Jun 28 2022 23:00:55.
## Author: Kirill GPRB.
## 
## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.

## Options
option(BUILD_DEV "Build development-friendly executables" ON)
option(BUILD_INTSRV "Link client with an integrated server library" OFF)

## More internal projects.
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(shared)

## Client application.
add_executable(vcl "${CMAKE_CURRENT_LIST_DIR}/main.cpp")
target_compile_definitions(vcl PRIVATE BUILD_VCL)
target_include_directories(vcl PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(vcl PRIVATE client)

## Dedicated server application.
### add_executable(vds "${CMAKE_CURRENT_LIST_DIR}/main.cpp")
### target_compile_definitions(vds PRIVATE BUILD_VDS)
### target_include_directories(vds PUBLIC "${CMAKE_SOURCE_DIR}")
### target_link_libraries(vds PRIVATE server)

## Enable development builds.
if(BUILD_DEV)
    target_compile_definitions(vcl PRIVATE BUILD_DEV)
    ### target_compile_definitions(vds PRIVATE BUILD_DEV)
endif()

## Link integrated server.
if(BUILD_INTSRV)
    ### target_compile_definitions(vcl PRIVATE BUILD_INTSRV)
    ### target_link_libraries(vcl PRIVATE server)
endif()
