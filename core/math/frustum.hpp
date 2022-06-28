/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 01:07:36.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <array>
#include <core/types.hpp>

namespace math
{
struct FPlane final {
    float d;
    vector3f_t n;
    float point(const vector3f_t &v) const;
};

class Frustum final {
public:
    Frustum();
    Frustum(const matrix4f_t &vpmatrix);
    void set(const matrix4f_t &vpmatrix);
    bool point(const vector3f_t &v) const;

private:
    std::array<math::FPlane, 6> planes;
};
} // namespace math
