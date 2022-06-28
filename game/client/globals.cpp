/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 00:54:47.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <game/client/globals.hpp>

// Windowing and events
entt::dispatcher globals::events = {};
GLFWwindow *globals::window = nullptr;

// Statistics
float globals::curtime = 0.0f;
float globals::frametime = 0.0f;
float globals::frametime_avg = 0.0f;
size_t globals::frame_count = 0;
