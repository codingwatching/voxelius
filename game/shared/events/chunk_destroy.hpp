/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Sun Jul 03 2022 00:55:38.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef C6E3163D_8F96_46ED_A1C8_491A81ECDC58
#define C6E3163D_8F96_46ED_A1C8_491A81ECDC58
#include <game/shared/chunks.hpp>

namespace events
{
struct ChunkDestroy final {
    chunk_pos_t cpos;
    SharedChunk *chunk;
};
} // namespace events


#endif /* C6E3163D_8F96_46ED_A1C8_491A81ECDC58 */
