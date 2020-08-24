#include <yavin/glincludes.h>
#include <yavin/fragmentshader.h>
//==============================================================================
namespace yavin {
//==============================================================================
fragmentshader::fragmentshader(const std::string& filepath,
                               shadersourcetype         string_type)
    : shaderstage(GL_FRAGMENT_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
fragmentshader::fragmentshader(fragmentshader&& other)
    : shaderstage(std::move(other)) {}
//==============================================================================
}  // namespace yavin
//==============================================================================
