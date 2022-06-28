# SPDX-License-Identifier: MPL-2.0
# 
# Copyright (c), 2022, Voxelius Team.
# Created: Tue Jun 28 2022 22:54:27.
# Author: Kirill GPRB.
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(SOURCES)
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/crc64.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/frustum.cpp")
target_sources(vcore PRIVATE ${SOURCES})
