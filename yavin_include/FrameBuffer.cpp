#include "FrameBuffer.h"

namespace Yavin {
FrameBuffer::FrameBuffer() {
  glCreateFramebuffers(1, &m_id);
  gl_error_check("glCreateFramebuffers");
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) {
  other.m_dont_delete = true;
  m_id                = other.m_id;
}

FrameBuffer::~FrameBuffer() {
  if (!m_dont_delete) {
    glDeleteFramebuffers(1, &m_id);
    gl_error_check("glDeleteFramebuffers");
  }
}

template <typename T, typename Components>
void FrameBuffer::attachTex2D(const Texture2D<T, Components>& tex, unsigned int i) {
  assert(i < GL_MAX_COLOR_ATTACHMENTS);
  glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0 + i, tex.id(), 0);
  gl_error_check("glNamedFramebufferTexture");
  gl_framebuffer_not_complete_check(m_id);
}

template void FrameBuffer::attachTex2D<float, RGBA>(const Texture2D<float, RGBA>&, unsigned int);
template void FrameBuffer::attachTex2D<int8_t, RGBA>(const Texture2D<int8_t, RGBA>&, unsigned int);
template void FrameBuffer::attachTex2D<uint8_t, RGBA>(const Texture2D<uint8_t, RGBA>&,
                                                      unsigned int);
template void FrameBuffer::attachTex2D<int16_t, RGBA>(const Texture2D<int16_t, RGBA>&,
                                                      unsigned int);
template void FrameBuffer::attachTex2D<uint16_t, RGBA>(const Texture2D<uint16_t, RGBA>&,
                                                       unsigned int);
template void FrameBuffer::attachTex2D<int32_t, RGBA>(const Texture2D<int32_t, RGBA>&,
                                                      unsigned int);
template void FrameBuffer::attachTex2D<uint32_t, RGBA>(const Texture2D<uint32_t, RGBA>&,
                                                       unsigned int);
template void FrameBuffer::attachTex2D<float, RGB>(const Texture2D<float, RGB>&, unsigned int);
template void FrameBuffer::attachTex2D<int8_t, RGB>(const Texture2D<int8_t, RGB>&, unsigned int);
template void FrameBuffer::attachTex2D<uint8_t, RGB>(const Texture2D<uint8_t, RGB>&, unsigned int);
template void FrameBuffer::attachTex2D<int16_t, RGB>(const Texture2D<int16_t, RGB>&, unsigned int);
template void FrameBuffer::attachTex2D<uint16_t, RGB>(const Texture2D<uint16_t, RGB>&,
                                                      unsigned int);
template void FrameBuffer::attachTex2D<int32_t, RGB>(const Texture2D<int32_t, RGB>&, unsigned int);
template void FrameBuffer::attachTex2D<uint32_t, RGB>(const Texture2D<uint32_t, RGB>&,
                                                      unsigned int);

void FrameBuffer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_id); }
void FrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::clear() {
  // glClearNamedFramebufferfi(m_id, GLenum buffer, GLint drawbuffer,
  //                           GLfloat depth, GLint stencil);
}

GLuint FrameBuffer::id() const { return m_id; }
}  // Yavin