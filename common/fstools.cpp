/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Fri Jul 01 2022 00:49:59.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <common/fstools.hpp>
#include <spdlog/fmt/fmt.h>
#include <stdio.h>

bool fstools::include(const vfs::vpath_t &path, std::string &out, const std::string &comment)
{
    vfs::file_t *file = vfs::open(path, vfs::OPEN_RD);
    if(!file)
        return false;

    out.clear();

    std::string line;
    while(vfs::readLine(file, line)) {
        // I use sscanf here because it's a bit more
        // convenient to have the formatted input instead
        // of this earse(), insert() and find() fuckery.
        // Also I just wrote a fucking assembler using sscanf.
        char incpath_str[128] = { 0 };
        if(sscanf(line.c_str(), " #include \"%127[^, \"\t\r\n]\"", incpath_str) == 1) {
            const vfs::vpath_t incpath_vp = vfs::vpath_t(incpath_str);
            const vfs::vpath_t incpath = incpath_vp.is_absolute() ? incpath_vp : (path.parent_path() / incpath_vp);
            if(!fstools::include(incpath, out))
                out += fmt::format("{} include failed: {}\r\n", comment, incpath.string());
            continue;
        }

        out += line;
        out += "\r\n";
    }

    return true;
}

