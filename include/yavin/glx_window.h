#ifndef YAVIN_GLX_WINDOW_H
#define YAVIN_GLX_WINDOW_H
//==============================================================================
#include <GL/glx.h>
#include <imgui/imgui.h>
#include <yavin/glincludes.h>

#include <list>
#include <string>
#include <vector>

#include "keyboard.h"
#include "mouse.h"
#include "x11.h"
//==============================================================================
namespace yavin {
//==============================================================================
struct window_listener : keyboard_listener, button_listener {
  virtual void on_mouse_motion(int /*x*/, int /*y*/) {}
  virtual void on_resize(int /*width*/, int /*height*/) {}
};
//==============================================================================
class window {
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig,
                                                       GLXContext, Bool,
                                                       const int *);
  static std::list<window *> contexts;

  std::vector<window_listener *>   m_listeners;
  static bool                      m_error_occured;
  static const int                 m_visual_attribs[23];
  Display *                        m_display;
  Screen *                         m_screen;
  int                              m_screen_id;
  Window                           m_window;
  GLXContext                       m_context;
  Colormap                         m_colormap;
  XEvent                           m_xevent;
  ImGuiIO &                        m_imgui_io;

 public:
  window(const std::string &title, unsigned int width, unsigned int height,
         int major = 4, int minor = 5);
  ~window();
  void make_current() { glXMakeCurrent(m_display, m_window, m_context); }
  void check_events();
  void add_listener(window_listener &l);

  /// Helper to check for extension string presence. Adapted from:
  /// http://www.opengl.org/resources/features/OGLextensions/
  static bool extension_supported(const char *extList, const char *extension);

  static int error_handler_static(Display *dpy, XErrorEvent *ev);
  int        error_handler(XErrorEvent *ev);

 private:
  ImGuiIO &init_imgui() const;
  void deinit_imgui() const;
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#include "x11undefs.h"
#endif
