#include "geometry_shader.h"

#include "gl_includes.h"

//==============================================================================
namespace yavin {
//==============================================================================

GeometryShader::GeometryShader(const std::string& filepath)
    : yavin::ShaderStage(GL_GEOMETRY_SHADER, filepath) {}

GeometryShader::GeometryShader(GeometryShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
