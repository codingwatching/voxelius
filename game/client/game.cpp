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
#include <game/client/events/screen_size.hpp>
#include <game/client/game.hpp>
#include <game/client/globals.hpp>
#include <game/client/input.hpp>
#include <game/client/model_interp.hpp>
#include <game/client/model_set.hpp>
#include <game/client/screen.hpp>
#include <spdlog/spdlog.h>

static void onKeyboardKey(const events::KeyboardKey &event)
{
    if(event.act == GLFW_PRESS && event.key == GLFW_KEY_ESCAPE) {
        spdlog::info("escape has been pressed! exiting...");
        glfwSetWindowShouldClose(client_globals::window, GLFW_TRUE);
    }
}

static void onScreenSize(const events::ScreenSize &event)
{
    spdlog::info("new screen size: {}x{}", event.width, event.height);
}

void client_game::init()
{
    input::init();
    screen::init();

    client_globals::dispatcher.sink<events::KeyboardKey>().connect<&onKeyboardKey>();
    client_globals::dispatcher.sink<events::ScreenSize>().connect<&onScreenSize>();
}

void client_game::initLate()
{
    screen::initLate();

}

void client_game::deinit()
{

}

void client_game::update()
{
    model_interp::update();
    model_set::update();
}

void client_game::updateFixed()
{

}

void client_game::updateLate()
{

}

void client_game::render()
{

}
