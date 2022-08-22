/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Mon Aug 22 2022 20:14:48.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <game/client/image.hpp>
#include <game/client/texture_array.hpp>
#include <spdlog/spdlog.h>

constexpr static const GLuint INVALID_HEAD = std::numeric_limits<GLuint>::max();
constexpr static const std::size_t INVALID_SIZE = std::numeric_limits<std::size_t>::max();
constexpr static const std::size_t MAX_SIZE = static_cast<std::size_t>(std::numeric_limits<int>::max());

TextureArray::TextureArray()
    : head(INVALID_HEAD), size(INVALID_SIZE), width(-1), height(-1), texture(), map()
{

}

bool TextureArray::create(int width, int height, std::size_t size)
{
    destroy();

    // make sure we have valid arguments...
    if(width <= 0 || height <= 0 || static_cast<int>(size) < 0 || size >= MAX_SIZE) {
        spdlog::error("texture_array: invalid create({}, {}, {}) call!", width, height, size);
        return false;
    }

    this->head = 0;
    this->size = size;
    this->width = width;
    this->height = height;

    texture.create();
    if(!texture.storage(width, height, static_cast<int>(size), Image::FORMAT)) {
        spdlog::error("texture_array: allocation failed.");
        destroy();
        return false;
    }

    return true;
}

void TextureArray::destroy()
{
    head = INVALID_HEAD;
    size = INVALID_SIZE;
    width = -1;
    height = -1;
    texture.destroy();
    map.clear();
}

void TextureArray::submit()
{
    if(!texture.valid()) {
        spdlog::warn("texture_array: submit failed: texture is not valid");
        return;
    }

    texture.genMipmap();
}

const ArrayTexture *TextureArray::emplace(const std::filesystem::path &path)
{
    if(!texture.valid()) {
        spdlog::warn("texture_array: emplace {} failed: texture is not valid", path.string());
        return nullptr;
    }

    const auto it = map.find(path);
    if(it != map.cend()) {
        // FIXME: it FLOODS the logs!
        spdlog::trace("texture_array: emplace {} possibly failed: entry is already present", path.string());
        return &it->second;
    }

    if(head >= size) {
        spdlog::warn("texture_array: emplace {} failed: array size exceeded [{}, max {}]", head + 1, size);
        return nullptr;
    }

    const Image image = Image(path);
    if(image.valid()) {
        int img_width;
        int img_height;
        image.getSize(img_width, img_height);

        // We don't allow bigger images as of now.
        // This is because it requires a lot of
        // tedious work setting up downsampling
        // environment for potentially non-existent scenario.
        if(img_width > width || img_height > height) {
            spdlog::warn("texture_array: emplace {} failed: size {}x{} exceeds {}x{}", path.string(), img_width, img_height, width, height);
            return nullptr;
        }

        ArrayTexture entry = {};
        entry.index = head++;
        entry.uv.x = static_cast<double>(img_width) / static_cast<double>(width);
        entry.uv.y = static_cast<double>(img_height) / static_cast<double>(height);

        texture.write(static_cast<int>(entry.index), 0, 0, img_width, img_height, Image::FORMAT, image.data());

        return &(map[path] = entry);
    }

    spdlog::warn("texture_array: emplace {} failed: {}", path.string(), vfs::getError());
    return nullptr;
}

const ArrayTexture *TextureArray::find(const std::filesystem::path &path) const
{
    const auto it = map.find(path);
    if(it != map.cend())
        return &it->second;
    return nullptr;
}

const glxx::Texture2DArray &TextureArray::get() const
{
    return texture;
}
