#include <yavin/geometryshader.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

geometryshader::geometryshader(const std::string& filepath,
                           shadersourcetype         string_type)
    : shaderstage{GL_GEOMETRY_SHADER, filepath, string_type} {}

geometryshader::geometryshader(geometryshader&& other)
    : shaderstage{std::move(other)} {}

//==============================================================================
}  // namespace yavin
//==============================================================================
