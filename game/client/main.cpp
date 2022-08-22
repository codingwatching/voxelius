/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Wed Jun 29 2022 15:35:40.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <GLFW/glfw3.h>
#include <common/clock.hpp>
#include <common/cmdline.hpp>
#include <common/cxpr.hpp>
#include <game/client/game.hpp>
#include <game/client/globals.hpp>
#include <game/client/image.hpp>
#include <game/client/main.hpp>
#include <game/shared/const.hpp>
#include <game/shared/globals.hpp>
#include <glad/gl.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>
#include <vector>

static void onGlfwError(int, const char *message)
{
    spdlog::error("glfw: {}", message);
}

static void GLAPIENTRY onGlMessage(GLenum, GLenum, GLuint, GLenum, GLsizei, const char *message, const void *)
{
    spdlog::debug("gl: {}", message);
}

void client::main()
{
    reactphysics3d::PhysicsCommon phys_common = reactphysics3d::PhysicsCommon();

    glfwSetErrorCallback(&onGlfwError);

    if(!glfwInit()) {
        spdlog::critical("glfw: init failed");
        std::terminate();
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // UNDONE: window dimensions in configs
    globals::window = glfwCreateWindow(640, 480, "Client", nullptr, nullptr);
    if(!globals::window) {
        spdlog::critical("glfw: unable to open a window");
        std::terminate();
    }

    const std::filesystem::path icon_path = std::filesystem::path("32x32.png");
    if(Image icon_image = Image(icon_path, false); icon_image.valid()) {
        GLFWimage icon = {};
        icon.width = icon_image.getWidth();
        icon.height = icon_image.getHeight();
        icon.pixels = reinterpret_cast<unsigned char *>(icon_image.data());
        glfwSetWindowIcon(globals::window, 1, &icon);
    }

    glfwMakeContextCurrent(globals::window);

    // UNDONE: vsync in configs
    glfwSwapInterval(1);

    if(!gladLoadGL(&glfwGetProcAddress)) {
        spdlog::critical("gl: unable to load OpenGL functions");
        std::terminate();
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(&onGlMessage, nullptr);

    // NVIDIA drivers tend to flood the console
    // with additional buffer information messages.
    const GLuint ignore_nvidia_131185 = 131185;
    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 1, &ignore_nvidia_131185, GL_FALSE);

    // Print some information on the GPU
    // UNDONE: ask Windows for a discrete accelerator...
    spdlog::info("GLSL version: {}", reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    spdlog::info("OpenGL version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    spdlog::info("OpenGL renderer: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));

    globals::world = phys_common.createPhysicsWorld();

    Clock<std::chrono::high_resolution_clock> clock;

    globals::curtime = clock.seconds(clock.now().time_since_epoch());
    globals::frametime = 0.0;
    double phys_accum = 0.0;

    client_game::init();
    client_game::initLate();

    while(!glfwWindowShouldClose(globals::window)) {
        globals::curtime = clock.seconds(clock.now().time_since_epoch());
        globals::frametime = clock.seconds(clock.reset());

        phys_accum += globals::frametime;
        while(phys_accum >= PHYS_TIMESTEP) {
            client_game::updateFixed();
            globals::world->update(PHYS_TIMESTEP);
            phys_accum -= PHYS_TIMESTEP;
        }

        // The game can decide whether
        // to interpolate physics objects or not.
        globals::phys_interpfactor = phys_accum / globals::frametime;

        client_game::update();

        // Make sure we don't have a stray program
        // bound to the state. Usually third-party
        // software (such as overlays, eg RivaTuner)
        // binds its internal programs which in turn
        // causes a visual mess with separable programs.
        glUseProgram(0);

        client_game::render();

        glfwSwapBuffers(globals::window);

        client_game::updateLate();

        glfwPollEvents();

        // It looks like a good idea to be sure
        // that we dispatch everything that just
        // happens to exist in the event queue.
        globals::dispatcher.update();
    }

    client_game::deinit();

    phys_common.destroyPhysicsWorld(globals::world);

    // We are going to destroy the OpenGL context
    // so we have to make sure there is no GLXX
    // objects that hold an OpenGL object handle.
    // Otherwise, GLXX object destructors will be
    // called when there is no OpenGL context anymore,
    // resulting in a nasty segmentation fault.
    globals::registry.clear();

    glfwDestroyWindow(globals::window);

    glfwTerminate();
}
