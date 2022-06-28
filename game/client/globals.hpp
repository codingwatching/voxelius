/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 00:51:15.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <core/types.hpp>
#include <entt/signal/dispatcher.hpp>

struct GLFWwindow;

namespace globals
{
// Windowing and events
extern entt::dispatcher events;
extern GLFWwindow *window;

// Statistics
extern float curtime;
extern float frametime;
extern float frametime_avg;
extern size_t frame_count;

} // namespace globals
