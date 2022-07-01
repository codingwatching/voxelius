/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Fri Jul 01 2022 01:51:06.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <reactphysics3d/body/RigidBody.h>

namespace comp
{
struct Body final {
    // Handle managed by PhysicsWorld
    reactphysics3d::RigidBody *body;
};
} // namespace comp
