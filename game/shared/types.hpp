/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Fri Jul 01 2022 20:26:57.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef F7C83737_2D58_4015_8307_B085B50B7618
#define F7C83737_2D58_4015_8307_B085B50B7618
#include <array>
#include <cstddef>
#include <functional>
#include <game/shared/const.hpp>
#include <glm/vec3.hpp>

using chunk_pos_t = glm::vec<3, glm::int32, glm::packed_highp>;
using local_pos_t = glm::vec<3, glm::int16, glm::packed_highp>;
using voxel_pos_t = glm::vec<3, glm::int64, glm::packed_highp>;
using voxel_idx_t = std::size_t;

// As of now voxel IDs are 8-bit values,
// not great not terrible, but in future
// with planned mod support, this won't be enough.
using voxel_t = glm::uint8;
using chunk_t = std::array<voxel_t, CHUNK_VOLUME>;

// READABILITY: instead of a zero use NULL_VOXEL.
constexpr static const voxel_t NULL_VOXEL = 0;

template<>
struct std::hash<chunk_pos_t> final {
    size_t operator()(const chunk_pos_t &cpos) const
    {
        return (cpos.x * 73856093) ^ (cpos.y * 19349663) ^ (cpos.z * 83492791);
    }
};


#endif /* F7C83737_2D58_4015_8307_B085B50B7618 */
