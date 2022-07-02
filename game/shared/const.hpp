/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Fri Jul 01 2022 18:16:28.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <common/cxpr.hpp>
#include <glm/vec3.hpp>

constexpr static const glm::dvec3 DIR_FORWARD = glm::dvec3(0.0, 0.0, -1.0);
constexpr static const glm::dvec3 DIR_RIGHT = glm::dvec3(1.0, 0.0, 0.0);
constexpr static const glm::dvec3 DIR_UP = glm::dvec3(0.0, 1.0, 0.0);

constexpr static const std::size_t CHUNK_SIZE = 16;
constexpr static const std::size_t CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
constexpr static const std::size_t CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
constexpr static const std::size_t CHUNK_SIZE_LOG2 = cxpr::log2(CHUNK_SIZE);

constexpr static const double PHYS_TICKRATE = 50.0;
constexpr static const double PHYS_TIMESTEP = 1.0 / PHYS_TICKRATE;
