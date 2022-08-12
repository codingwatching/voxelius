/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Sun Jul 03 2022 00:29:46.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

namespace globals
{
extern entt::dispatcher dispatcher;
extern entt::registry registry;
} // namespace globals
