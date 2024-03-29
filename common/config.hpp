/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Tue Jun 28 2022 00:54:00.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef FC7C05E9_9E21_4988_89B6_255408A064F3
#define FC7C05E9_9E21_4988_89B6_255408A064F3
#include <common/vfs.hpp>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>
#include <toml.hpp>

template<typename impl_type>
class TomlConfig {
public:
    bool read(const std::filesystem::path &path);
    void write(const std::filesystem::path &path);

    // implementations define:
    //  void impl_postRead();
    //  void impl_preWrite();

public:
    toml::table toml;
};

template<typename impl_type>
inline bool TomlConfig<impl_type>::read(const std::filesystem::path &path)
{
    bool success = true;

    try {
        std::string source;
        if(!vfs::readString(path, source))
            throw std::runtime_error(vfs::error());
        toml = toml::parse(source);
    }
    catch(const std::exception &ex) {
        spdlog::error("toml: parsing {} failed: {}", path.string(), ex.what());
        toml = toml::table();
        success = false;
    }

    static_cast<impl_type *>(this)->impl_postRead();

    return success;
}

template<typename impl_type>
inline void TomlConfig<impl_type>::write(const std::filesystem::path &path)
{
    static_cast<impl_type *>(this)->impl_preWrite();
    std::stringstream ss;
    ss << toml;
    vfs::File::writeString(path, ss.str());
}


#endif /* FC7C05E9_9E21_4988_89B6_255408A064F3 */
