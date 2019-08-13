#include <yavin/fragmentshader.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

fragmentshader::fragmentshader(const std::string& filepath,
                               StringType         string_type)
    : shaderstage(GL_FRAGMENT_SHADER, filepath, string_type) {}

fragmentshader::fragmentshader(fragmentshader&& other)
    : shaderstage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
