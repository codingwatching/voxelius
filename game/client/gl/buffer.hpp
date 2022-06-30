/* SPDX-License-Identifier: MPL-2.0 */
/* 
 * Copyright (c), 2022, Voxelius Team.
 * Created: Wed Jun 29 2022 19:21:38.
 * Author: Kirill GPRB.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <game/client/gl/object.hpp>

namespace gl
{
class Buffer final : public Object<Buffer> {
public:
    Buffer() = default;
    Buffer(Buffer &&rhs);
    Buffer &operator=(Buffer &&rhs);
    void create();
    void destroy();
    void storage(std::size_t size, const void *data, GLbitfield flags);
    void resize(std::size_t new_size, const void *data, GLbitfield usage);
    void write(std::size_t offset, std::size_t size, const void *data);
};
} // namespace gl

inline gl::Buffer::Buffer(gl::Buffer &&rhs)
{
    handle = rhs.handle;
    rhs.handle = 0;
}

inline gl::Buffer &gl::Buffer::operator=(gl::Buffer &&rhs)
{
    gl::Buffer copy(std::move(rhs));
    std::swap(handle, copy.handle);
    return *this;
}

inline void gl::Buffer::create()
{
    destroy();
    glCreateBuffers(1, &handle);
}

inline void gl::Buffer::destroy()
{
    if(handle) {
        glDeleteBuffers(1, &handle);
        handle = 0;
    }
}

inline void gl::Buffer::storage(std::size_t size, const void *data, GLbitfield flags)
{
    glNamedBufferStorage(handle, static_cast<GLsizeiptr>(size), data, flags);
}

inline void gl::Buffer::resize(std::size_t new_size, const void *data, GLbitfield usage)
{
    glNamedBufferData(handle, static_cast<GLsizeiptr>(new_size), data, usage);
}

inline void gl::Buffer::write(std::size_t offset, std::size_t size, const void *data)
{
    glNamedBufferSubData(handle, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
}
