#include <yavin/egl/context.h>
//==============================================================================
namespace yavin::egl {
//==============================================================================
context::context(std::shared_ptr<egl::display> const& disp, EGLConfig cfg,
                 EGLint const* context_attributes)
    : context(disp, EGL_NO_CONTEXT, cfg, context_attributes) {}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
context::context(std::shared_ptr<egl::display> const& disp,
                 EGLContext parent_context, EGLConfig cfg,
                 EGLint const* context_attributes)
    : m_egl_display{disp} {
  m_egl_context = eglCreateContext(m_egl_display->get(), cfg, parent_context,
                                   context_attributes);
  if (auto const err = eglGetError(); err != EGL_SUCCESS) {
    throw std::runtime_error{"[EGL] could not create context: " +
                             egl::error_string(err)};
  }
}
//----------------------------------------------------------------------------
context::~context() {
  eglDestroyContext(m_egl_display->get(), m_egl_context);
}
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
