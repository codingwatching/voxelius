/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Fri Jul 01 2022 20:12:04.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <GLFW/glfw3.h>
#include <game/client/events/screen_size.hpp>
#include <game/client/globals.hpp>
#include <game/client/screen.hpp>
#include <game/shared/globals.hpp>
#include <spdlog/spdlog.h>

static void onScreenSize(GLFWwindow *window, int width, int height)
{
    events::ScreenSize event = {};
    event.width = width;
    event.height = height;
    globals::dispatcher.trigger(event);
}

void screen::init()
{
    spdlog::debug("screen: taking over framebuffer events");
    glfwSetFramebufferSizeCallback(globals::window, &onScreenSize);
}

void screen::initLate()
{
    int width, height;
    glfwGetFramebufferSize(globals::window, &width, &height);
    onScreenSize(globals::window, width, height);
}

void screen::getSize(int &width, int &height)
{
    glfwGetFramebufferSize(globals::window, &width, &height);
}
