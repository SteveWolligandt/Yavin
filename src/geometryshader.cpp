#include <yavin/geometryshader.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

geometryshader::geometryshader(const std::string& filepath)
    : yavin::shaderstage(GL_GEOMETRY_SHADER, filepath) {}

geometryshader::geometryshader(geometryshader&& other)
    : shaderstage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
