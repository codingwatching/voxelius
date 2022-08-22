/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Fri Jul 01 2022 00:49:40.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef B6F11D35_63A2_4D8C_8105_5E99372768F8
#define B6F11D35_63A2_4D8C_8105_5E99372768F8
#include <common/vfs.hpp>

namespace fstools
{
bool include(const std::filesystem::path &path, std::string &out, const std::string &comment = "//");
} // namespace fstools

#endif /* B6F11D35_63A2_4D8C_8105_5E99372768F8 */
