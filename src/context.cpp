#include <yavin/context.h>
#include <yavin/window.h>
//==============================================================================
namespace yavin {
//==============================================================================
context::context() : m_egl_disp{std::make_shared<egl::display>()} {
  setup(EGL_NO_CONTEXT, true);
}
//------------------------------------------------------------------------------
context::context(context const& parent) : m_egl_disp{parent.m_egl_disp} {
  setup(parent.m_egl_context->get(), false);
}
//------------------------------------------------------------------------------
context::context(window const& parent) : m_egl_disp{parent.m_egl_disp} {
  setup(parent.m_egl_context->get(), false);
}
//==============================================================================
// methods
//==============================================================================
context context::create_shared_context() const {
  return context{*this};
}
//------------------------------------------------------------------------------
void context::make_current() {
  if (!eglMakeCurrent(m_egl_disp->get(), EGL_NO_SURFACE,
                      EGL_NO_SURFACE, m_egl_context->get())) {
    throw std::runtime_error{"[EGL] make context current."};
  }
  if (m_glew == nullptr) { m_glew = glew::create(); }
}
//------------------------------------------------------------------------------
void context::release() {
  if (!eglMakeCurrent(m_egl_disp->get(), EGL_NO_SURFACE,
                      EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
    throw std::runtime_error{"[EGL] could not release context"};
  }
}
//------------------------------------------------------------------------------
void context::setup(EGLContext parent, bool cur) {
  eglBindAPI(EGL_OPENGL_API);
  EGLint const ctx_attrs[] = {EGL_CONTEXT_MAJOR_VERSION, 4,
                              EGL_CONTEXT_MINOR_VERSION, 5,
                              EGL_NONE};
  EGLint const cfg_attrs[] = {EGL_RED_SIZE,     8,
                              EGL_GREEN_SIZE,   8,
                              EGL_BLUE_SIZE,    8,
                              EGL_ALPHA_SIZE,   0,
                              EGL_DEPTH_SIZE,   0,
                              EGL_STENCIL_SIZE, 0,
                              EGL_CONFORMANT,   EGL_OPENGL_BIT,
                              EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                              EGL_NONE};
  auto         cfg         = m_egl_disp->create_config(cfg_attrs);

  m_egl_context =
      std::make_shared<egl::context>(m_egl_disp, parent, cfg, ctx_attrs);
  if (cur) { make_current(); }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
