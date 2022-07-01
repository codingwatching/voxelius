/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Fri Jul 01 2022 19:54:02.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <game/client/comp/interp.hpp>
#include <game/client/comp/model_matrix.hpp>
#include <game/client/globals.hpp>
#include <game/client/model_interp.hpp>
#include <game/shared/comp/body.hpp>
#include <glm/gtc/type_ptr.hpp>

void model_interp::update()
{
    const auto group = client_globals::registry.group<comp::Interp>(entt::get<comp::Body, comp::ModelMatrix>);
    for(const auto [entity, interp, body, model] : group.each()) {
        const reactphysics3d::Transform &bt = body.body->getTransform();
        const reactphysics3d::Transform nt = bt.interpolateTransforms(interp.transform, bt, client_globals::interpfactor);
        nt.getOpenGLMatrix(glm::value_ptr(model.matrix));
    }
}
