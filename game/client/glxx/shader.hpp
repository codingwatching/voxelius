/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef E29ED32A_C651_432D_BBC4_B9705EC39AB8
#define E29ED32A_C651_432D_BBC4_B9705EC39AB8
#include <game/client/glxx/object.hpp>
#include <string>
#include <spdlog/spdlog.h>
#include <vector>

namespace glxx
{
class Shader final : public Object<Shader> {
public:
    Shader() = default;
    Shader(Shader &&rhs);
    Shader &operator=(Shader &&rhs);
    void create();
    void destroy();
    bool glsl(GLenum stage, const std::string &source);
    bool spirv(GLenum stage, const std::vector<uint8_t> &binary);
    constexpr GLbitfield stageBit() const;

private:
    GLbitfield bit {0};
};

namespace detail
{
static inline GLbitfield getStageBit(GLenum stage)
{
    if(stage == GL_VERTEX_SHADER)
        return GL_VERTEX_SHADER_BIT;
    if(stage == GL_FRAGMENT_SHADER)
        return GL_FRAGMENT_SHADER_BIT;
    return 0;
}

static inline void checkShaderInfoLog(GLuint shader)
{
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if(length > 1) {
        std::string info_log(static_cast<std::size_t>(length) + 1, static_cast<char>(0));
        glGetShaderInfoLog(shader, static_cast<GLsizei>(info_log.size()), nullptr, info_log.data());
        spdlog::debug(info_log);
    }
}

static inline void checkProgramInfoLog(GLuint program)
{
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if(length > 1) {
        std::string info_log(static_cast<std::size_t>(length) + 1, static_cast<char>(0));
        glGetProgramInfoLog(program, static_cast<GLsizei>(info_log.size()), nullptr, info_log.data());
        spdlog::debug(info_log);
    }
}
} // namespace detail
} // namespace glxx

inline glxx::Shader::Shader(glxx::Shader &&rhs)
    : bit(rhs.bit)
{
    handle = rhs.handle;
    rhs.handle = 0;
    rhs.bit = 0;
}

inline glxx::Shader &glxx::Shader::operator=(glxx::Shader &&rhs)
{
    glxx::Shader copy(std::move(rhs));
    std::swap(handle, copy.handle);
    std::swap(bit, copy.bit);
    return *this;
}

inline void glxx::Shader::create()
{
    destroy();
    handle = glCreateProgram();
    glProgramParameteri(handle, GL_PROGRAM_SEPARABLE, GL_TRUE);
}

inline void glxx::Shader::destroy()
{
    if(handle) {
        glDeleteProgram(handle);
        handle = 0;
        bit = 0;
    }
}

inline bool glxx::Shader::glsl(GLenum stage, const std::string &source)
{
    if(!bit && (bit = glxx::detail::getStageBit(stage))) {
        GLint status;
        const char *data = source.c_str();

        GLuint shader = glCreateShader(stage);
        glShaderSource(shader, 1, &data, nullptr);
        glCompileShader(shader);
        glxx::detail::checkShaderInfoLog(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(!status) {
            glDeleteShader(shader);
            return false;
        }

        glAttachShader(handle, shader);
        glLinkProgram(handle);
        glDeleteShader(shader);
        glxx::detail::checkProgramInfoLog(handle);

        glGetProgramiv(handle, GL_LINK_STATUS, &status);
        return !!status;
    }

    return false;
}

inline bool glxx::Shader::spirv(GLenum stage, const std::vector<uint8_t> &binary)
{
    if(!bit && (bit = glxx::detail::getStageBit(stage))) {
        GLint status;

        GLuint shader = glCreateShader(stage);
        glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), static_cast<GLsizei>(binary.size()));
        glSpecializeShader(shader, "main", 0, nullptr, nullptr);
        glxx::detail::checkShaderInfoLog(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(!status) {
            glDeleteShader(shader);
            return false;
        }

        glAttachShader(handle, shader);
        glLinkProgram(handle);
        glDeleteShader(shader);
        glxx::detail::checkProgramInfoLog(handle);

        glGetProgramiv(handle, GL_LINK_STATUS, &status);
        return !!status;
    }

    return false;
}

inline constexpr GLbitfield glxx::Shader::stageBit() const
{
    return bit;
}


#endif /* E29ED32A_C651_432D_BBC4_B9705EC39AB8 */
