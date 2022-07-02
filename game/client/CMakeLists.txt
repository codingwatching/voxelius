## SPDX-License-Identifier: MPL-2.0
## 
## Copyright (c), 2022, Voxelius Team.
## Created: Wed Jun 29 2022 15:29:18.
## Author: Kirill GPRB.
## 
## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_library(client STATIC "")
target_compile_definitions(client PUBLIC GLFW_INCLUDE_NONE)
target_include_directories(client PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(client PUBLIC common shared glad glfw stb)

set(SOURCES)
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/game.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/globals.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/image.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/main.cpp")
target_sources(client PRIVATE ${SOURCES})

set(SOURCES)
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/atlas.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/input.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/model_set.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/phys_interp.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/player_move.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/screen.cpp")
target_sources(client PRIVATE ${SOURCES})
