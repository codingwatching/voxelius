## SPDX-License-Identifier: MPL-2.0
## 
## Copyright (c), 2022, Voxelius Team.
## Created: Wed Jun 29 2022 14:47:46.
## Author: Kirill GPRB.
## 
## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.10)
project(voxelius LANGUAGES CXX VERSION 0.0.1)

cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

## External dependencies.
add_subdirectory(external/entt)
add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/physfs)
add_subdirectory(external/spdlog)
add_subdirectory(external/thread_pool)
add_subdirectory(external/toml)

## Internal projects.
add_subdirectory(common)
add_subdirectory(game)
