#ifndef YAVIN_GLX_WINDOW_H
#define YAVIN_GLX_WINDOW_H
//==============================================================================
#include <GL/glx.h>
#include <yavin/glincludes.h>

#include <list>
#include <string>

#include "x11.h"
//==============================================================================
namespace yavin {
//==============================================================================
class window {
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig,
                                                       GLXContext, Bool,
                                                       const int *);
  static std::list<window *> contexts;
  static bool                error_occured;
  static const int           visual_attribs[23];
  Display *                  display;
  Window                     win;
  GLXContext                 ctx;
  Colormap                   cmap;

 public:
  window(const std::string &title, int major = 3, int minor = 0);
  ~window();
  void make_current() { glXMakeCurrent(display, win, ctx); }
  void check_keyboard();

  /// Helper to check for extension string presence. Adapted from:
  /// http://www.opengl.org/resources/features/OGLextensions/
  static bool extension_supported(const char *extList, const char *extension);

  static int error_handler_static(Display *dpy, XErrorEvent *ev);
  int        error_handler(XErrorEvent *ev);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#include "x11undefs.h"
#endif
