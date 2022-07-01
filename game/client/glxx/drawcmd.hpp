/*
 * Copyright (c) 2022 Kirill GPRB
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <game/client/glxx/object.hpp>
#include <spdlog/spdlog.h>

namespace glxx
{
struct DrawArraysCmd final {
    GLuint vertices;
    GLuint instances;
    GLuint base_vertex;
    GLuint base_instance;
};

struct DrawElementsCmd final {
    GLuint indices;
    GLuint instances;
    GLuint base_index;
    GLuint base_vertex;
    GLuint base_instance;
};

class DrawCommand final : public Object<DrawCommand> {
public:
    DrawCommand() = default;
    DrawCommand(GLenum mode, std::size_t vertices, std::size_t instances, std::size_t base_vertex, std::size_t base_instance);
    DrawCommand(GLenum mode, GLenum type, std::size_t indices, std::size_t instances, std::size_t base_index, std::size_t base_vertex, std::size_t base_instance);
    DrawCommand(DrawCommand &&rhs);
    DrawCommand &operator=(DrawCommand &&rhs);
    void create();
    void destroy();
    void set(GLenum mode, std::size_t vertices, std::size_t instances, std::size_t base_vertex, std::size_t base_instance);
    void set(GLenum mode, GLenum type, std::size_t indices, std::size_t instances, std::size_t base_index, std::size_t base_vertex, std::size_t base_instance);
    void invoke() const;
    std::size_t size() const;

private:
    bool indexed {false};
    GLenum mode {0};
    GLenum type {0};
    std::size_t nv {0};
};
} // namespace glxx

inline glxx::DrawCommand::DrawCommand(GLenum mode, std::size_t vertices, std::size_t instances, std::size_t base_vertex, std::size_t base_instance)
    : indexed(false), mode(mode), type(0), nv(0)
{
    create();
    set(mode, vertices, instances, base_vertex, base_instance);
}

inline glxx::DrawCommand::DrawCommand(GLenum mode, GLenum type, std::size_t indices, std::size_t instances, std::size_t base_index, std::size_t base_vertex, std::size_t base_instance)
    : indexed(true), mode(mode), type(type), nv(0)
{
    create();
    set(mode, type, indices, instances, base_index, base_vertex, base_instance);
}

inline glxx::DrawCommand::DrawCommand(glxx::DrawCommand &&rhs)
{
    handle = rhs.handle;
    indexed = rhs.indexed;
    mode = rhs.mode;
    type = rhs.type;
    nv = rhs.nv;
    rhs.handle = 0;
    rhs.indexed = false;
    rhs.mode = 0;
    rhs.type = 0;
    rhs.nv = 0;
}

inline glxx::DrawCommand &glxx::DrawCommand::operator=(glxx::DrawCommand &&rhs)
{
    glxx::DrawCommand copy(std::move(rhs));
    std::swap(handle, copy.handle);
    std::swap(indexed, rhs.indexed);
    std::swap(mode, rhs.mode);
    std::swap(type, rhs.type);
    std::swap(nv, rhs.nv);
    return *this;
}

inline void glxx::DrawCommand::create()
{
    destroy();
    glCreateBuffers(1, &handle);
}

inline void glxx::DrawCommand::destroy()
{
    if(handle) {
        glDeleteBuffers(1, &handle);
        handle = 0;
    }
}

inline void glxx::DrawCommand::set(GLenum mode, std::size_t vertices, std::size_t instances, std::size_t base_vertex, std::size_t base_instance)
{
    nv = vertices;
    indexed = false;
    this->mode = mode;
    glxx::DrawArraysCmd cmd = {};
    cmd.vertices = static_cast<GLuint>(vertices);
    cmd.instances = static_cast<GLuint>(instances);
    cmd.base_vertex = static_cast<GLuint>(base_vertex);
    cmd.base_instance = static_cast<GLuint>(base_instance);
    glNamedBufferData(handle, static_cast<GLsizeiptr>(sizeof(cmd)), &cmd, GL_STATIC_DRAW);
}

inline void glxx::DrawCommand::set(GLenum mode, GLenum type, std::size_t indices, std::size_t instances, std::size_t base_index, std::size_t base_vertex, std::size_t base_instance)
{
    nv = indices;
    indexed = true;
    this->mode = mode;
    this->type = type;
    glxx::DrawElementsCmd cmd = {};
    cmd.indices = static_cast<GLuint>(indices);
    cmd.instances = static_cast<GLuint>(instances);
    cmd.base_index = static_cast<GLuint>(base_index);
    cmd.base_vertex = static_cast<GLuint>(base_vertex);
    cmd.base_instance = static_cast<GLuint>(base_instance);
    glNamedBufferData(handle, static_cast<GLsizeiptr>(sizeof(cmd)), &cmd, GL_STATIC_DRAW);
}

inline void glxx::DrawCommand::invoke() const
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, handle);
    
    if(indexed) {
        glDrawElementsIndirect(mode, type, nullptr);
        return;
    }

    glDrawArraysIndirect(mode, nullptr);
}

inline std::size_t glxx::DrawCommand::size() const
{
    return nv;
}
