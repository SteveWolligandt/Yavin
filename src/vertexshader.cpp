#include <yavin/vertexshader.h>

//==============================================================================
namespace yavin {
//==============================================================================

VertexShader::VertexShader(const std::string& filepath)
    : ShaderStage(GL_VERTEX_SHADER, filepath) {}

VertexShader::VertexShader(VertexShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
