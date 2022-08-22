/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Sat Jul 02 2022 18:18:44.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef DD3B4344_DC3D_46BB_B83C_46F5217E6A6A
#define DD3B4344_DC3D_46BB_B83C_46F5217E6A6A
#include <reactphysics3d/mathematics/Transform.h>

namespace comp
{
struct InterpTransform final {
    reactphysics3d::Transform prev;
    reactphysics3d::Transform interp;
};
} // namespace comp


#endif /* DD3B4344_DC3D_46BB_B83C_46F5217E6A6A */
