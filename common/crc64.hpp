/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Tue Jun 28 2022 01:06:09.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef E8A1ABB7_62A8_4C83_A5A0_12E19057E75F
#define E8A1ABB7_62A8_4C83_A5A0_12E19057E75F
#include <cstddef>
#include <string>

namespace crc64
{
std::size_t get(const void *s, std::size_t n);
std::size_t get(const std::string &s);
} // namespace crc64


#endif /* E8A1ABB7_62A8_4C83_A5A0_12E19057E75F */
