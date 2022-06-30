/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 19:22:06.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <GLFW/glfw3.h>
#include <common/cmdline.hpp>
#include <exception>
#include <game/client/gl/context.hpp>
#include <glad/gl.h>
#include <spdlog/spdlog.h>
#include <unordered_set>
#include <vector>

static void GLAPIENTRY debugCallback(GLenum, GLenum, GLuint id, GLenum, GLsizei, const char *message, const void *)
{
    spdlog::debug("gl: [{}]: {}", id, message);
}

void gl::earlyInit()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
}

void gl::initialize()
{
    if(!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress))) {
        spdlog::error("Unable to load OpenGL functions");
        std::terminate();
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);

    // NVIDIA drivers tend to flood the console
    // with additional buffer information messages.
    const GLuint ignore_nvidia_131185 = 131185;
    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 1, &ignore_nvidia_131185, GL_FALSE);
}
