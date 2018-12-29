#include "compute_shader.h"

#include "gl_includes.h"

//==============================================================================
namespace yavin {
//==============================================================================

ComputeShader::ComputeShader(const std::string& filepath)
    : yavin::ShaderStage(GL_COMPUTE_SHADER, filepath) {}

ComputeShader::ComputeShader(ComputeShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
