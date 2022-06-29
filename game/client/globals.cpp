/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 15:37:45.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <game/client/globals.hpp>

// Windowing and events
GLFWwindow *client_globals::window = nullptr;
entt::dispatcher client_globals::dispatcher = {};

// Counters
float client_globals::epoch = 0.0f;
float client_globals::frametime = 0.0f;
float client_globals::frametime_avg = 0.0f;
size_t client_globals::frame_count = 0;
