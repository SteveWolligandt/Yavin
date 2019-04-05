#include <yavin/geometry_shader.h>

#include <yavin/gl_includes.h>

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
