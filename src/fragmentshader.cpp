#include <yavin/glincludes.h>
#include <yavin/fragmentshader.h>
//==============================================================================
namespace yavin {
//==============================================================================
fragmentshader::fragmentshader(std::string const& filepath,
                               shadersourcetype         string_type)
    : shaderstage(GL_FRAGMENT_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
fragmentshader::fragmentshader(std::string_view const& filepath,
                               shadersourcetype        string_type)
    : shaderstage(GL_FRAGMENT_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
fragmentshader::fragmentshader(char const*      filepath,
                               shadersourcetype string_type)
    : shaderstage(GL_FRAGMENT_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
fragmentshader::fragmentshader(fragmentshader&& other)
    : shaderstage(std::move(other)) {}
//==============================================================================
}  // namespace yavin
//==============================================================================
