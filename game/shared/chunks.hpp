/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Sat Jul 02 2022 16:00:10.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <entt/entity/entity.hpp>
#include <game/shared/types.hpp>
#include <memory>

struct ClientChunk;
struct ServerChunk;
struct SharedChunk final {
    chunk_t chunk;
    entt::entity entity;
    ClientChunk *client;
    ServerChunk *server;
};

namespace chunks
{
void clear();
void remove(const chunk_pos_t &cpos);
SharedChunk *create(const chunk_pos_t &cpos);
SharedChunk *find(const chunk_pos_t &cpos);
voxel_t get(const voxel_pos_t &vpos);
voxel_t get(const chunk_pos_t &cpos, const local_pos_t &lpos);
bool set(const voxel_pos_t &vpos, voxel_t voxel, bool force = false);
bool set(const chunk_pos_t &cpos, const local_pos_t &lpos, voxel_t voxel, bool force = false);
} // namespace chunks
