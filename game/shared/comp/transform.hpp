/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Thu Jun 30 2022 01:17:46.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <common/types.hpp>

namespace comp
{
struct Transform final {
    vector3f_t position { VECTOR3F_ZERO };
    quaternf_t rotation { QUATERNF_IDENT };
};
} // namespace comp
