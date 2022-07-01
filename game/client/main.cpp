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
#include <common/clock.hpp>
#include <game/client/game.hpp>
#include <game/client/globals.hpp>
#include <game/client/main.hpp>
#include <game/shared/const.hpp>
#include <glad/gl.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <spdlog/spdlog.h>

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

    // UNDONE: window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // UNDONE: window dimensions in configs
    client_globals::window = glfwCreateWindow(640, 480, "Client", nullptr, nullptr);
    if(!client_globals::window) {
        spdlog::critical("glfw: unable to open a window");
        std::terminate();
    }

    glfwMakeContextCurrent(client_globals::window);

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

    client_globals::world = phys_common.createPhysicsWorld();

    client_game::init();

    client_globals::curtime = 0.0;
    client_globals::frametime = 0.0;
    double phys_accum = 0.0;

    Clock<std::chrono::high_resolution_clock> clock;

    while(!glfwWindowShouldClose(client_globals::window)) {
        client_globals::curtime = clock.seconds(clock.now().time_since_epoch());
        client_globals::frametime = clock.seconds(clock.reset());

        phys_accum += client_globals::frametime;
        while(phys_accum >= PHYS_TIMESTEP) {
            client_game::updateFixed();
            client_globals::world->update(PHYS_TIMESTEP);
            phys_accum -= PHYS_TIMESTEP;
        }

        // The game can decide whether
        // to interpolate physics objects or not.
        client_globals::interpfactor = phys_accum / client_globals::frametime;

        client_game::update();

        // Make sure we don't have a stray program
        // bound to the state. Usually third-party
        // software (such as overlays, eg RivaTuner)
        // binds its internal programs which in turn
        // causes a visual mess with separable programs.
        glUseProgram(0);

        client_game::render();

        glfwSwapBuffers(client_globals::window);

        client_game::updateLate();

        glfwPollEvents();

        // It looks like a good idea to be sure
        // that we dispatch everything that just
        // happens to exist in the event queue.
        client_globals::dispatcher.update();
    }

    client_game::deinit();

    phys_common.destroyPhysicsWorld(client_globals::world);

    // We are going to destroy the OpenGL context
    // so we have to make sure there is no GLXX
    // objects that hold an OpenGL object handle.
    // Otherwise, GLXX object destructors will be
    // called when there is no OpenGL context anymore,
    // resulting in a nasty segmentation fault.
    client_globals::registry.clear();

    glfwDestroyWindow(client_globals::window);

    glfwTerminate();
}
