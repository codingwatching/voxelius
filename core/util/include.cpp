/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 01:10:38.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <core/util/include.hpp>
#include <spdlog/fmt/fmt.h>
#include <stdio.h>

bool util::include(const vfs::vpath_t &vpath, std::string &out, const std::string &comment)
{
    vfs::file_t *vfile = vfs::open(vpath, vfs::OPEN_RD);
    if(!vfile)
        return false;

    out.clear();

    std::string line;
    while(vfs::readline(vfile, line)) {
        // I use sscanf here because it's a bit more
        // convenient to have the formatted input instead
        // of this earse(), insert() and find() fuckery.
        // Also I just wrote a fucking assembler using sscanf.
        char filename_str[128] = { 0 };
        if(sscanf(line.c_str(), " #include \"%127[^, \"\t\r\n]\"", filename_str) == 1) {
            if(!util::include(vpath.parent_path() / filename_str, out))
                out += fmt::format("{} include failed: {}\r\n", comment, filename_str);
            continue;
        }

        out += line;
        out += "\r\n";
    }

    return true;
}
