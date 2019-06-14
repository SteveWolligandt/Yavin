#ifndef YAVIN_GLFW_CONTEXT
#define YAVIN_GLFW_CONTEXT

#include <string>
#include "dllexport.h"
#include <yavin/glfw_window.h>
#include <GLFW/glfw3.h>

//==============================================================================
namespace yavin {
//==============================================================================

class glfw_context {
 public:
  DLL_API glfw_context(unsigned int major = 4, unsigned int minor = 5,
                       unsigned int width = 1, unsigned int height = 1);
  DLL_API glfw_context(const glfw_window& w, unsigned int major = 4,
                       unsigned int minor = 5, unsigned int width = 1,
                       unsigned int height = 1);
  DLL_API ~glfw_context();
  DLL_API void make_current();

 private:
  GLFWwindow*  m_window;
};

//==============================================================================
}
//==============================================================================
#endif
