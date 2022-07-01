/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Fri Jul 01 2022 20:01:05.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <game/client/comp/interp.hpp>
#include <game/client/comp/model.hpp>
#include <game/client/globals.hpp>
#include <game/client/model_set.hpp>
#include <game/shared/comp/body.hpp>
#include <glm/gtc/type_ptr.hpp>

void model_set::update()
{
    const auto group = client_globals::registry.group(entt::get<comp::Body, comp::Model>, entt::exclude<comp::Interp>);
    for(const auto [entity, body, model] : group.each()) {
        // This results in a somewhat jittery look.
        body.body->getTransform().getOpenGLMatrix(glm::value_ptr(model.matrix));
    }
}
