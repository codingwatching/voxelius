/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Mon Jun 27 2022 23:55:25.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs_std = std::filesystem;
namespace vfs
{
using ioflags_t = unsigned short;
constexpr static const ioflags_t IO_BINARY = ioflags_t(1 << 0);
constexpr static const ioflags_t IO_APPEND = ioflags_t(1 << 1);
constexpr static const ioflags_t IO_FAV_RW = ioflags_t(1 << 2);

// Converting virtual paths to physical.
// NOTE: These tend to check for file existence.
const fs_std::path getPathRd(const fs_std::path &vpath, ioflags_t flags = 0);
const fs_std::path getPathWr(const fs_std::path &vpath, ioflags_t flags = 0);

// Mounting new search paths to the vfs.
void addPathRO_A(const fs_std::path &rpath);
void addPathRO_P(const fs_std::path &rpath);
bool setRootRW(const fs_std::path &rpath);
bool setRootRO(const fs_std::path &rpath);
void resetAllPaths();

// Opening streams
std::ifstream openRd(const fs_std::path &vpath, ioflags_t flags = 0);
std::ofstream openWr(const fs_std::path &vpath, ioflags_t flags = 0);

// Utility functions
bool readBinary(std::ifstream &ifile, std::vector<unsigned char> &out);
bool readString(std::ifstream &ifile, std::string &out);
bool writeBinary(std::ofstream &ofile, const std::vector<unsigned char> &in);
bool writeString(std::ofstream &ofile, const std::string &in);
} // namespace vfs
