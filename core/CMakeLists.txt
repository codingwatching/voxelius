# SPDX-License-Identifier: MPL-2.0
# 
# Copyright (c), 2022, Voxelius Team.
# Created: Tue Jun 28 2022 22:50:13.
# Author: Kirill GPRB.
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_library(vcore STATIC "")
target_include_directories(vcore PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(vcore PUBLIC glm physfs spdlog thread_pool toml)

set(SOURCES)
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/cmdline.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/vfs.cpp")
target_sources(vcore PRIVATE ${SOURCES})

add_subdirectory(math)
add_subdirectory(util)

if(WIN32)
    ## Definitions in <windows.h> conflict with
    ## the standard constexpr functions in <limits>
    target_compile_definitions(v_common PUBLIC NOMINMAX)
endif()

if(MSVC)
    ## MSVC really doesn't like how we just use
    ## functions marked by MS as "unsafe" safely.
    target_compile_definitions(v_common PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()
