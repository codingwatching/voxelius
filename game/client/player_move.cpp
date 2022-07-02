/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Sat Jul 02 2022 17:56:33.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <GLFW/glfw3.h>
#include <game/client/events/keyboard_key.hpp>
#include <game/client/globals.hpp>
#include <game/client/player_move.hpp>
#include <game/shared/comp/body.hpp>
#include <game/shared/convert.hpp>
#include <game/shared/globals.hpp>

using move_keys_t = unsigned short;
constexpr static const move_keys_t MV_FORWARD   = move_keys_t(1 << 0);
constexpr static const move_keys_t MV_BACK      = move_keys_t(1 << 1);
constexpr static const move_keys_t MV_LEFT      = move_keys_t(1 << 2);
constexpr static const move_keys_t MV_RIGHT     = move_keys_t(1 << 3);

static bool mv_updated = false;
static move_keys_t mv_keys = 0;

static void onKeyboardKey(const events::KeyboardKey &event)
{
    move_keys_t mask = 0;
    switch(event.key) {
        case GLFW_KEY_W:
            mask = MV_FORWARD;
            break;
        case GLFW_KEY_S:
            mask = MV_BACK;
            break;
        case GLFW_KEY_A:
            mask = MV_LEFT;
            break;
        case GLFW_KEY_D:
            mask = MV_RIGHT;
            break;
        default:
            return;
    }

    switch(event.act) {
        case GLFW_PRESS:
            mv_updated = true;
            mv_keys |= mask;
            break;
        case GLFW_RELEASE:
            mv_updated = true;
            mv_keys &= ~mask;
            break;
    }
}

void player_move::init()
{
    mv_updated = false;
    mv_keys = 0;
    shared_globals::dispatcher.sink<events::KeyboardKey>().connect<&onKeyboardKey>();
}

void player_move::update()
{
    if(mv_updated && client_globals::local_player != entt::null) {
        comp::Body &body = shared_globals::registry.get<comp::Body>(client_globals::local_player);
        const reactphysics3d::Transform &bt = body.body->getTransform();
        const glm::dquat orient = convert::toGLM(bt.getOrientation());

        glm::dvec3 wishdir = glm::dvec3(0.0, 0.0, 0.0);
        if(mv_keys & MV_FORWARD)
            wishdir += DIR_FORWARD;
        if(mv_keys & MV_BACK)
            wishdir -= DIR_FORWARD;
        if(mv_keys & MV_LEFT)
            wishdir -= DIR_RIGHT;
        if(mv_keys & MV_RIGHT)
            wishdir += DIR_RIGHT;
        const glm::dvec3 wishvel = orient * wishdir * 10.0;

        body.body->setLinearVelocity(convert::toRP3D(wishvel));

        mv_updated = false;
    }
}
