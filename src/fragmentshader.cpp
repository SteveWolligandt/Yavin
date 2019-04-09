#include <yavin/fragmentshader.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

fragmentshader::fragmentshader(const std::string& filepath)
    : shaderstage(GL_FRAGMENT_SHADER, filepath) {}

fragmentshader::fragmentshader(fragmentshader&& other)
    : shaderstage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
