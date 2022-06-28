/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 14:39:46.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <common/vfs.hpp>
#include <spdlog/spdlog.h>

namespace detail
{
static inline const std::string lastError()
{
    return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
}

static inline const bool checkStr(const std::string &tok)
{
    if(tok == "." || tok == "..")
        return false;
    if(tok.find('\\') != std::string::npos)
        return false;
    if(tok.find(':') != std::string::npos)
        return false;
    return true;
}
} // namespace detail

bool vfs::init(const std::string &argv_0)
{
    if(PHYSFS_init(argv_0.c_str()))
        return true;
    spdlog::error("vfs: init failed: {}", detail::lastError());
    return false;
}

void vfs::shutdown()
{
    if(PHYSFS_deinit())
        return;
    spdlog::warn("vfs: proper deinit failed: {}", detail::lastError());
}

bool vfs::setwr(const vfs::rpath_t &rpath)
{
    const std::string rstr = rpath.native();
    if(PHYSFS_setWriteDir(rstr.c_str()))
        return true;
    spdlog::error("vfs: setwr [{}] failed: {}", rstr, detail::lastError());
    return false;
}

bool vfs::mount(const vfs::rpath_t &rpath, const vfs::vpath_t &vpath, bool append)
{
    const std::string rstr = rpath.native();
    const std::string vstr = vpath.string();
    if(PHYSFS_mount(rstr.c_str(), vstr.c_str(), append ? 1 : 0))
        return true;
    spdlog::error("vfs: mount [{} -> {}] failed: {}", rstr, vstr, detail::lastError());
    return false;
}

bool vfs::umount(const vfs::rpath_t &rpath)
{
    const std::string rstr = rpath.native();
    if(PHYSFS_unmount(rstr.c_str()))
        return true;
    spdlog::error("vfs: umount [{}] failed: {}", rstr, detail::lastError());
    return false;
}

bool vfs::mkdir(const vfs::vpath_t &vpath)
{
    const std::string vstr = vpath.string();
    if(PHYSFS_mkdir(vstr.c_str()))
        return true;
    spdlog::error("vfs: mkdir [{}] failed: {}", vstr, detail::lastError());
    return false;
}

bool vfs::exists(const vfs::vpath_t &vpath)
{
    return PHYSFS_exists(vpath.string().c_str()) ? true : false;
}

bool vfs::directory(const vfs::vpath_t &vpath)
{
    PHYSFS_Stat s = {};
    const std::string vstr = vpath.string();
    if(PHYSFS_stat(vstr.c_str(), &s))
        return s.filetype == PHYSFS_FILETYPE_DIRECTORY;
    return false;
}

bool vfs::symlink(const vfs::vpath_t &vpath)
{
    PHYSFS_Stat s = {};
    const std::string vstr = vpath.string();
    if(PHYSFS_stat(vstr.c_str(), &s))
        return s.filetype == PHYSFS_FILETYPE_SYMLINK;
    return false;
}

bool vfs::valid(const vfs::vpath_t &vpath)
{
    if(vpath.is_absolute() && vpath.root_path() == vfs::root()) {
        size_t pos;
        std::string tok;
        std::string vstr = vpath.string();

        while((pos = vstr.find(vfs::vpath_t::preferred_separator)) != std::string::npos) {
            tok = vstr.substr(0, pos);
            vstr.erase(0, pos + 1);
            if(detail::checkStr(tok))
                continue;
            return false;
        }

        return detail::checkStr(vstr);
    }
    
    return false;
}

void vfs::close(vfs::file_t *vfile)
{
    PHYSFS_close(vfile);
}

vfs::file_t *vfs::open(const vfs::vpath_t &vpath, vfs::openmode_t mode)
{
    const std::string vstr = vpath.string();
    vfs::file_t *vfile = nullptr;

    do {
        // MODE: READ
        if(mode & vfs::OPEN_RD) {
            vfile = PHYSFS_openRead(vstr.c_str());
            break;
        }

        // MODE: APPEND WRITE
        if((mode & vfs::OPEN_WR) && (mode & vfs::OPEN_AP)) {
            vfile = PHYSFS_openAppend(vstr.c_str());
            break;
        }

        // MODE: OVERWRITE
        if(mode & vfs::OPEN_WR) {
            vfile = PHYSFS_openWrite(vstr.c_str());
            break;
        }

        spdlog::error("vfs: open [{}, {}] failed: invalid mode", vstr, mode);
        return nullptr;
    } while(false);

    if(!vfile) {
        spdlog::error("vfs: open [{}, {}] failed: {}", vstr, mode, detail::lastError());
        return nullptr;
    }

    return vfile;
}

size_t vfs::length(vfs::file_t *vfile)
{
    return vfile ? static_cast<size_t>(PHYSFS_fileLength(vfile)) : 0;
}

size_t vfs::position(vfs::file_t *vfile)
{
    return vfile ? static_cast<size_t>(PHYSFS_tell(vfile)) : vfs::NPOS;
}

size_t vfs::read(vfs::file_t *vfile, void *buffer, size_t size)
{
    if(vfile)
        return static_cast<size_t>(PHYSFS_readBytes(vfile, buffer, static_cast<PHYSFS_uint64>(size)));
    return 0;
}

size_t vfs::write(vfs::file_t *vfile, const void *buffer, size_t size)
{
    if(vfile)
        return static_cast<size_t>(PHYSFS_writeBytes(vfile, buffer, static_cast<PHYSFS_uint64>(size)));
    return 0;
}

bool vfs::readline(file_t *vfile, std::string &out)
{
    if(vfile && !PHYSFS_eof(vfile)) {
        char ch;
        out.clear();
        while(PHYSFS_readBytes(vfile, &ch, sizeof(ch)) == 1) {
            if(ch == '\n')
                break;
            out.append(std::string(ch, 1));
        }

        return true;
    }

    return false;
}

const vfs::rpath_t vfs::wr()
{
    return vfs::rpath_t(PHYSFS_getWriteDir());
}

const vfs::vpath_t vfs::root()
{
    return vfs::vpath_t("/");
}

bool vfs::readBytes(const vfs::vpath_t &vpath, std::vector<uint8_t> &out)
{
    vfs::file_t *vfile = vfs::open(vpath, vfs::OPEN_RD);
    if(vfile) {
        out.resize(vfs::length(vfile));
        vfs::read(vfile, out.data(), out.size());
        vfs::close(vfile);
        return true;
    }

    return false;
}

bool vfs::readString(const vfs::vpath_t &vpath, std::string &out)
{
    vfs::file_t *vfile = vfs::open(vpath, vfs::OPEN_RD);
    if(vfile) {
        out.resize(vfs::length(vfile));
        vfs::read(vfile, out.data(), out.size());
        vfs::close(vfile);
        return true;
    }

    return false;
}

bool vfs::writeBytes(const vfs::vpath_t &vpath, const std::vector<uint8_t> &in)
{
    vfs::file_t *vfile = vfs::open(vpath, vfs::OPEN_WR);
    if(vfile) {
        vfs::write(vfile, in.data(), in.size());
        vfs::close(vfile);
        return true;
    }

    return false;
}

bool vfs::writeString(const vfs::vpath_t &vpath, const std::string &in)
{
    vfs::file_t *vfile = vfs::open(vpath, vfs::OPEN_WR);
    if(vfile) {
        vfs::write(vfile, in.data(), in.size());
        vfs::write(vfile, "\n", 1);
        vfs::close(vfile);
        return true;
    }

    return false;
}
