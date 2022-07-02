/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 22:18:19.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <GLFW/glfw3.h>
#include <game/client/events/cursor_move.hpp>
#include <game/client/events/keyboard_key.hpp>
#include <game/client/events/mouse_button.hpp>
#include <game/client/events/mouse_scroll.hpp>
#include <game/client/globals.hpp>
#include <game/client/input.hpp>
#include <game/shared/globals.hpp>
#include <spdlog/spdlog.h>

static double last_xpos = 0.0f;
static double last_ypos = 0.0f;
static void onCursorMove(GLFWwindow *window, double xpos, double ypos)
{
    events::CursorMove event = {};
    event.xpos = xpos;
    event.ypos = ypos;
    event.dx = xpos - last_xpos;
    event.dy = ypos - last_ypos;
    shared_globals::dispatcher.trigger(event);

    // Store previous positions
    last_xpos = xpos;
    last_ypos = ypos;
}

static void onKeyboardKey(GLFWwindow *window, int key, int scn, int act, int mod)
{
    events::KeyboardKey event = {};
    event.act = static_cast<unsigned int>(act);
    event.key = static_cast<unsigned int>(key);
    event.mod = static_cast<unsigned int>(mod);
    shared_globals::dispatcher.trigger(event);
}

static void onMouseButton(GLFWwindow *window, int btn, int act, int mod)
{
    events::MouseButton event = {};
    event.act = static_cast<unsigned int>(act);
    event.btn = static_cast<unsigned int>(btn);
    event.mod = static_cast<unsigned int>(mod);
    shared_globals::dispatcher.trigger(event);
}

static void onMouseScroll(GLFWwindow *window, double dx, double dy)
{
    events::MouseScroll event = {};
    event.dx = dx;
    event.dy = dy;
    shared_globals::dispatcher.trigger(event);
}

void input::init()
{
    spdlog::debug("input: taking over window input events.");
    glfwSetCursorPosCallback(client_globals::window, &onCursorMove);
    glfwSetKeyCallback(client_globals::window, &onKeyboardKey);
    glfwSetMouseButtonCallback(client_globals::window, &onMouseButton);
    glfwSetScrollCallback(client_globals::window, &onMouseScroll);
}
