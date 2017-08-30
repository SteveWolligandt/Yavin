#include "GeometryShader.h"

#include "gl_includes.h"

namespace Yavin {
GeometryShader::GeometryShader(const std::string& filepath)
    : Yavin::ShaderStage(GL_GEOMETRY_SHADER, filepath) {}

GeometryShader::GeometryShader(GeometryShader&& other) : ShaderStage(std::move(other)) {}
}  // namespace Yavin