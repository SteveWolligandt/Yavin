#ifndef YAVIN__EGL_CONTEXT_H
#define YAVIN__EGL_CONTEXT_H
//==============================================================================
#include <yavin/egl/display.h>
#include <memory>
//==============================================================================
namespace yavin::egl {
//==============================================================================
class context {
  std::shared_ptr<egl::display> m_egl_display;
  EGLContext m_egl_context;
  //==============================================================================
 public:
  context(std::shared_ptr<egl::display> const& disp, EGLint* attributes)
      : m_egl_display{disp},
        m_egl_context{eglCreateContext(m_egl_display->get(),
                                       m_egl_display->config(), EGL_NO_CONTEXT,
                                       attributes)} {}
  //------------------------------------------------------------------------------
  ~context() { eglDestroyContext(m_egl_display->get(), m_egl_context); }
  //==============================================================================
  auto get() const -> auto const& { return m_egl_context; }
  auto get() -> auto& { return m_egl_context; }
};
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
