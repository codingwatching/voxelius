/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 01:16:56.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <algorithm>
#include <cctype>
#include <common/cmdline.hpp>
#include <common/math/constexpr.hpp>
#include <common/util/spdlog_sinks.hpp>
#include <common/vfs.hpp>
#include <iostream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int argc, char **argv)
{
    try {
        spdlog::logger *lp = spdlog::default_logger_raw();
        lp->sinks().clear();
        lp->sinks().push_back(util::makeSinkSimple<spdlog::sinks::stderr_color_sink_mt>());
    }
    catch(const spdlog::spdlog_ex &ex) {
        // fall back to iostream and die
        std::cerr << "spdlog: " << ex.what() << std::endl;
        std::terminate();
    }

    cmdline::append(argc, argv);

#if defined(NDEBUG)
    const bool is_debug = cmdline::exists("debug");
#else
    const bool is_debug = true;
#endif

    spdlog::set_level(spdlog::level::info);
    if(cmdline::exists("trace")) {
        spdlog::set_level(spdlog::level::trace);
        spdlog::info("setting log_level to TRACE (cmdline)");
    }
    else if(is_debug) {
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("setting log_level to DEBUG");
    }

    vfs::addPathRO_A("base");
    vfs::addPathRO_A("assets");

    if(!vfs::setRootRW(fs_std::current_path() / "rwroot")) {
        spdlog::error("fs: unable to find or create rw_root directory");
        std::terminate();
    }

    spdlog::info("main.cpp stub in action lmao");

    // Ouch
    std::ofstream ofile = vfs::openWr("test.txt", vfs::IO_APPEND);
    vfs::writeString(ofile, "appending!");

#if defined(VGAME_CLIENT)
    // run client here
#else
#error Amogus
#endif

    return 0;
}
