#include <iostream>
#include <cstring>
#include <yavin/context.h>

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092

//==============================================================================
namespace yavin {
//==============================================================================

bool Context::error_occured = false;
const int Context::visual_attribs[23] = {
  GLX_X_RENDERABLE    , True,
  GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
  GLX_RENDER_TYPE     , GLX_RGBA_BIT,
  GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
  GLX_RED_SIZE        , 8,
  GLX_GREEN_SIZE      , 8,
  GLX_BLUE_SIZE       , 8,
  GLX_ALPHA_SIZE      , 8,
  GLX_DEPTH_SIZE      , 24,
  GLX_STENCIL_SIZE    , 8,
  GLX_DOUBLEBUFFER    , True,
  //GLX_SAMPLE_BUFFERS  , 1,
  //GLX_SAMPLES         , 4,
  None
};
std::list<Context*> Context::contexts;

Context::Context(int major, int minor) : display{XOpenDisplay(nullptr)}, ctx{} {
  contexts.push_back(this);
  if (!display) throw std::runtime_error{"Failed to open X display"};

  // FBConfigs were added in GLX version 1.3.
  int glx_major, glx_minor;
  if (!glXQueryVersion(display, &glx_major, &glx_minor) ||
      ((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1))
    throw std::runtime_error{"Invalid GLX version"};

  // std::cout << "Getting matching framebuffer configs\n";
  int          fbcount;
  GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display),
                                       visual_attribs, &fbcount);
  if (!fbc)
    throw std::runtime_error{"Failed to retrieve a framebuffer config\n"};
  // std::cout << "Found " << fbcount << " matching FB configs.\n";

  // Pick the FB config/visual with the most samples per pixel
  // std::cout << "Getting XVisualInfos\n";
  int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

  int i;
  for (i = 0; i < fbcount; ++i) {
    XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i]);
    if (vi) {
      int samp_buf, samples;
      glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
      glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);

      // std::cout << "  Matching fbconfig " << i << ", visual ID 0x"
      //           << vi->visualid << ": SAMPLE_BUFFERS = " << samp_buf
      //           << ", SAMPLES = " << samples << "\n";

      if (best_fbc < 0 || (samp_buf && samples > best_num_samp))
        best_fbc = i, best_num_samp = samples;
      if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
        worst_fbc = i, worst_num_samp = samples;
    }
    XFree(vi);
  }

  GLXFBConfig bestFbc = fbc[best_fbc];

  // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
  XFree(fbc);

  // Get a visual
  XVisualInfo *vi = glXGetVisualFromFBConfig(display, bestFbc);
  // std::cout << "Chosen visual ID = 0x" << vi->visualid << "\n";

  // std::cout << "Creating colormap\n";
  XSetWindowAttributes swa;
  swa.colormap = cmap = XCreateColormap(
      display, RootWindow(display, vi->screen), vi->visual, AllocNone);
  swa.background_pixmap = None;
  swa.border_pixel      = 0;
  swa.event_mask        = StructureNotifyMask;

  // std::cout << "Creating window\n";
  win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 1,
                      1, 0, vi->depth, InputOutput, vi->visual,
                      CWBorderPixel | CWColormap | CWEventMask, &swa);
  if (!win) throw std::runtime_error{"Failed to create window."};

  // Done with the visual info data
  XFree(vi);
  // XStoreName(display, win, name.c_str());
  // XMapWindow(display, win);

  // Get the default screen's GLX extension list
  const char *glxExts =
      glXQueryExtensionsString(display, DefaultScreen(display));

  // NOTE: It is not necessary to create or make current to a context before
  // calling glXGetProcAddressARB
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
  glXCreateContextAttribsARB =
      (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
          (const GLubyte *)"glXCreateContextAttribsARB");

  // Install an X error handler so the application won't exit if GL 3.0
  // context allocation fails.
  //
  // Note this error handler is global.  All display connections in all threads
  // of a process use the same error handler, so be sure to guard against other
  // threads issuing X commands while this code is running.
  int (*oldHandler)(Display *, XErrorEvent *) =
      XSetErrorHandler(&ctxErrorHandlerStatic);

  // Check for the GLX_ARB_create_context extension string and the function.
  // If either is not present, use GLX 1.3 context creation method.
  if (!extension_supported(glxExts, "GLX_ARB_create_context") ||
      !glXCreateContextAttribsARB) {
    std::cout << "glXCreateContextAttribsARB() not found"
                 " ... using old-style GLX context\n";
    ctx = glXCreateNewContext(display, bestFbc, GLX_RGBA_TYPE, 0, True);
  }

  else {
    int context_attribs[] = {GLX_CONTEXT_MAJOR_VERSION_ARB, major,
                             GLX_CONTEXT_MINOR_VERSION_ARB, minor,
                             // GLX_CONTEXT_FLAGS_ARB        ,
                             // GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                             None};

    ctx =
        glXCreateContextAttribsARB(display, bestFbc, 0, True, context_attribs);

    // Sync to ensure any errors generated are processed.
    XSync(display, False);
    if (!error_occured && ctx) {
      std::cout << "Created GL " << major << "." << minor << " context\n";
      make_current();

      glewExperimental = true;
      GLenum err       = glewInit();
      if (GLEW_OK != err)
        throw std::runtime_error{std::string("cannot initialize GLEW: ") +
                                 std::string((char *)glewGetErrorString(err))};
    } else {
      // Couldn't create GL 3.0+ context.  Fall back to old-style 2.x context.
      // When a context version below 3.0+ is requested, implementations will
      // return the newest context version compatible with OpenGL versions less
      // than version 3.0+.
      // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
      context_attribs[1] = 1;
      // GLX_CONTEXT_MINOR_VERSION_ARB = 0
      context_attribs[3] = 0;

      error_occured = false;

      std::cout << "Failed to create GL " << major << "." << minor
                << " context ... using old-style GLX context\n";
      ctx = glXCreateContextAttribsARB(display, bestFbc, 0, True,
                                       context_attribs);
    }
  }

  // Sync to ensure any errors generated are processed.
  XSync(display, False);

  // Restore the original error handler
  XSetErrorHandler(oldHandler);

  if (error_occured || !ctx)
    throw std::runtime_error{"Failed to create an OpenGL context"};
}

//------------------------------------------------------------------------------
Context::~Context() {
  glXMakeCurrent(display, 0, 0);
  glXDestroyContext(display, ctx);

  // XDestroyWindow(display, win);
  XFreeColormap(display, cmap);
  XCloseDisplay(display);
  contexts.remove(this);
}

//------------------------------------------------------------------------------
bool Context::extension_supported(const char *extList, const char *extension) {
  const char *start;
  const char *where, *terminator;

  // Extension names should not have spaces.
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  // It takes a bit of care to be fool-proof about parsing the OpenGL extensions
  // string. Don't be fooled by sub-strings, etc.
  for (start=extList;;) {
    where = strstr(start, extension);
    if (!where) break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0') return true;
    start = terminator;
  }
  return false;
}

//------------------------------------------------------------------------------
int Context::ctxErrorHandlerStatic(Display *display, XErrorEvent * ev) {
  for (auto ctx : contexts)
    if (ctx->display == display)
      return ctx->ctxErrorHandler(ev);
  return 0;
}

int Context::ctxErrorHandler(XErrorEvent * /*ev*/) {
  error_occured = true;
  return 0;
}
//==============================================================================
} // namespace yavin
//==============================================================================
