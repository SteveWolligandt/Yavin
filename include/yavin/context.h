#ifndef __YAVIN_CONTEXT_H__
#define __YAVIN_CONTEXT_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>
#include <string>

#include <yavin/gl_includes.h>
#include <GL/glx.h>

//==============================================================================
namespace yavin {
//==============================================================================

class Context {
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig,
                                                       GLXContext, Bool,
                                                       const int *);

  static std::list<Context *> contexts;
  static bool                 error_occured;
  static const int            visual_attribs[23];
  Display *                   display;
  Window                      win;
  GLXContext                  ctx;
  Colormap                    cmap;

 public:
  Context(int major = 3, int minor = 0);
  ~Context();
  void make_current() { glXMakeCurrent(display, win, ctx); }

  //! Helper to check for extension string presence.  Adapted from:
  //!   http://www.opengl.org/resources/features/OGLextensions/
  static bool extension_supported(const char *extList, const char *extension);

  static int ctxErrorHandlerStatic(Display *dpy, XErrorEvent *ev);
  int        ctxErrorHandler(XErrorEvent *ev);
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
