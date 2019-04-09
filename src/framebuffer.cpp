#include <yavin/framebuffer.h>

//==============================================================================
namespace yavin {
//==============================================================================

framebuffer::framebuffer() {
  gl::create_framebuffers(1, &id());
}

//------------------------------------------------------------------------------
framebuffer::~framebuffer() {
  gl::delete_framebuffers(1, &id());
}

//------------------------------------------------------------------------------
template <typename T, typename Components>
void framebuffer::attach(const tex2D<T, Components>& tex, unsigned int i) {
  assert(i < GL_MAX_COLOR_ATTACHMENTS);
  gl::named_framebuffer_texture(id(), GL_COLOR_ATTACHMENT0 + i, tex.id(), 0);
  gl_framebuffer_not_complete_check(id());
}

//------------------------------------------------------------------------------
template <typename T>
void framebuffer::attach(const tex2D<T, Depth>& tex) {
  gl::named_framebuffer_texture(id(), GL_DEPTH_ATTACHMENT, tex.id(), 0);
  gl_framebuffer_not_complete_check(id());
}

//------------------------------------------------------------------------------
void framebuffer::bind() {
  gl::bind_framebuffer(GL_FRAMEBUFFER, id());
}

//------------------------------------------------------------------------------
void framebuffer::unbind() {
  gl::bind_framebuffer(GL_FRAMEBUFFER, 0);
}

//------------------------------------------------------------------------------
void framebuffer::clear() {
  // glClearNamedFramebufferfi(id(), GLenum buffer, GLint drawbuffer,
  //                           GLfloat depth, GLint stencil);
}

//==============================================================================
template void framebuffer::attach<float, R>(const tex2D<float, R>&,
                                            unsigned int);
template void framebuffer::attach<int8_t, R>(const tex2D<int8_t, R>&,
                                             unsigned int);
template void framebuffer::attach<uint8_t, R>(const tex2D<uint8_t, R>&,
                                              unsigned int);
template void framebuffer::attach<int16_t, R>(const tex2D<int16_t, R>&,
                                              unsigned int);
template void framebuffer::attach<uint16_t, R>(const tex2D<uint16_t, R>&,
                                               unsigned int);
template void framebuffer::attach<int32_t, R>(const tex2D<int32_t, R>&,
                                              unsigned int);
template void framebuffer::attach<uint32_t, R>(const tex2D<uint32_t, R>&,
                                               unsigned int);

template void framebuffer::attach<float, RG>(const tex2D<float, RG>&,
                                             unsigned int);
template void framebuffer::attach<int8_t, RG>(const tex2D<int8_t, RG>&,
                                              unsigned int);
template void framebuffer::attach<uint8_t, RG>(const tex2D<uint8_t, RG>&,
                                               unsigned int);
template void framebuffer::attach<int16_t, RG>(const tex2D<int16_t, RG>&,
                                               unsigned int);
template void framebuffer::attach<uint16_t, RG>(const tex2D<uint16_t, RG>&,
                                                unsigned int);
template void framebuffer::attach<int32_t, RG>(const tex2D<int32_t, RG>&,
                                               unsigned int);
template void framebuffer::attach<uint32_t, RG>(const tex2D<uint32_t, RG>&,
                                                unsigned int);
template void framebuffer::attach<float, RGB>(const tex2D<float, RGB>&,
                                              unsigned int);
template void framebuffer::attach<int8_t, RGB>(const tex2D<int8_t, RGB>&,
                                               unsigned int);
template void framebuffer::attach<uint8_t, RGB>(const tex2D<uint8_t, RGB>&,
                                                unsigned int);
template void framebuffer::attach<int16_t, RGB>(const tex2D<int16_t, RGB>&,
                                                unsigned int);
template void framebuffer::attach<uint16_t, RGB>(const tex2D<uint16_t, RGB>&,
                                                 unsigned int);
template void framebuffer::attach<int32_t, RGB>(const tex2D<int32_t, RGB>&,
                                                unsigned int);
template void framebuffer::attach<uint32_t, RGB>(const tex2D<uint32_t, RGB>&,
                                                 unsigned int);
template void framebuffer::attach<float, RGBA>(const tex2D<float, RGBA>&,
                                               unsigned int);
template void framebuffer::attach<int8_t, RGBA>(const tex2D<int8_t, RGBA>&,
                                                unsigned int);
template void framebuffer::attach<uint8_t, RGBA>(const tex2D<uint8_t, RGBA>&,
                                                 unsigned int);
template void framebuffer::attach<int16_t, RGBA>(const tex2D<int16_t, RGBA>&,
                                                 unsigned int);
template void framebuffer::attach<uint16_t, RGBA>(const tex2D<uint16_t, RGBA>&,
                                                  unsigned int);
template void framebuffer::attach<int32_t, RGBA>(const tex2D<int32_t, RGBA>&,
                                                 unsigned int);
template void framebuffer::attach<uint32_t, RGBA>(const tex2D<uint32_t, RGBA>&,
                                                  unsigned int);
template void framebuffer::attach<uint32_t>(const tex2D<uint32_t, Depth>&);
template void framebuffer::attach<float>(const tex2D<float, Depth>&);


//==============================================================================
}  // namespace yavin
//==============================================================================
