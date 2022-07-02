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
#include <game/client/atlas.hpp>
#include <game/client/globals.hpp>

Atlas client_globals::atlas;
GLFWwindow *client_globals::window = nullptr;
entt::entity client_globals::local_player = entt::null;
reactphysics3d::PhysicsWorld *client_globals::world = nullptr;
double client_globals::curtime = 0.0;
double client_globals::frametime = 0.0;
double client_globals::phys_interpfactor = 0.0;
