/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Sat Jul 02 2022 21:22:20.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef EFC8BBEF_0C82_41EC_A112_7DE5C1948B96
#define EFC8BBEF_0C82_41EC_A112_7DE5C1948B96
#include <common/mixin.hpp>
#include <common/vfs.hpp>
#include <game/client/glxx/texture.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <unordered_map>

class Atlas final {
public:
    struct Entry final {
        GLuint index;
        glm::dvec2 uv;
    };

public:
    Atlas();
    virtual ~Atlas() = default;

    bool create(int width, int height, int count);
    void destroy();
    void submit();

    const Entry *emplace(const std::filesystem::path &path);
    const Entry *find(const std::filesystem::path &path) const;
    const glxx::Texture2DArray &get() const;

private:
    int width, height, count;
    GLuint last_index;
    glxx::Texture2DArray texture;
    std::unordered_map<std::filesystem::path, Entry> entries;
};


#endif /* EFC8BBEF_0C82_41EC_A112_7DE5C1948B96 */
