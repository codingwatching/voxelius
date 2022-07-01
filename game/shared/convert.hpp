/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Fri Jul 01 2022 01:14:59.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <game/shared/types.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Vector2.h>
#include <reactphysics3d/mathematics/Vector3.h>

// WHY DOUBLE PRECISION?
// Considering the core concept of Voxelius being
// a Minecraft clone but without Minecraft's limitations,
// it would be just logical to assume that the world size
// should be really close to infinity, meaning we need
// more space to store coordinates. The 32-bit float may
// just fail at some big coordinates.
// So in general it would be just easier to consider a
// more precise floating point format than to try and hack
// our way into acheiving billionth coordinates while having
// single precision coordinates in shaders.

// WHY A PHYSICS ENGINE?
// Optimizations and not reinventing a wheel.
// I just assume that the game code must be much
// more tame with a single collision mesh per chunk
// than with all this messing around with AABB.
// Also technically would allow usage of non-cubic
// voxel models ([interpolated] marching cubes?).

namespace convert
{
static inline const glm::dquat toGLM(const reactphysics3d::Quaternion &q)
{
    return glm::dquat(q.w, q.x, q.y, q.z);
}

static inline const glm::dvec2 toGLM(const reactphysics3d::Vector2 &v)
{
    return glm::dvec2(v.x, v.y);
}

static inline const glm::dvec3 toGLM(const reactphysics3d::Vector3 &v)
{
    return glm::dvec3(v.x, v.y, v.z);
}

static inline const reactphysics3d::Quaternion toRP3D(const glm::dquat &q)
{
    return reactphysics3d::Quaternion(q.x, q.y, q.z, q.w);
}

static inline const reactphysics3d::Vector2 toRP3D(const glm::dvec2 &v)
{
    return reactphysics3d::Vector2(v.x, v.y);
}

static inline const reactphysics3d::Vector3 toRP3D(const glm::dvec3 &v)
{
    return reactphysics3d::Vector3(v.x, v.y, v.z);
}

constexpr static inline const chunk_pos_t toChunkPosition(const glm::dvec3 &wpos)
{
    return chunk_pos_t {
        static_cast<chunk_pos_t::value_type>(wpos.x) >> CHUNK_SIZE_LOG2,
        static_cast<chunk_pos_t::value_type>(wpos.y) >> CHUNK_SIZE_LOG2,
        static_cast<chunk_pos_t::value_type>(wpos.z) >> CHUNK_SIZE_LOG2
    };
}

constexpr static inline const chunk_pos_t toChunkPosition(const voxel_pos_t &vpos)
{
    return chunk_pos_t {
        static_cast<chunk_pos_t::value_type>(vpos.x >> CHUNK_SIZE_LOG2),
        static_cast<chunk_pos_t::value_type>(vpos.y >> CHUNK_SIZE_LOG2),
        static_cast<chunk_pos_t::value_type>(vpos.z >> CHUNK_SIZE_LOG2)
    };
}

constexpr static inline const local_pos_t toLocalPosition(const voxel_pos_t &vpos)
{
    return local_pos_t {
        glm::abs(static_cast<local_pos_t::value_type>(vpos.x % CHUNK_SIZE)),
        glm::abs(static_cast<local_pos_t::value_type>(vpos.y % CHUNK_SIZE)),
        glm::abs(static_cast<local_pos_t::value_type>(vpos.z % CHUNK_SIZE))
    };
}

constexpr static inline const local_pos_t toLocalPosition(const voxel_idx_t &index)
{
    return local_pos_t {
        static_cast<local_pos_t::value_type>(index / CHUNK_AREA),
        static_cast<local_pos_t::value_type>(index % CHUNK_SIZE),
        static_cast<local_pos_t::value_type>((index / CHUNK_SIZE) % CHUNK_SIZE)
    };
}

constexpr static inline const voxel_pos_t toVoxelPosition(const glm::dvec3 &wpos)
{
    return voxel_pos_t(wpos.x, wpos.y, wpos.z);
}

constexpr static inline const voxel_pos_t toVoxelPosition(const chunk_pos_t &cpos, const local_pos_t &lpos)
{
    return voxel_pos_t {
        static_cast<voxel_pos_t::value_type>(lpos.x + (cpos.x << CHUNK_SIZE_LOG2)),
        static_cast<voxel_pos_t::value_type>(lpos.y + (cpos.y << CHUNK_SIZE_LOG2)),
        static_cast<voxel_pos_t::value_type>(lpos.z + (cpos.z << CHUNK_SIZE_LOG2))
    };
}

constexpr static inline const voxel_idx_t toVoxelIndex(const local_pos_t &lpos)
{
    return static_cast<voxel_idx_t>((lpos.x * CHUNK_SIZE + lpos.z) * CHUNK_SIZE + lpos.y);
}

constexpr static inline const glm::dvec3 toChunkWorldPosition(const chunk_pos_t &cpos)
{
    return glm::dvec3 {
        static_cast<float>(cpos.x << CHUNK_SIZE_LOG2),
        static_cast<float>(cpos.y << CHUNK_SIZE_LOG2),
        static_cast<float>(cpos.z << CHUNK_SIZE_LOG2),
    };
}
} // namespace convert
