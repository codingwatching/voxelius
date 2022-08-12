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
#include <entt/entity/entity.hpp>
#include <reactphysics3d/engine/PhysicsWorld.h>

class Atlas;
struct GLFWwindow;

namespace globals
{
extern Atlas atlas;
extern GLFWwindow *window;
extern entt::entity local_player;
extern reactphysics3d::PhysicsWorld *world;
extern double curtime;
extern double frametime;
extern double phys_interpfactor;
} // namespace globals
