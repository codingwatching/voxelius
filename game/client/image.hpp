/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Fri Jul 01 2022 23:38:11.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef A86E4BD8_41CF_4E53_A6E9_2D5BEF32E3A6
#define A86E4BD8_41CF_4E53_A6E9_2D5BEF32E3A6
#include <common/mixin.hpp>
#include <common/vfs.hpp>
#include <game/client/glxx/pixel_format.hpp>

class Image final : public mixin::NonCopyable {
public:
    constexpr static const glxx::PixelFormat FORMAT = glxx::PixelFormat::R8G8B8A8_UNORM;

public:
    Image();
    Image(const std::filesystem::path &path, bool flip = true);
    Image(Image &&rhs);
    virtual ~Image();
    
    Image &operator=(Image &&rhs);

    bool create(const std::filesystem::path &path, bool flip = true);
    void destroy();
    bool valid() const;

    int getWidth() const;
    int getHeight() const;
    void getSize(int &width, int &height) const;

    const void *data() const;
    void *data();

private:
    int width, height;
    void *pixels;
};


#endif /* A86E4BD8_41CF_4E53_A6E9_2D5BEF32E3A6 */
