/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Tue Jun 28 2022 14:28:51.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef A6D058AA_CEC1_40C3_A5BD_5EDAAA41D7BC
#define A6D058AA_CEC1_40C3_A5BD_5EDAAA41D7BC
#include <filesystem>
#include <limits>
#include <physfs.h>
#include <vector>

namespace fs_std = std::filesystem;
namespace vfs
{
using file_t = PHYSFS_File;
using openmode_t = unsigned short;

constexpr static const openmode_t OPEN_RD = openmode_t(1 << 0);
constexpr static const openmode_t OPEN_WR = openmode_t(1 << 1);
constexpr static const openmode_t OPEN_AP = openmode_t(1 << 2);

constexpr static const size_t NPOS = std::numeric_limits<size_t>::max();

bool init(const std::string &argv_0);
bool deinit();

bool setWritePath(const std::filesystem::path &path);
bool createDirectories(const std::filesystem::path &path);
bool mount(const std::filesystem::path &path, const std::filesystem::path &mountpoint, bool append);
bool umount(const std::filesystem::path &path);

bool exists(const std::filesystem::path &path);
bool isDirectory(const std::filesystem::path &path);
bool isSymlink(const std::filesystem::path &path);
bool isPathValid(const std::filesystem::path &path);

void close(file_t *file);
file_t *open(const std::filesystem::path &path, openmode_t mode);
size_t getLength(file_t *file);
size_t getPosition(file_t *file);
size_t read(file_t *file, void *buffer, size_t size);
size_t write(file_t *file, const void *buffer, size_t size);
bool readLine(file_t *file, std::string &out);
bool setPosition(file_t *file, size_t position);

const std::filesystem::path getWritePath();
const std::filesystem::path getRootPath();
const std::string getError();

bool readBytes(const std::filesystem::path &path, std::vector<uint8_t> &out);
bool readString(const std::filesystem::path &path, std::string &out);
bool writeBytes(const std::filesystem::path &path, const std::vector<uint8_t> &in);
bool writeString(const std::filesystem::path &path, const std::string &in);
} // namespace vfs


#endif /* A6D058AA_CEC1_40C3_A5BD_5EDAAA41D7BC */
