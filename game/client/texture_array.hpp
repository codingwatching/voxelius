/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Mon Aug 22 2022 17:20:33.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef F0779A19_ADAB_46E3_88D5_3FBC1E988025
#define F0779A19_ADAB_46E3_88D5_3FBC1E988025
#include <cstddef>
#include <filesystem>
#include <game/client/glxx/texture.hpp>
#include <glm/vec2.hpp>
#include <unordered_map>

struct ArrayTexture final {
    GLuint index;
    glm::dvec2 uv;
};

class TextureArray final {
public:
    TextureArray();
    virtual ~TextureArray() = default;

    bool create(int width, int height, std::size_t size);
    void destroy();
    void submit();

    const ArrayTexture *emplace(const std::filesystem::path &path);
    const ArrayTexture *find(const std::filesystem::path &path) const;

    const glxx::Texture2DArray &get() const;

private:
    GLuint head;
    std::size_t size;
    int width, height;
    glxx::Texture2DArray texture;
    std::unordered_map<std::filesystem::path, ArrayTexture> map;
};

#endif /* F0779A19_ADAB_46E3_88D5_3FBC1E988025 */
