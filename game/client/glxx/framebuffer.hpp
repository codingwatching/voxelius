/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef BD1F3414_2439_43B8_AC4E_328F85870738
#define BD1F3414_2439_43B8_AC4E_328F85870738
#include <game/client/glxx/renderbuffer.hpp>
#include <game/client/glxx/texture.hpp>
#include <common/cxpr.hpp>

namespace glxx
{
class Framebuffer final : public Object<Framebuffer> {
public:
    Framebuffer() = default;
    Framebuffer(Framebuffer &&rhs);
    Framebuffer &operator=(Framebuffer &&rhs);
    void create();
    void destroy();
    void attach(GLenum attachment, const Texture2D &texture);
    void attach(GLenum attachment, const Renderbuffer &rbo);
    bool complete() const;
    void bind() const;
    template<typename... args_type>
    void setFragmentTargets(args_type &&... args);
    static void unbind();
};
} // namespace glxx

inline glxx::Framebuffer::Framebuffer(glxx::Framebuffer &&rhs)
{
    handle = rhs.handle;
    rhs.handle = 0;
}

inline glxx::Framebuffer &glxx::Framebuffer::operator=(glxx::Framebuffer &&rhs)
{
    glxx::Framebuffer copy(std::move(rhs));
    std::swap(handle, copy.handle);
    return *this;
}

inline void glxx::Framebuffer::create()
{
    destroy();
    glCreateFramebuffers(1, &handle);
}

inline void glxx::Framebuffer::destroy()
{
    if(handle) {
        glDeleteFramebuffers(1, &handle);
        handle = 0;
    }
}

inline void glxx::Framebuffer::attach(GLenum attachment, const glxx::Texture2D &texture)
{
    glNamedFramebufferTexture(handle, attachment, texture.get(), 0);
}

inline void glxx::Framebuffer::attach(GLenum attachment, const glxx::Renderbuffer &rbo)
{
    glNamedFramebufferRenderbuffer(handle, attachment, GL_RENDERBUFFER, rbo.get());
}

inline bool glxx::Framebuffer::complete() const
{
    return glCheckNamedFramebufferStatus(handle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

inline void glxx::Framebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

template<typename... args_type>
inline void glxx::Framebuffer::setFragmentTargets(args_type &&... args)
{
    const GLenum attachments[] = { static_cast<GLenum>(args)... };
    glNamedFramebufferDrawBuffers(handle, static_cast<GLsizei>(cxpr::arraySize(attachments)), attachments);
}

inline void glxx::Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


#endif /* BD1F3414_2439_43B8_AC4E_328F85870738 */
