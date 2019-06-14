#ifndef YAVIN_APIS_H
#define YAVIN_APIS_H

#include <stdexcept>
#include "glincludes.h"

#include <GLFW/glfw3.h>

//==============================================================================
namespace yavin::apis {
//==============================================================================

struct glew {
  static void init() { static glew inst{}; }
  glew() {
    glewExperimental = true;
    GLenum err       = glewInit();
    if (GLEW_OK != err) {
      throw std::runtime_error{
          std::string{"cannot initialize GLEW: "} +
          std::string{reinterpret_cast<const char*>(glewGetErrorString(err))}};
    }
  }
};

//==============================================================================
struct glfw {
  static void init() { static glfw inst{}; }
  glfw() {
    if (!glfwInit()) { throw std::runtime_error{"glfwInit() failed."}; }
  }

  ~glfw() { glfwTerminate(); }
};

//==============================================================================
}  // namespace yavin::apis
//==============================================================================

#endif
