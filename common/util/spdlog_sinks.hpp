/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 01:12:51.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <spdlog/spdlog.h>
#include <string_view>

namespace util
{
template<typename sink_type, typename... args_type>
inline spdlog::sink_ptr makeSink(const std::string_view pattern, args_type &&... args)
{
    sink_type *sink = new  sink_type(std::forward<args_type>(args)...);
    sink->set_pattern(std::string(pattern.cbegin(), pattern.cend()));
    return spdlog::sink_ptr(sink);
}

template<typename sink_type, typename... args_type>
inline spdlog::sink_ptr makeSinkSimple(args_type &&... args)
{
    // This is a "default" nicely looking format.
    // This is for things that usually are either
    // printed to files or terminals.
    // Practice (old source tree, attempts at developer
    // console with JS input) showed that it doesn't
    // look good for in-game terminal-ish things.
    return util::makeSink<sink_type>("[%H:%M:%S] %^[%L]%$ %v", std::forward<args_type>(args)...);
}
} // namespace util
