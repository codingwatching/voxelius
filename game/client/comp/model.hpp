/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Fri Jul 01 2022 19:49:43.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

namespace comp
{
struct Model final {
    glm::dmat4x4 matrix { glm::identity<glm::dmat4x4>() };
};
} // namespace comp
