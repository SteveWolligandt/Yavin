#include <yavin/egl_window.h>

//==============================================================================
namespace yavin::egl {
//==============================================================================
window::window(EGLint w, EGLint h, EGLint major, EGLint minor)
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

  const EGLint pbufferAttribs[] = {
        EGL_WIDTH, w,
        EGL_HEIGHT, h,
        EGL_NONE,
  };
  m_egl_surface =
      eglCreatePbufferSurface(m_egl_display, egl_cfg, pbufferAttribs);

  eglBindAPI(EGL_OPENGL_API);
  m_egl_context =
      eglCreateContext(m_egl_display, egl_cfg, EGL_NO_CONTEXT, nullptr);

  // initialize glew
  std::cerr << "Created GL " << major << "." << minor << " egl::window\n";
  glewExperimental = true;
  eglMakeCurrent(m_egl_display, m_egl_surface, m_egl_surface, m_egl_context);
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    throw std::runtime_error{std::string("cannot initialize GLEW: ") +
                             std::string((char *)glewGetErrorString(err))};
  }
}
//------------------------------------------------------------------------------
window::~window() {
  eglTerminate(m_egl_display);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
