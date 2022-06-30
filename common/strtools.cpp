/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 19:37:49.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <common/strtools.hpp>

const std::vector<std::string> strtools::split(const std::string &str, const std::string &sep)
{
    size_t cpos = 0;
    size_t lpos = 0;
    std::vector<std::string> vec;

    while((cpos = str.find(sep, lpos)) != std::string::npos) {
        vec.push_back(str.substr(lpos, cpos - lpos));
        lpos = cpos + sep.length();
    }

    if(lpos <= str.length()) {
        // We lack the last element
        vec.push_back(str.substr(lpos, str.length() - lpos));
    }

    return vec;
}
