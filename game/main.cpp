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
#include <common/vfs.hpp>
#include <game/client/main.hpp>
#include <iostream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

static inline const vfs::rpath_t getRPathGame()
{
    std::string gamepath_arg;
    if(cmdline::get("gamepath", gamepath_arg)) {
        // Allow setting game path via cmdline.
        return vfs::rpath_t(gamepath_arg);
    }

    // TODO: if Voxelius ever gets uploaded
    // somewhere like AUR or any other package
    // database, make sure to ifdef the desired
    // content location specific for the system.

#if defined(BUILD_DEV)
    // We are building a development-friendly
    // version of the game, meaning the clown
    // behind the monitor wants it to use assets
    // provided in the source code tree.
    return fs_std::current_path() / "assets";
#endif

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    // We are building the game for a system that
    // has a lot of directories designed specifically
    // to store application's read-only data.
    return vfs::rpath_t("/usr/share/voxelius");
#endif

    // We are building the game for a system
    // that suspiciously looks like Windows.
    // In that case we can store things right
    // smack in the same place as executable.
    return fs_std::current_path();
}

static inline const vfs::rpath_t getRPathUser()
{
    std::string userpath_arg;
    if(cmdline::get("userpath", userpath_arg)) {
        // Allow setting user path via cmdline.
        return vfs::rpath_t(userpath_arg);
    }

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    if(const char *xdg_home = std::getenv("XDG_DATA_HOME")) {
        // We are building the game for an XDG-compliant
        // system. That means the user's home directory
        // can be retreived from XDG_DATA_HOME envar.
        return vfs::rpath_t(xdg_home) / ".voxelius";
    }

    if(const char *unix_home = std::getenv("HOME")) {
        // We are building the game for a system that
        // at least is POSIX-compliant. That means the
        // user's home directory can be retreived
        // from HOME environment variable.
        return vfs::rpath_t(unix_home) / ".voxelius";
    }
#endif

    if(const char *appdata = std::getenv("APPDATA")) {
        // We are building the game for a system
        // that suspiciously looks like Windows.
        // In that case we can use AppData/Roaming.
        return vfs::rpath_t(appdata) / "voxelius";
    }

    // We are building the game for a system
    // that at least has a working C++ compiler.
    // In that case we just use the same place
    // as with game subdirectory. What could go wrong?
    return fs_std::current_path();
}

int main(int argc, char **argv)
{
    try {
        // Try to setup a color stderr sink.
        spdlog::logger *lp = spdlog::default_logger_raw();
        lp->sinks().clear();
        lp->sinks().push_back(std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
        lp->set_pattern("[%H:%M:%S] %^[%L]%$ %v");
    }
    catch(const spdlog::spdlog_ex &ex) {
        // Failed! Fall back to iostream and die.
        std::cerr << "spdlog: " << ex.what() << std::endl;
        std::terminate();
    }

    cmdline::clear();
    cmdline::append(argc, argv);

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

    if(!vfs::init(argv[0])) {
        spdlog::critical("vfs: init failed: {}", vfs::getError());
        std::terminate();
    }

    const vfs::rpath_t rpath_game = getRPathGame();
    const vfs::rpath_t rpath_user = getRPathUser();

    spdlog::info("vfs: setting game path to {}", rpath_game.native());
    spdlog::info("vfs: setting user path to {}", rpath_user.native());

    fs_std::create_directories(rpath_game);
    fs_std::create_directories(rpath_user);

    if(!vfs::mount(rpath_game, vfs::getRootPath(), false)) {
        spdlog::critical("vfs: unable to mount {}: {}", rpath_game.native(), vfs::getError());
        std::terminate();
    }

    if(!vfs::mount(rpath_user, vfs::getRootPath(), false)) {
        spdlog::critical("vfs: unable to mount {}: {}", rpath_user.native(), vfs::getError());
        std::terminate();
    }

    if(!vfs::setWritePath(rpath_user)) {
        spdlog::critical("vfs: unable to set write path to {}: {}", rpath_user.native(), vfs::getError());
        std::terminate();
    }

#if defined(BUILD_VCL)
    spdlog::info("main: running client...");
    client::main();
#elif defined(BUILD_VDS)
    spdlog::info("main: running dedicated server...");
    spdlog::error("server: not implemented!");
#else
#    error Amogus
#endif

    if(!vfs::deinit()) {
        // Improper deinit is not a death sentence.
        spdlog::warn("vfs: deinit failed: {}", vfs::getError());
    }

    return 0;
}
