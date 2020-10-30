#include <yavin/geometryshader.h>

#include <yavin/glincludes.h>
//==============================================================================
namespace yavin {
//==============================================================================
geometryshader::geometryshader(std::string const& filepath,
                           shadersourcetype         string_type)
    : shaderstage{GL_GEOMETRY_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
geometryshader::geometryshader(std::string_view const& filepath,
                           shadersourcetype         string_type)
    : shaderstage{GL_GEOMETRY_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
geometryshader::geometryshader(char const*      filepath,
                               shadersourcetype string_type)
    : shaderstage{GL_GEOMETRY_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
geometryshader::geometryshader(geometryshader&& other)
    : shaderstage{std::move(other)} {}
//==============================================================================
}  // namespace yavin
//==============================================================================
