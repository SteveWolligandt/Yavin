#ifndef YAVIN_CONTEXT_H
#define YAVIN_CONTEXT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>
#include <string>

#include <yavin/glincludes.h>
#include <GL/glx.h>

//==============================================================================
namespace yavin {
//==============================================================================

class glx_context {
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig,
                                                       GLXContext, Bool,
                                                       const int *);

  static std::list<glx_context *> contexts;
  static bool                 error_occured;
  static const int            visual_attribs[23];
  Display *                   display;
  Window                      win;
  GLXContext                  ctx;
  Colormap                    cmap;

 public:
  glx_context(int major = 3, int minor = 0);
  ~glx_context();
  void make_current() { glXMakeCurrent(display, win, ctx); }

  //! Helper to check for extension string presence.  Adapted from:
  //!   http://www.opengl.org/resources/features/OGLextensions/
  static bool extension_supported(const char *extList, const char *extension);

  static int error_handler_static(Display *dpy, XErrorEvent *ev);
  int        error_handler(XErrorEvent *ev);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#ifndef YAVIN_X11_CONTEXT_DONT_DELETE
#undef Always
#undef None
#undef Success
#endif
#endif
