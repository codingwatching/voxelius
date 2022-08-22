/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Tue Jun 28 2022 14:39:46.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <common/strtools.hpp>
#include <common/vfs.hpp>
#include <spdlog/spdlog.h>

// Checks if an input string is a valid PhysFS path.
// A valid PhysFS path shouldn't contain unsafe entries
// such as Windows/MacOS path separators and directory
// changing bits ("." and "..", however "..." should be fine).
static inline const bool checkPathBit(const std::string &tok)
{
    if(tok == "." || tok == "..")
        return false;
    if(tok.find('\\') != std::string::npos)
        return false;
    if(tok.find(':') != std::string::npos)
        return false;
    return true;
}

bool vfs::init(const std::string &argv_0)
{
    return !!PHYSFS_init(argv_0.c_str());
}

bool vfs::deinit()
{
    return !!PHYSFS_deinit();
}

bool vfs::setWritePath(const std::filesystem::path &path)
{
    return !!PHYSFS_setWriteDir(path.native().c_str());
}

bool vfs::createDirectories(const std::filesystem::path &path)
{
    return !!PHYSFS_mkdir(path.string().c_str());
}

bool vfs::mount(const std::filesystem::path &path, const std::filesystem::path &mountpoint, bool append)
{
    return !!PHYSFS_mount(path.native().c_str(), mountpoint.string().c_str(), append ? 1 : 0);
}

bool vfs::umount(const std::filesystem::path &path)
{
    return !!PHYSFS_unmount(path.native().c_str());
}

bool vfs::exists(const std::filesystem::path &path)
{
    return PHYSFS_exists(path.string().c_str()) ? true : false;
}

bool vfs::isDirectory(const std::filesystem::path &path)
{
    PHYSFS_Stat s = {};
    const std::string vstr = path.string();
    if(PHYSFS_stat(vstr.c_str(), &s))
        return s.filetype == PHYSFS_FILETYPE_DIRECTORY;
    return false;
}

bool vfs::isSymlink(const std::filesystem::path &path)
{
    PHYSFS_Stat s = {};
    const std::string vstr = path.string();
    if(PHYSFS_stat(vstr.c_str(), &s))
        return s.filetype == PHYSFS_FILETYPE_SYMLINK;
    return false;
}

bool vfs::isPathValid(const std::filesystem::path &path)
{
    if(path.is_absolute() && path.root_path() == vfs::getRootPath()) {
        std::vector<std::string> bits = strtools::split(path.string(), std::string(std::filesystem::path::preferred_separator, 1));
        for(const std::string &bit : bits) {
            if(checkPathBit(bit))
                continue;
            return false;
        }

        return true;
    }

    return false;
}

void vfs::close(vfs::file_t *file)
{
    PHYSFS_close(file);
}

vfs::file_t *vfs::open(const std::filesystem::path &path, vfs::openmode_t mode)
{
    const std::string vstr = path.string();
    vfs::file_t *file = nullptr;

    do {
        // MODE: READ
        if(mode & vfs::OPEN_RD) {
            file = PHYSFS_openRead(vstr.c_str());
            break;
        }

        // MODE: APPEND WRITE
        if((mode & vfs::OPEN_WR) && (mode & vfs::OPEN_AP)) {
            file = PHYSFS_openAppend(vstr.c_str());
            break;
        }

        // MODE: OVERWRITE
        if(mode & vfs::OPEN_WR) {
            file = PHYSFS_openWrite(vstr.c_str());
            break;
        }

        spdlog::error("vfs: open [{}, {}] failed: invalid mode", vstr, mode);
        return nullptr;
    } while(false);

    return file;
}

size_t vfs::getLength(vfs::file_t *file)
{
    return file ? static_cast<size_t>(PHYSFS_fileLength(file)) : 0;
}

size_t vfs::getPosition(vfs::file_t *file)
{
    return file ? static_cast<size_t>(PHYSFS_tell(file)) : vfs::NPOS;
}

size_t vfs::read(vfs::file_t *file, void *buffer, size_t size)
{
    if(file)
        return static_cast<size_t>(PHYSFS_readBytes(file, buffer, static_cast<PHYSFS_uint64>(size)));
    return 0;
}

size_t vfs::write(vfs::file_t *file, const void *buffer, size_t size)
{
    if(file)
        return static_cast<size_t>(PHYSFS_writeBytes(file, buffer, static_cast<PHYSFS_uint64>(size)));
    return 0;
}

bool vfs::readLine(file_t *file, std::string &out)
{
    char tmp[2] = { 0 };
    if(file && !PHYSFS_eof(file)) {
        out.clear();
        while(PHYSFS_readBytes(file, tmp, 1) == 1) {
            tmp[1] = 0x00; // make sure it's terminated.
            if(tmp[0] == '\n')
                break;
            out.append(tmp);
        }

        return true;
    }

    return false;
}

bool vfs::setPosition(vfs::file_t *file, size_t position)
{
    return !!PHYSFS_seek(file, static_cast<PHYSFS_uint64>(position));
}

const std::filesystem::path vfs::getWritePath()
{
    return std::filesystem::path(PHYSFS_getWriteDir());
}

const std::filesystem::path vfs::getRootPath()
{
    return std::filesystem::path("/");
}

const std::string vfs::getError()
{
    return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
}

bool vfs::readBytes(const std::filesystem::path &path, std::vector<uint8_t> &out)
{
    vfs::file_t *file = vfs::open(path, vfs::OPEN_RD);
    if(file) {
        out.resize(vfs::getLength(file));
        vfs::read(file, out.data(), out.size());
        vfs::close(file);
        return true;
    }

    return false;
}

bool vfs::readString(const std::filesystem::path &path, std::string &out)
{
    vfs::file_t *file = vfs::open(path, vfs::OPEN_RD);
    if(file) {
        out.resize(vfs::getLength(file));
        vfs::read(file, out.data(), out.size());
        vfs::close(file);
        return true;
    }

    return false;
}

bool vfs::writeBytes(const std::filesystem::path &path, const std::vector<uint8_t> &in)
{
    vfs::file_t *file = vfs::open(path, vfs::OPEN_WR);
    if(file) {
        vfs::write(file, in.data(), in.size());
        vfs::close(file);
        return true;
    }

    return false;
}

bool vfs::writeString(const std::filesystem::path &path, const std::string &in)
{
    vfs::file_t *file = vfs::open(path, vfs::OPEN_WR);
    if(file) {
        vfs::write(file, in.data(), in.size());
        vfs::write(file, "\n", 1);
        vfs::close(file);
        return true;
    }

    return false;
}
