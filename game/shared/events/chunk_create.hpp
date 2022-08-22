/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Sun Jul 03 2022 00:54:53.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef BE575846_9549_4BB0_AD43_3912CF4F9000
#define BE575846_9549_4BB0_AD43_3912CF4F9000
#include <game/shared/chunks.hpp>

namespace events
{
struct ChunkCreate final {
    chunk_pos_t cpos;
    SharedChunk *chunk;
};
} // namespace events



#endif /* BE575846_9549_4BB0_AD43_3912CF4F9000 */
