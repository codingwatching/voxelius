/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef FF5205F0_4FB9_459E_8512_47E9AB32304F
#define FF5205F0_4FB9_459E_8512_47E9AB32304F
#include <game/client/glxx/shader.hpp>

namespace glxx
{
class Pipeline : public Object<Pipeline> {
public:
    Pipeline() = default;
    Pipeline(Pipeline &&rhs);
    Pipeline &operator=(Pipeline &&rhs);
    void create();
    void destroy();
    void stage(const Shader &shader);
    void bind() const;
};
} // namespace glxx

inline glxx::Pipeline::Pipeline(glxx::Pipeline &&rhs)
{
    handle = rhs.handle;
    rhs.handle = 0;
}

inline glxx::Pipeline &glxx::Pipeline::operator=(glxx::Pipeline &&rhs)
{
    glxx::Pipeline copy(std::move(rhs));
    std::swap(handle, copy.handle);
    return *this;
}

inline void glxx::Pipeline::create()
{
    destroy();
    glCreateProgramPipelines(1, &handle);
}

inline void glxx::Pipeline::destroy()
{
    if(handle) {
        glDeleteProgramPipelines(1, &handle);
        handle = 0;
    }
}

inline void glxx::Pipeline::stage(const glxx::Shader &shader)
{
    glUseProgramStages(handle, shader.stageBit(), shader.get());
}

inline void glxx::Pipeline::bind() const
{
    glBindProgramPipeline(handle);
}


#endif /* FF5205F0_4FB9_459E_8512_47E9AB32304F */
