/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 14:28:51.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <filesystem>
#include <limits>
#include <physfs.h>
#include <vector>

namespace fs_std = std::filesystem;
namespace vfs
{
using file_t = PHYSFS_File;
using rpath_t = fs_std::path;
using vpath_t = fs_std::path;
using openmode_t = unsigned short;

constexpr static const openmode_t OPEN_RD = openmode_t(1 << 0);
constexpr static const openmode_t OPEN_WR = openmode_t(1 << 1);
constexpr static const openmode_t OPEN_AP = openmode_t(1 << 2);

constexpr static const size_t NPOS = std::numeric_limits<size_t>::max();

bool init(const std::string &argv_0);
void shutdown();

bool setwr(const rpath_t &rpath);
bool mount(const rpath_t &rpath, const vpath_t &vpath, bool append);
bool umount(const rpath_t &rpath);
bool mkdir(const vpath_t &vpath);

bool exists(const vpath_t &vpath);
bool directory(const vpath_t &vpath);
bool symlink(const vpath_t &vpath);
bool valid(const vpath_t &vpath);

void close(file_t *vfile);
file_t *open(const vpath_t &vpath, openmode_t mode);
size_t length(file_t *vfile);
size_t position(file_t *vfile);
size_t read(file_t *vfile, void *buffer, size_t size);
size_t write(file_t *vfile, const void *buffer, size_t size);
bool readline(file_t *vfile, std::string &out);

const rpath_t wr();
const vpath_t root();

bool readBytes(const vpath_t &vpath, std::vector<uint8_t> &out);
bool readString(const vpath_t &vpath, std::string &out);
bool writeBytes(const vpath_t &vpath, const std::vector<uint8_t> &in);
bool writeString(const vpath_t &vpath, const std::string &in);
} // namespace vfs
