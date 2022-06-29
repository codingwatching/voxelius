/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 00:43:41.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <common/cmdline.hpp>
#include <unordered_map>

// Checks if argv[n] is a valid command line option.
// A valid command options starts with an undefined
// amount of hyphens and never ends with one.
static inline const bool isOptionArgv(const std::string &argv)
{
    if(argv.find_last_of('-') >= argv.size() - 1)
        return false;
    return argv[0] == '-';
}

// Cuts all the trailing hyphens from the argv[n].
static inline const std::string getOptionFromArgv(const std::string &argv)
{
    size_t i;
    for(i = 0; argv[i] == '-'; i++)
        ;
    return std::string(argv.cbegin() + i, argv.cend());
}

// {"key":"value"} with no option prefixes.
static std::unordered_map<std::string, std::string> options;

void cmdline::clear()
{
    options.clear();
}

void cmdline::append(const std::string &opt)
{
    options[opt] = std::string();
}

void cmdline::append(const std::string &opt, const std::string &arg)
{
    options[opt] = arg;
}

void cmdline::append(int argc, char **argv)
{
    for(int i = 1; i < argc; i++) {
        if(isOptionArgv(argv[i])) {
            std::string opt = getOptionFromArgv(argv[i]);
            if((i + 1 < argc) && !isOptionArgv(argv[i + 1])) {
                cmdline::append(opt, argv[++i]);
                continue;
            }

            cmdline::append(opt);
        }
    }
}

bool cmdline::exists(const std::string &opt)
{
    return (options.find(opt) != options.cend());
}

bool cmdline::get(const std::string &opt, std::string &out_arg)
{
    const auto it = options.find(opt);
    if(it == options.cend())
        return false;
    out_arg.assign(it->second);
    return true;
}
