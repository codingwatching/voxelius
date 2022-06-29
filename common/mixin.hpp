/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 00:50:16.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
namespace mixin
{
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable &rhs) = delete;
    NonCopyable &operator=(const NonCopyable &rhs) = delete;
    virtual ~NonCopyable() = default;
};

class NonMovable {
public:
    NonMovable() = default;
    NonMovable(NonMovable &&rhs) = delete;
    NonMovable &operator=(NonMovable &&rhs) = delete;
    virtual ~NonMovable() = default;
};
} // namespace mixin
