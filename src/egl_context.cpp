#include <yavin/egl_context.h>
#include <yavin/egl_window.h>
//==============================================================================
namespace yavin {
//==============================================================================
context::context() {
  EGLint attr_list[]{EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_NONE};
  m_egl_env = std::make_shared<egl::environment>(attr_list);
  setup(EGL_NO_CONTEXT, true);
}
//------------------------------------------------------------------------------
context::context(context const& parent)  {
  EGLint attr_list[]{EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_NONE};
  m_egl_env = std::make_shared<egl::environment>(attr_list);
  setup(parent.m_egl_context->get(), false);
}
//------------------------------------------------------------------------------
context::context(window const& parent)  {
  EGLint attr_list[]{EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_NONE};
  m_egl_env = std::make_shared<egl::environment>(attr_list);
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
  if (!eglMakeCurrent(m_egl_env->display()->get(), EGL_NO_SURFACE,
                      EGL_NO_SURFACE, m_egl_context->get())) {
    throw std::runtime_error{"[EGL] make context current."};
  }
  if (m_glew == nullptr) { m_glew = glew::create(); }
}
//------------------------------------------------------------------------------
void context::release() {
  if (!eglMakeCurrent(m_egl_env->display()->get(), EGL_NO_SURFACE,
                      EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
    throw std::runtime_error{"[EGL] could not release context"};
  }
}
//------------------------------------------------------------------------------
void context::setup(EGLContext parent, bool cur) {
  // EGL context and surface
  eglBindAPI(EGL_OPENGL_API);
  EGLint context_attributes[] = {EGL_CONTEXT_MAJOR_VERSION, 4,
                                 EGL_CONTEXT_MINOR_VERSION, 5, EGL_NONE};
  m_egl_context = std::make_shared<egl::context>(m_egl_env->display(), parent,
                                                 context_attributes);
  if (cur) { make_current(); }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
