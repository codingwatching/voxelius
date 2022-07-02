/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Sat Jul 02 2022 18:53:29.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <game/client/comp/interp_transform.hpp>
#include <game/client/comp/model_matrix.hpp>
#include <game/client/model_set.hpp>
#include <game/shared/comp/body.hpp>
#include <game/shared/globals.hpp>
#include <glm/gtc/type_ptr.hpp>

void model_set::update()
{
    const auto group = shared_globals::registry.group<comp::ModelMatrix>(entt::get<comp::Body>);
    for(const auto [entity, model, body] : group.each()) {
        const reactphysics3d::Transform *tr = nullptr;
        if(const auto *interp = shared_globals::registry.try_get<comp::InterpTransform>(entity))
            tr = &interp->interp;
        else
            tr = &body.body->getTransform();
        tr->getOpenGLMatrix(glm::value_ptr(model.matrix));
    }
}
