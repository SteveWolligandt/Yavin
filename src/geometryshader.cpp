#include <yavin/geometryshader.h>

#include <yavin/glincludes.h>

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
