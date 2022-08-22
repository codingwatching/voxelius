## SPDX-License-Identifier: MPL-2.0
## 
## Copyright (c), 2022, Voxelius Contributors.
## Created: Wed Jun 29 2022 15:29:01.
## Author: Kirill GPRB.
## 
## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_library(shared STATIC "")
target_include_directories(shared PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(shared PUBLIC common EnTT  reactphysics3d)

set(SOURCES)
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/chunks.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/globals.cpp")
target_sources(shared PRIVATE ${SOURCES})
