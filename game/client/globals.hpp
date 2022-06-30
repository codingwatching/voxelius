/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 15:36:51.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <cstddef>
#include <entt/signal/dispatcher.hpp>

// Defined in <GLFW/glfw3.h.
struct GLFWwindow;

namespace client_globals
{
// Windowing and events
extern GLFWwindow *window;
extern entt::dispatcher dispatcher;

// Counters
extern double epoch;
extern double frametime;
extern double frametime_avg;
extern std::size_t frame_count;
} // namespace client_globals

// Alias namespaces.
// This generally protects us from
// linkage errors (both client and
// server have a globals namespace).
namespace globals = client_globals;
