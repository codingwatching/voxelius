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
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

struct GLFWwindow;
namespace reactphysics3d { class PhysicsWorld; }

namespace client_globals
{
// Windowing and events
extern GLFWwindow *window;
extern entt::dispatcher dispatcher;

// Entities and physics
extern entt::registry registry;
extern reactphysics3d::PhysicsWorld *world;

// Miscellaneous
extern double curtime;
extern double frametime;
extern double interpfactor;
} // namespace client_globals
