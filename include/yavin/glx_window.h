#ifndef YAVIN_GLX_WINDOW_H
#define YAVIN_GLX_WINDOW_H
//==============================================================================
#include <GL/glx.h>
#include <yavin/glincludes.h>

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "imgui_api_backend.h"
#include "imgui_render_backend.h"
#include "window_listener.h"
#include "window_notifier.h"
#include "x11.h"
//==============================================================================
namespace yavin {
//==============================================================================
class window : public window_notifier{
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig,
                                                       GLXContext, Bool,
                                                       const int *);
  static std::list<window *> contexts;

  static bool                      m_error_occured;
  static const int                 m_visual_attribs[23];
  Display *                        m_display;
  Screen *                         m_screen;
  int                              m_screen_id;
  Window                           m_window;
  GLXContext                       m_context;
  Colormap                         m_colormap;
  XEvent                           m_xevent;
  std::unique_ptr<imgui_render_backend>         m_imgui_render_backend;
  bool                             m_shift_down, m_ctrl_down, m_alt_down;

 public:
  window(const std::string &title, unsigned int width, unsigned int height,
         int major = 4, int minor = 5);
  ~window();
  void make_current() { glXMakeCurrent(m_display, m_window, m_context); }
  void refresh();
  void render_imgui();
  void check_events();

  /// Helper to check for extension string presence. Adapted from:
  /// http://www.opengl.org/resources/features/OGLextensions/
  static bool extension_supported(const char *extList, const char *extension);

  static int error_handler_static(Display *dpy, XErrorEvent *ev);
  int        error_handler(XErrorEvent *ev);
  void       swap_buffers();
  bool       shift_down() const { return m_shift_down; }
  bool       ctrl_down() const { return m_ctrl_down; }
  bool       alt_down() const { return m_alt_down; }

 private:
  void init_imgui();
  void deinit_imgui();
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#include "x11undefs.h"
#endif
