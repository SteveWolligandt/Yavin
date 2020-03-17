#ifndef YAVIN_GLX_CONTEXT_H
#define YAVIN_GLX_CONTEXT_H
//==============================================================================
#include <yavin/glincludes.h>
#include <GL/glx.h>
#include "x11.h"

#include <list>
#include <string>
//==============================================================================
namespace yavin {
//==============================================================================
class context {
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig,
                                                       GLXContext, Bool,
                                                       const int *);
  static std::list<context *> contexts;
  static bool                 error_occured;
  static const int            visual_attribs[23];
  Display *                   display;
  Window                      win;
  GLXContext                  ctx;
  Colormap                    cmap;
  bool                        glew_initialized = false;

 public:
  context(int major = 3, int minor = 3);
 private:
  context(int major, int minor, const context &parent);
 public:
  ~context();
  void    make_current();
  void    release();
  void    init_glew();
  context create_shared_context(int major = 3, int minor = 3) const;

 private:
  void setup(int major, int minor, GLXContext parent, bool cur);

 public:
  /// Helper to check for extension string presence. Adapted from:
  /// http://www.opengl.org/resources/features/OGLextensions/
  static bool extension_supported(const char *ext_list, const char *extension);

  static int error_handler_static(Display *dpy, XErrorEvent *ev);
  int        error_handler(XErrorEvent *ev);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#include "x11undefs.h"
#endif
