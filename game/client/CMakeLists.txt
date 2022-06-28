# SPDX-License-Identifier: MPL-2.0
# 
# Copyright (c), 2022, Voxelius Team.
# Created: Tue Jun 28 2022 23:06:42.
# Author: Kirill GPRB.
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_library(vclient STATIC "")
target_compile_definitions(vclient PUBLIC GLFW_INCLUDE_NONE)
target_include_directories(vclient PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(vclient PUBLIC vcore glad glfw)

## This is a hack: we don't have vshared yet.
target_link_libraries(vclient PUBLIC EnTT)

set(SOURCES)
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/globals.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/vclient.cpp")
target_sources(vclient PRIVATE ${SOURCES})
