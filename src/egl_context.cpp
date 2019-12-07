#include <yavin/egl_context.h>

//==============================================================================
namespace yavin {
//==============================================================================
egl_context::egl_context(EGLint major, EGLint minor)
    : m_egl_display{eglGetDisplay(EGL_DEFAULT_DISPLAY)} {
  if (m_egl_display == EGL_NO_DISPLAY) {
    throw std::runtime_error{"[EGL] could not get a display"};
  }
  if (!eglInitialize(m_egl_display, &major, &minor)) {
    throw std::runtime_error{"[EGL] failed to initialize"};
  }

  EGLint    num_cfgs;
  EGLConfig egl_cfg;

  if (!eglChooseConfig(m_egl_display, attribute_list.data(), &egl_cfg, 1,
                       &num_cfgs)) {
    throw std::runtime_error{"[EGL] failed to choose config"};
  }
  //m_egl_surf = eglCreatePbufferSurface(m_egl_display, egl_cfg, pbufferAttribs);
  eglBindAPI(EGL_OPENGL_API);
  m_egl_context =
      eglCreateContext(m_egl_display, egl_cfg, EGL_NO_CONTEXT, nullptr);
  std::cerr << "Created GL " << major << "." << minor << " egl_context\n";
  glewExperimental = true;
  eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, m_egl_context);
  GLenum err       = glewInit();
  if (GLEW_OK != err) {
    throw std::runtime_error{std::string("cannot initialize GLEW: ") +
                             std::string((char *)glewGetErrorString(err))};
  }
}
//------------------------------------------------------------------------------
egl_context::~egl_context() {
  eglTerminate(m_egl_display);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
