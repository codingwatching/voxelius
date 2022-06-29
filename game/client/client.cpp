/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 15:35:40.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <GLFW/glfw3.h>
#include <common/util/clock.hpp>
#include <exception>
#include <game/client/client.hpp>
#include <game/client/game.hpp>
#include <game/client/gl/context.hpp>
#include <game/client/globals.hpp>
#include <game/client/input.hpp>
#include <glad/gl.h>
#include <spdlog/spdlog.h>

static void onGlfwError(int code, const char *message)
{
    spdlog::error("glfw: error {}: {}", code, message);
}

void client::run()
{
    glfwSetErrorCallback(&onGlfwError);

    if(!glfwInit()) {
        spdlog::critical("glfw: init failed.");
        std::terminate();
    }

    // UNDONE: window should be able to
    // dynamically resize. But not now...
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Sets all the required window hints for the
    // GLFW to correctly prepare context for us.
    gl::earlyInit();

    globals::window = glfwCreateWindow(640, 480, "VX_CLIENT", nullptr, nullptr);
    if(!globals::window) {
        spdlog::critical("glfw: unable to open a window.");
        std::terminate();
    }

    glfwMakeContextCurrent(globals::window);

    gl::initialize();

    input::initialize();

    game::initialize();

    // Reset counters
    globals::epoch = 0.0f;
    globals::frametime = 0.0f;
    globals::frametime_avg = 0.0f;
    globals::frame_count = 0;

    util::Clock<std::chrono::high_resolution_clock> clock;
    while(!glfwWindowShouldClose(globals::window)) {
        globals::epoch = util::seconds<float>(clock.now().time_since_epoch());
        globals::frametime = util::seconds<float>(clock.reset());
        globals::frametime_avg += globals::frametime;
        globals::frametime_avg *= 0.5f;
        globals::frame_count++;

        game::update();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game::render();

        glfwSwapBuffers(globals::window);

        game::lateUpdate();

        glfwPollEvents();

        // Dispatch all the queued events.
        // We usually trigger() them but in some
        // cases it might be better to enqueue them.
        globals::dispatcher.update();
    }

    spdlog::info("client shutdown after {} frames.", globals::frame_count);
    spdlog::info("average frametime: {:.03f} ms ({:.02f} FPS)", globals::frametime_avg * 1.0e3f, 1.0f / globals::frametime_avg);

    game::shutdown();

    glfwDestroyWindow(globals::window);
    glfwTerminate();
}
