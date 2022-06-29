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
#include <common/cmdline.hpp>
#include <common/util/spdlog_sinks.hpp>
#include <common/vfs.hpp>
#include <game/client/client.hpp>
#include <iostream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// Returns a native system-specific path
// to a place where the game should store
// its default configuration files.
static inline const vfs::rpath_t getDefaultRPath()
{
    const vfs::rpath_t fallback = fs_std::current_path() / "default";

    std::string dpath_arg;
    if(cmdline::get("dpath", dpath_arg)) {
        // Allow path overriding via cmdline.
        return vfs::rpath_t(dpath_arg);
    }

#if defined(BUILD_DEV)
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

// Returns a native system-specific path
// to a place where the game should write
// configuration files, screenshots and logs.
static inline const vfs::rpath_t getWriteRPath()
{
    constexpr static const char *DOT_PATH = ".voxelius";
    constexpr static const char *NORM_PATH = "voxelius";
    const vfs::rpath_t fallback = fs_std::current_path() / "rwroot";

    std::string wpath_arg;
    if(cmdline::get("wpath", wpath_arg)) {
        // Allow path overriding via cmdline.
        return vfs::rpath_t(wpath_arg);
    }

#if defined(BUILD_DEV)
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

int main(int argc, char **argv)
{
    try {
        // Try to setup a color stderr sink.
        spdlog::logger *lp = spdlog::default_logger_raw();
        lp->sinks().clear();
        lp->sinks().push_back(util::makeSinkSimple<spdlog::sinks::stderr_color_sink_mt>());
    }
    catch(const spdlog::spdlog_ex &ex) {
        // Failed! Fall back to iostream and die.
        std::cerr << "spdlog: " << ex.what() << std::endl;
        std::terminate();
    }

    cmdline::clear();
    cmdline::append(argc, argv);

#if defined(BUILD_DEV)
    spdlog::warn("Development build.");
    spdlog::warn("Local paths (source tree) will be used for mounting.");
#endif

#if defined(NDEBUG)
    // When not built for debugging,
    // we still might want to see the
    // log messages with DEBUG severity.
    const bool is_debug = cmdline::exists("debug");
#else
    const bool is_debug = true;
#endif

    spdlog::set_level(spdlog::level::info);
    if(cmdline::exists("trace")) {
        spdlog::set_level(spdlog::level::trace);
        spdlog::info("spdlog: set_level(trace) via cmdline");
    }
    else if(is_debug) {
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("spdlog: set_level(debug)");
    }

    if(!vfs::initialize(argv[0])) {
        spdlog::critical("vfs: initialize failed: {}", vfs::getError());
        std::terminate();
    }

    const vfs::rpath_t rpath_def = getDefaultRPath();
    const vfs::rpath_t rpath_rwr = getWriteRPath();

    spdlog::info("vfs: rpath_def={}", rpath_def.native());
    spdlog::info("vfs: rpath_rwr={}", rpath_rwr.native());

    fs_std::create_directories(rpath_rwr);

    if(!vfs::mount(rpath_def, vfs::getRootPath(), true)) {
        // Not a death sentence but still kind of bad.
        spdlog::warn("vfs: unable to mount {}: {}", rpath_def.native(), vfs::getError());
    }

    if(!vfs::setWritePath(rpath_rwr)) {
        spdlog::critical("vfs: unable to set write path to {}: {}", rpath_rwr.native(), vfs::getError());
        std::terminate();
    }

    // UNDONE: at this point we might want to load
    // a special magic list that should contain all
    // the possible subdirectories that contain assets.

    if(!vfs::mount(rpath_rwr, vfs::getRootPath(), false)) {
        spdlog::critical("vfs: unable to mount {}: {}", rpath_rwr.native(), vfs::getError());
        std::terminate();
    }

#if defined(BUILD_VCL)
    spdlog::info("Running client...");
    client::run();
#elif defined(BUILD_VDS)
    spdlog::info("Running dedicated server...");
    spdlog::error("Not implemented!");
#else
#    error Amogus
#endif

    if(!vfs::shutdown()) {
        // Improper shutdown is not a death sentence.
        spdlog::warn("vfs: shutdown failed: {}", vfs::getError());
    }

    return 0;
}
