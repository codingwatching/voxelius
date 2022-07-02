/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Sat Jul 02 2022 18:18:44.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <reactphysics3d/mathematics/Transform.h>

namespace comp
{
struct InterpTransform final {
    reactphysics3d::Transform prev;
    reactphysics3d::Transform interp;
};
} // namespace comp
