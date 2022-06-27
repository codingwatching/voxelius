/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 00:11:52.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <common/vfs.hpp>
#include <deque>
#include <iterator>

static fs_std::path rw_root = fs_std::current_path();
static fs_std::path ro_root = fs_std::current_path();
static std::deque<fs_std::path> ro_paths;

const fs_std::path vfs::getPathRd(const fs_std::path &vpath, vfs::ioflags_t flags)
{
    const fs_std::path rpath_rw = rw_root / vpath;

    if((flags & vfs::IO_FAV_RW) && fs_std::exists(rpath_rw)) {
        // IO_FAV_RW means that we prefer reading
        // from the read-write path rather than from
        // read-only path. This can be useful when we
        // read configuration files - user-defined
        // values should be over everything that is in
        // content/resourcepack subdirectories.
        return rpath_rw;
    }

    // Check read-only paths.
    for(const fs_std::path &it : ro_paths) {
        const fs_std::path rpath_ro = ro_root / it / vpath;
        if(!fs_std::exists(rpath_ro))
            continue;
        return rpath_ro;
    }

    // Use the read-write path.
    return rpath_rw;
}

const fs_std::path vfs::getPathWr(const fs_std::path &vpath, vfs::ioflags_t flags)
{
    return rw_root / vpath;
}

void vfs::addPathRO_A(const fs_std::path &rpath)
{
    // Make sure we don't have repeating
    // search paths somewhere in the middle
    // of the path list.
    for(auto it = ro_paths.cbegin(); it != ro_paths.cend(); it++) {
        if((*it) == rpath) {
            it = ro_paths.erase(it) - 1;
            continue;
        }
    }

    // Append.
    ro_paths.push_back(rpath);
}

void vfs::addPathRO_P(const fs_std::path &rpath)
{
    // Make sure we don't have repeating
    // search paths somewhere in the middle
    // of the path list.
    for(auto it = ro_paths.cbegin(); it != ro_paths.cend(); it++) {
        if((*it) == rpath) {
            it = ro_paths.erase(it) - 1;
            continue;
        }
    }

    // Prepend.
    ro_paths.push_front(rpath);
}

bool vfs::setRootRO(const fs_std::path &new_root)
{
    if(!fs_std::create_directories(new_root) && !fs_std::exists(new_root))
        return false;
    if(!fs_std::is_directory(new_root))
        return false;
    ro_root = new_root;
    return true;
}

bool vfs::setRootRW(const fs_std::path &new_root)
{
    if(!fs_std::create_directories(new_root) && !fs_std::exists(new_root))
        return false;
    if(!fs_std::is_directory(new_root))
        return false;
    rw_root = new_root;
    return true;
}

void vfs::resetAllPaths()
{
    rw_root = fs_std::current_path();
    ro_root = fs_std::current_path();
    ro_paths.clear();
}

std::ifstream vfs::openRd(const fs_std::path &vpath, vfs::ioflags_t flags)
{
    std::ios_base::openmode mode = std::ios::in;
    if(flags & vfs::IO_BINARY)
        mode |= std::ios::binary;
    std::ifstream ifile = std::ifstream(vfs::getPathRd(vpath, flags), mode);
    if(flags & vfs::IO_BINARY)
        ifile.unsetf(std::ios::skipws);
    return std::move(ifile);
}

std::ofstream vfs::openWr(const fs_std::path &vpath, vfs::ioflags_t flags)
{
    std::ios_base::openmode mode = std::ios::out;
    if(flags & vfs::IO_APPEND)
        mode = std::ios::app;
    if(flags & vfs::IO_BINARY)
        mode |= std::ios::binary;
    return std::move(std::ofstream(vfs::getPathWr(vpath, flags), mode));
}

bool vfs::readBinary(std::ifstream &ifile, std::vector<unsigned char> &out)
{
    if(!ifile.is_open())
        return false;

    // Store position
    std::streampos oldpos = ifile.tellg();

    ifile.seekg(0, std::ios::end);
    out.resize(static_cast<size_t>(ifile.tellg()));
    ifile.seekg(0, std::ios::beg);

    out.insert(out.begin(), std::istream_iterator<char>(ifile), std::istream_iterator<char>());

    // Restore position
    ifile.seekg(oldpos, std::ios::beg);

    return true;
}

bool vfs::readString(std::ifstream &ifile, std::string &out)
{
    if(!ifile.is_open())
        return false;

    // Store position
    std::streampos oldpos = ifile.tellg();

    ifile.seekg(0, std::ios::end);
    out.resize(static_cast<size_t>(ifile.tellg()));
    ifile.seekg(0, std::ios::beg);

    out.assign(std::istreambuf_iterator<char>(ifile), std::istreambuf_iterator<char>());

    // Restore position
    ifile.seekg(oldpos, std::ios::beg);

    return true;
}

bool vfs::writeBinary(std::ofstream &ofile, const std::vector<unsigned char> &in)
{
    if(!ofile.is_open())
        return false;
    ofile.write(reinterpret_cast<const char *>(in.data()), in.size());
    return true;
}

bool vfs::writeString(std::ofstream &ofile, const std::string &in)
{
    if(!ofile.is_open())
        return false;
    ofile << in << std::endl;
    return true;
}
