#ifndef YAVIN_GLEW_H
#define YAVIN_GLEW_H
//==============================================================================
#include <yavin/glincludes.h>

#include <memory>
#include <stdexcept>
//==============================================================================
namespace yavin {
//==============================================================================
struct glew {
  glew() {
    glewExperimental = true;
    auto err         = glewInit();
    if (GLEW_OK != err) {
      throw std::runtime_error{std::string("cannot initialize GLEW: ") +
                               std::string((char*)glewGetErrorString(err))};
    }
  }
  static auto create() { return std::make_unique<glew>(); }
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
