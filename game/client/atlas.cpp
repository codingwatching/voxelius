/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Sat Jul 02 2022 21:33:20.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <common/cxpr.hpp>
#include <game/client/atlas.hpp>
#include <game/client/image.hpp>
#include <spdlog/spdlog.h>

Atlas::Atlas()
    : width(0), height(0), count(0), last_index(0)
{

}

bool Atlas::create(int width, int height, int count)
{
    destroy();

    this->width = width;
    this->height = height;
    this->count = count;
    this->last_index = 0;

    texture.create();
    if(!texture.storage(width, height, count, Image::FORMAT)) {
        spdlog::error("atlas: unable to allocate texture array");
        destroy();
        return false;
    }

    return true;
}

void Atlas::destroy()
{
    width = 0;
    height = 0;
    count = 0;
    last_index = std::numeric_limits<GLuint>::max();
    texture.destroy();
}

void Atlas::submit()
{
    // FIXME: should we generate a mipmap for
    // the atlas considering we have no filtering?
    // texture.genMipmap();
}

const Atlas::Entry *Atlas::emplace(const std::filesystem::path &path)
{
    if(!texture.valid()) {
        spdlog::warn("atlas: unable to emplace {}: texture array is invalid", path.string());
        return nullptr;
    }

    const auto it = entries.find(path);
    if(it != entries.cend()) {
        // FIXME: it FLOODS the logs
        // spdlog::warn("atlas: unable to emplace {}: already present", path.string());
        return &it->second;
    }

    if(last_index >= count) {
        spdlog::warn("atlas: unable to emplace {}: count exceeded [{}, max {}]", path.string(), last_index, count);
        return nullptr;
    }

    if(const Image image = Image(path); image.valid()) {
        int iw, ih;
        image.getSize(iw, ih);

        // We allow SMALLER images but not
        // LARGER images. Having an image that
        // has dimensions larger than the array
        // can provide will require me setting up
        // an environment for downsampling it which
        // is much harder than just 

        if(iw > width || ih > height) {
            // I hereby consider this a hate crime.
            spdlog::error("atlas: {} exceeds atlas size [{}x{}, max {}x{}]", path.string(), iw, ih, width, height);
            return nullptr;
        }

        Entry entry = {};
        entry.index = last_index++;
        entry.uv.x = static_cast<double>(iw) / static_cast<double>(width);
        entry.uv.y = static_cast<double>(ih) / static_cast<double>(height);

        texture.write(static_cast<int>(entry.index), 0, 0, iw, ih, Image::FORMAT, image.data());

        return &(entries[path] = entry);
    }

    spdlog::warn("atlas: unable to emplace {}: {}", path.string(), vfs::getError());
    return nullptr;
}

const Atlas::Entry *Atlas::find(const std::filesystem::path &path) const
{
    const auto it = entries.find(path);
    if(it != entries.cend())
        return &it->second;
    return nullptr;
}

const glxx::Texture2DArray &Atlas::get() const
{
    return texture;
}
