/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Sun Jul 03 2022 00:56:33.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef B88BA98B_02A0_4A83_B0C2_CA5B6D68996D
#define B88BA98B_02A0_4A83_B0C2_CA5B6D68996D
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


#endif /* B88BA98B_02A0_4A83_B0C2_CA5B6D68996D */
