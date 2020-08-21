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
          EGLint const* context_attributes);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  context(std::shared_ptr<egl::display> const& disp, EGLContext parent_context,
          EGLConfig cfg, EGLint const* context_attributes);
  //----------------------------------------------------------------------------
  ~context();
  //============================================================================
  auto get() const -> auto const& { return m_egl_context; }
  auto get() -> auto& { return m_egl_context; }
};
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
