/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 22:06:54.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <GLFW/glfw3.h>
#include <game/client/events/keyboard_key.hpp>
#include <game/client/game.hpp>
#include <game/client/globals.hpp>
#include <spdlog/spdlog.h>

static void onKeyboardKey(const events::KeyboardKey &event)
{
    if(event.act == GLFW_PRESS && event.key == GLFW_KEY_ESCAPE) {
        spdlog::info("ESC has been pressed! Exiting...");
        glfwSetWindowShouldClose(globals::window, GLFW_TRUE);
    }
}

void client_game::initialize()
{
    globals::dispatcher.sink<events::KeyboardKey>().connect<&onKeyboardKey>();
}

void client_game::shutdown()
{
}

void client_game::update()
{
}

void client_game::lateUpdate()
{
}

void client_game::render()
{
}
