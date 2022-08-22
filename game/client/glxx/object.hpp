/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef B4A95A22_058B_484D_8689_DC7A008C3267
#define B4A95A22_058B_484D_8689_DC7A008C3267
#include <common/mixin.hpp>
#include <cstddef>
#include <glad/gl.h>
#include <utility>

namespace glxx
{
template<typename object_type>
class Object : public mixin::NonCopyable {
public:
    Object() = default;
    virtual ~Object();
    void create();
    void destroy();
    constexpr bool valid() const;
    constexpr GLuint get() const;

protected:
    GLuint handle {0};
};
} // namespace glxx

template<typename object_type>
inline glxx::Object<object_type>::~Object()
{
    destroy();
}

template<typename object_type>
inline void glxx::Object<object_type>::create()
{
    static_cast<object_type *>(this)->create();
}

template<typename object_type>
inline void glxx::Object<object_type>::destroy()
{
    static_cast<object_type *>(this)->destroy();
}

template<typename object_type>
inline constexpr bool glxx::Object<object_type>::valid() const
{
    return handle != 0;
}

template<typename object_type>
inline constexpr GLuint glxx::Object<object_type>::get() const
{
    return handle;
}


#endif /* B4A95A22_058B_484D_8689_DC7A008C3267 */
