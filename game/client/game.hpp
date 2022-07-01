/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 21:55:07.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

namespace client_game
{
void init();
void initLate();
void deinit();
void update();
void updateFixed();
void updateLate();
void render();
} // namespace client_game
