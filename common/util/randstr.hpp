/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 01:12:03.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <common/math/constexpr.hpp>
#include <random>
#include <string>

namespace util
{
// Generates a random alphanumeric string
// using an STL-compliant random engine.
template<typename random_engine>
static inline const std::string randstr(random_engine &engine, size_t size)
{
    constexpr static const char alnums[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    constexpr static const size_t alnums_sz = math::arraySize(alnums);
    std::uniform_int_distribution<size_t> udist(0, alnums_sz - 1);
    std::string result = std::string(size, static_cast<char>(0));
    for(size_t i = 0; i < size; i++)
        result[i] = alnums[udist(engine)];
    return result;
}
} // namespace util
