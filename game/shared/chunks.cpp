/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Sun Jul 03 2022 00:59:02.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <game/shared/chunks.hpp>
#include <game/shared/comp/chunk.hpp>
#include <game/shared/convert.hpp>
#include <game/shared/events/chunk_create.hpp>
#include <game/shared/events/chunk_destroy.hpp>
#include <game/shared/events/voxel_set.hpp>
#include <game/shared/globals.hpp>
#include <spdlog/spdlog.h>
#include <unordered_map>

static std::unordered_map<chunk_pos_t, SharedChunk> chunks_map;

void chunks::clear()
{
    for(auto it = chunks_map.begin(); it != chunks_map.end(); it++) {
        events::ChunkDestroy event = {};
        event.cpos = it->first;
        event.chunk = &it->second;
        globals::dispatcher.trigger(event);
        globals::registry.destroy(it->second.entity);
    }

    chunks_map.clear();
}

void chunks::remove(const chunk_pos_t &cpos)
{
    const auto it = chunks_map.find(cpos);
    if(it != chunks_map.cend()) {
        events::ChunkDestroy event = {};
        event.cpos = it->first;
        event.chunk = &it->second;
        globals::dispatcher.trigger(event);
        globals::registry.destroy(it->second.entity);
        chunks_map.erase(it);
    }
}

// FIXME: should it be renamed to findOrCreate?
// Because it does the exact thing findOrCreate
// would do...
SharedChunk *chunks::create(const chunk_pos_t &cpos)
{
    const auto it = chunks_map.find(cpos);
    if(it == chunks_map.cend()) {
        SharedChunk &chunk = (chunks_map[cpos] = SharedChunk());
        chunk.chunk.fill(NULL_VOXEL);
        chunk.entity = globals::registry.create();
        chunk.client = nullptr;
        chunk.server = nullptr;

        comp::Chunk &cc = globals::registry.emplace<comp::Chunk>(chunk.entity);
        cc.position = cpos;

        events::ChunkCreate event = {};
        event.cpos = cpos;
        event.chunk = &chunk;
        globals::dispatcher.trigger(event);

        return &chunk;
    }

    return &it->second;
}

SharedChunk *chunks::find(const chunk_pos_t &cpos)
{
    const auto it = chunks_map.find(cpos);
    if(it == chunks_map.cend())
        return nullptr;
    return &it->second;
}

voxel_t chunks::get(const voxel_pos_t &vpos)
{
    if(const SharedChunk *chunk = chunks::find(convert::toChunkPosition(vpos)))
        return chunk->chunk.at(convert::toVoxelIndex(convert::toLocalPosition(vpos)));
    return NULL_VOXEL;
}

voxel_t chunks::get(const chunk_pos_t &cpos, const local_pos_t &lpos)
{
    return chunks::get(convert::toVoxelPosition(cpos, lpos));
}

bool chunks::set(const voxel_pos_t &vpos, voxel_t voxel, bool force)
{
    const chunk_pos_t cpos = convert::toChunkPosition(vpos);
    if(SharedChunk *chunk = force ? chunks::create(cpos) : chunks::find(cpos)) {
        events::VoxelSet event = {};
        event.cpos = cpos;
        event.lpos = convert::toLocalPosition(vpos);
        event.index = convert::toVoxelIndex(event.lpos);
        event.chunk = chunk;
        event.voxel = voxel;
        chunk->chunk.at(event.index) = voxel;
        globals::dispatcher.trigger(event);
        return true;
    }

    return false;
}

bool chunks::set(const chunk_pos_t &cpos, const local_pos_t &lpos, voxel_t voxel, bool force)
{
    return chunks::set(convert::toVoxelPosition(cpos, lpos), voxel, force);
}
