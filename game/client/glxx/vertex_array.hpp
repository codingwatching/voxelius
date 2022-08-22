/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef F07D2DDF_2D70_4BFD_A4F5_A62DB7CDB6D7
#define F07D2DDF_2D70_4BFD_A4F5_A62DB7CDB6D7
#include <game/client/glxx/buffer.hpp>
#include <spdlog/spdlog.h>

namespace glxx
{
class VertexArray final : public Object<VertexArray> {
public:
    VertexArray() = default;
    VertexArray(VertexArray &&rhs);
    VertexArray &operator=(VertexArray &&rhs);
    void create();
    void destroy();
    void bind() const;
    void setIndexBuffer(const Buffer &ibo);
    void setVertexBuffer(GLuint binding, const Buffer &vbo, std::size_t stride);
    void enableAttribute(GLuint attrib, bool enable);
    void setAttributeFormat(GLuint attrib, GLenum type, std::size_t count, std::size_t offset, bool normalized);
    void setAttributeBinding(GLuint attrib, GLuint binding);
};
} // namespace glxx

inline glxx::VertexArray::VertexArray(glxx::VertexArray &&rhs)
{
    handle = rhs.handle;
    rhs.handle = 0;
}

inline glxx::VertexArray &glxx::VertexArray::operator=(glxx::VertexArray &&rhs)
{
    glxx::VertexArray copy(std::move(rhs));
    std::swap(handle, copy.handle);
    return *this;
}

inline void glxx::VertexArray::create()
{
    destroy();
    glCreateVertexArrays(1, &handle);
}

inline void glxx::VertexArray::destroy()
{
    if(handle) {
        glDeleteVertexArrays(1, &handle);
        handle = 0;
    }
}

inline void glxx::VertexArray::bind() const
{
    glBindVertexArray(handle);
}

inline void glxx::VertexArray::setIndexBuffer(const glxx::Buffer &ibo)
{
    glVertexArrayElementBuffer(handle, ibo.get());
}

inline void glxx::VertexArray::setVertexBuffer(GLuint binding, const glxx::Buffer &vbo, std::size_t stride)
{
    glVertexArrayVertexBuffer(handle, binding, vbo.get(), 0, static_cast<GLsizei>(stride));
}

inline void glxx::VertexArray::enableAttribute(GLuint attrib, bool enable)
{
    if(enable) {
        glEnableVertexArrayAttrib(handle, attrib);
        return;
    }

    glDisableVertexArrayAttrib(handle, attrib);
}

inline void glxx::VertexArray::setAttributeFormat(GLuint attrib, GLenum type, std::size_t count, std::size_t offset, bool normalized)
{
    switch(type) {
        case GL_FLOAT:
            glVertexArrayAttribFormat(handle, attrib, static_cast<GLint>(count), type, normalized ? GL_TRUE : GL_FALSE, static_cast<GLuint>(offset));
            break;
        case GL_INT:
        case GL_UNSIGNED_INT:
            glVertexArrayAttribIFormat(handle, attrib, static_cast<GLint>(count), type, static_cast<GLuint>(offset));
            break;
        default:
            spdlog::warn("GL: unknown attrib type {}", type);
            break;
    }
}

inline void glxx::VertexArray::setAttributeBinding(GLuint attrib, GLuint binding)
{
    glVertexArrayAttribBinding(handle, attrib, binding);
}


#endif /* F07D2DDF_2D70_4BFD_A4F5_A62DB7CDB6D7 */
