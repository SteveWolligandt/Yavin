#include "VertexShader.h"

//==============================================================================
namespace Yavin {
//==============================================================================

VertexShader::VertexShader(const std::string& filepath)
    : ShaderStage(GL_VERTEX_SHADER, filepath) {}

VertexShader::VertexShader(VertexShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace Yavin
//==============================================================================
