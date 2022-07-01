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
bool deinit();

bool setWritePath(const rpath_t &path);
bool createDirectories(const vpath_t &path);
bool mount(const rpath_t &path, const vpath_t &mountpoint, bool append);
bool umount(const rpath_t &path);

bool exists(const vpath_t &path);
bool isDirectory(const vpath_t &path);
bool isSymlink(const vpath_t &path);
bool isPathValid(const vpath_t &path);

void close(file_t *file);
file_t *open(const vpath_t &path, openmode_t mode);
size_t getLength(file_t *file);
size_t getPosition(file_t *file);
size_t read(file_t *file, void *buffer, size_t size);
size_t write(file_t *file, const void *buffer, size_t size);
bool readLine(file_t *file, std::string &out);
bool setPosition(file_t *file, size_t position);

const rpath_t getWritePath();
const vpath_t getRootPath();
const std::string getError();

bool readBytes(const vpath_t &path, std::vector<uint8_t> &out);
bool readString(const vpath_t &path, std::string &out);
bool writeBytes(const vpath_t &path, const std::vector<uint8_t> &in);
bool writeString(const vpath_t &path, const std::string &in);
} // namespace vfs
