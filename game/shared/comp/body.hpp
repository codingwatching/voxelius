/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Fri Jul 01 2022 01:51:06.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef D6EBAA0E_8CC1_4E80_9342_A6BC393F46A5
#define D6EBAA0E_8CC1_4E80_9342_A6BC393F46A5
#include <reactphysics3d/body/RigidBody.h>

namespace comp
{
struct Body final {
    // Handle managed by PhysicsWorld
    reactphysics3d::RigidBody *body;
};
} // namespace comp


#endif /* D6EBAA0E_8CC1_4E80_9342_A6BC393F46A5 */
