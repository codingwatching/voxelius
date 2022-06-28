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
#include <core/cmdline.hpp>
#include <core/math/constexpr.hpp>
#include <core/util/spdlog_sinks.hpp>
#include <core/vfs.hpp>
#include <cstdlib>
#include <game/client/vclient.hpp>
#include <iostream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace detail
{
static inline const vfs::rpath_t defaultPath()
{
    const vfs::rpath_t fallback = fs_std::current_path() / "default";

    std::string dpath_arg;
    if(cmdline::get("dpath", dpath_arg)) {
        // Allow path overriding via cmdline.
        return vfs::rpath_t(dpath_arg);
    }

#if defined(V_DEVBUILD)
    // The "default" or fallback directory is present
    // in the source code tree, so we might like to
    // use it during development and use it as a
    // backup solution on release/distribution builds.
    return fallback;
#endif

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    // On UNIX and UNIX-like systems we can straight up
    // read from the specifically designed subdirectory
    // that should contain all our default configuration files.
    return vfs::rpath_t("/etc/games/voxelius");
#else
    // On any other system (including Windows), we want
    // to store default configuration files in a directory
    // neighbouring the executable, so we "fall back" to it.
    return fallback;
#endif
}

static inline const vfs::rpath_t writePath()
{
    constexpr static const char *DOT_PATH = ".voxelius";
    constexpr static const char *NORM_PATH = "voxelius";
    const vfs::rpath_t fallback = fs_std::current_path() / "rwroot";

    std::string wpath_arg;
    if(cmdline::get("wpath", wpath_arg)) {
        // Allow path overriding via cmdline.
        return vfs::rpath_t(wpath_arg);
    }

#if defined(V_DEVBUILD)
    // The "rwroot" or fallback directory is present
    // in the source code tree, so we might like to
    // use it during development and use it as a
    // backup solution on release/distribution builds.
    return fallback;
#endif

#if defined(_WIN32)
    if(const char *appdata = std::getenv("APPDATA")) {
        // For Windows, we write configs, screenshots
        // and logs to the AppData/Roaming/NORM_PATH.
        return vfs::rpath_t(appdata) / NORM_PATH;
    }
#endif

    if(const char *xdg_home = std::getenv("XDG_DATA_HOME")) {
        // For UNIX and UNIX-like systems we write configs,
        // screenshots and logs to ~/DOT_PATH. In case of
        // whatever XDG-compliant environment, we can yoink
        // the home path from XDG_DATA_HOME environment variable.
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        return vfs::rpath_t(xdg_home) / DOT_PATH;
    }

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    if(const char *homedir = std::getenv("HOME")) {
        // For UNIX and UNIX-like systems we write configs,
        // screenshots and logs to ~/DOT_PATH. In case of
        // whatever POSIX-compliant system, we can yoink
        // the home path from HOME environment variable.
        // https://pubs.opengroup.org/onlinepubs/009696899/basedefs/xbd_chap08.html
        return vfs::rpath_t(homedir) / DOT_PATH;
    }
#endif

    return fallback;
}
} // namespace detail

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

    if(!vfs::init(argv[0])) {
        spdlog::critical("vfs: init failed: {}", vfs::error());
        std::terminate();
    }

    const vfs::rpath_t rpath_default = detail::defaultPath();
    spdlog::info("vfs: rpath_default=[{}]", rpath_default.string());

    if(!vfs::mount(rpath_default, vfs::root(), true)) {
        // Lack of default/fallback configuration files
        // is bad but nowhere near to being critical.
        spdlog::warn("vfs: mounting {} failed: {}", rpath_default.string(), vfs::error());
    }

    const vfs::rpath_t rpath_write = detail::writePath();
    spdlog::info("vfs: rpath_write=[{}]", rpath_write.string());
    fs_std::create_directories(rpath_write);

    if(!vfs::setwr(rpath_write)) {
        // Now lack of write path is critical!
        spdlog::critical("vfs: unable to set write path to {}: {}", rpath_write.string(), vfs::error());
        std::terminate();
    }

    // TODO:
    // Somewhere around this (exactly inbetween
    // setting the write path and mounting it) we
    // have to load a list of locations we want to
    // possibly mount and at least try to mount them.

    if(!vfs::mount(rpath_write, vfs::root(), false)) {
        // Lack of write path readability is also critical!
        spdlog::critical("vfs: mounting {} failed: {}", rpath_write.string(), vfs::error());
        std::terminate();
    }

#if defined(V_CLIENT)
    vclient::run();
#else
#    error Amogus
#endif

    if(!vfs::shutdown()) {
        // Improper shutdown isn't something we would worry about.
        spdlog::warn("vfs: shutdown failed: {}", vfs::error());
    }

    return 0;
}
