/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Tue Jun 28 2022 00:42:40.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef C38A4868_BC2E_4EA4_B3E2_96789EF50921
#define C38A4868_BC2E_4EA4_B3E2_96789EF50921
#include <string>

namespace cmdline
{
void clear();
void append(const std::string &opt);
void append(const std::string &opt, const std::string &arg);
void append(int argc, char **argv);
bool exists(const std::string &opt);
bool get(const std::string &opt, std::string &out_arg);
} // namespace cmdline


#endif /* C38A4868_BC2E_4EA4_B3E2_96789EF50921 */
