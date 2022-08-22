/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Fri Jul 01 2022 23:41:09.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <game/client/image.hpp>
#include <stb_image.h>

Image::Image()
    : width(0), height(0), pixels(nullptr)
{

}

Image::Image(const std::filesystem::path &path, bool flip)
    : width(0), height(0), pixels(nullptr)
{
    create(path, flip);
}

Image::Image(Image &&rhs)
    : width(rhs.width), height(rhs.height), pixels(rhs.pixels)
{
    rhs.width = 0;
    rhs.height = 0;
    rhs.pixels = nullptr;
}

Image::~Image()
{
    destroy();
}

Image &Image::operator=(Image &&rhs)
{
    Image copy(std::move(rhs));
    std::swap(copy.width, width);
    std::swap(copy.height, height);
    std::swap(copy.pixels, pixels);
    return *this;
}

bool Image::create(const std::filesystem::path &path, bool flip)
{
    stbi_set_flip_vertically_on_load(flip ? 1 : 0);

    destroy();

    std::vector<uint8_t> buffer;
    if(vfs::readBytes(path, buffer)) {
        pixels = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(buffer.data()), static_cast<int>(buffer.size()), &width, &height, nullptr, STBI_rgb_alpha);
        return width && height && pixels;
    }

    return false;
}

void Image::destroy()
{
    if(pixels) {
        stbi_image_free(pixels);
        pixels = nullptr;
    }
}

bool Image::valid() const
{
    return width && height && pixels;
}

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}

void Image::getSize(int &width, int &height) const
{
    width = this->width;
    height = this->height;
}

const void *Image::data() const
{
    return pixels;
}

void *Image::data()
{
    return pixels;
}
