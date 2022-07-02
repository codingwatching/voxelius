/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Sun Jul 03 2022 00:56:33.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <game/shared/chunks.hpp>

namespace events
{
struct VoxelSet final {
    chunk_pos_t cpos;
    local_pos_t lpos;
    voxel_idx_t index;
    SharedChunk *chunk;
    voxel_t voxel;
};
} // namespace events
