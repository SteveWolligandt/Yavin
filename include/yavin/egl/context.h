#ifndef YAVIN_EGL_CONTEXT_H
#define YAVIN_EGL_CONTEXT_H
//==============================================================================
#include <yavin/egl/display.h>

#include <memory>
//==============================================================================
namespace yavin::egl {
//==============================================================================
class context {
  std::shared_ptr<egl::display> m_egl_display;
  EGLContext                    m_egl_context;
  //============================================================================
 public:
  context(std::shared_ptr<egl::display> const& disp, EGLConfig cfg,
          EGLint const* context_attributes)
      : context(disp, EGL_NO_CONTEXT, cfg, context_attributes) {}
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  context(std::shared_ptr<egl::display> const& disp, EGLContext parent_context,
          EGLConfig cfg, EGLint const* context_attributes)
      : m_egl_display{disp} {
    m_egl_context = eglCreateContext(m_egl_display->get(), cfg, parent_context,
                                     context_attributes);
    if (auto const err = eglGetError(); err != EGL_SUCCESS) {
      throw std::runtime_error{"[EGL] could not create context: " +
                               egl::error_string(err)};
    }
  }
  //----------------------------------------------------------------------------
  ~context() { eglDestroyContext(m_egl_display->get(), m_egl_context); }
  //============================================================================
  auto get() const -> auto const& { return m_egl_context; }
  auto get() -> auto& { return m_egl_context; }
};
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
