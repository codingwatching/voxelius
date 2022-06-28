/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 23:10:39.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <GLFW/glfw3.h>
#include <core/util/clock.hpp>
#include <game/client/globals.hpp>
#include <game/client/vclient.hpp>
#include <glad/gl.h>
#include <spdlog/spdlog.h>

static void onGlfwError(int code, const char *message)
{
    spdlog::error("glfw: {}", message);
}

void vclient::run()
{
    glfwSetErrorCallback(&onGlfwError);
    if(!glfwInit()) {
        spdlog::error("glfw: init failed.");
        std::terminate();
    }

    // Will figure that out later.
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    globals::window = glfwCreateWindow(640, 480, "Client", nullptr, nullptr);
    if(!globals::window) {
        spdlog::error("glfw: unable to open a window.");
        std::terminate();
    }

    globals::curtime = 0.0f;
    globals::frame_count = 0;
    globals::frametime = 0.0f;
    globals::frametime_avg = 0.0f;

    util::Clock<std::chrono::high_resolution_clock> clock;
    util::Clock<std::chrono::high_resolution_clock> clock_avg;
    while(!glfwWindowShouldClose(globals::window)) {
        globals::curtime = util::seconds<float>(clock.now().time_since_epoch());
        globals::frametime = util::seconds<float>(clock.reset());

        if(util::seconds<float>(clock_avg.elapsed()) > 0.0625f) {
            globals::frametime_avg += globals::frametime;
            globals::frametime_avg *= 0.5f;
            clock_avg.reset();
            spdlog::info("perf: {:.03f} ms ({:.02f} FPS)", globals::frametime_avg * 1000.0f, 1.0f / globals::frametime_avg);
        }

        glfwSwapBuffers(globals::window);
        glfwPollEvents();

        globals::frame_count++;
    }

    spdlog::info("client shutdown after {} frames. avg dt: {:.03f} ms ({:.02f} FPS)", globals::frame_count, globals::frametime_avg * 1000.0f, 1.0f / globals::frametime_avg);

    glfwDestroyWindow(globals::window);
    glfwTerminate();
}
