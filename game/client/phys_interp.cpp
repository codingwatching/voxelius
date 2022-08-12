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
#include <game/client/comp/interp_transform.hpp>
#include <game/client/globals.hpp>
#include <game/client/phys_interp.hpp>
#include <game/shared/comp/body.hpp>
#include <game/shared/globals.hpp>

void phys_interp::update()
{
    const double factor = globals::phys_interpfactor;
    const auto group = globals::registry.group<comp::InterpTransform>(entt::get<comp::Body>);
    for(const auto [entity, interp, body] : group.each()) {
        const reactphysics3d::Transform &bt = body.body->getTransform();
        const reactphysics3d::Transform &pt = interp.prev;
        interp.interp = reactphysics3d::Transform::interpolateTransforms(pt, bt, factor);
        interp.prev = bt;
    }
}
