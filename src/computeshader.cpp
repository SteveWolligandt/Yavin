#include <yavin/computeshader.h>

#include <yavin/glincludes.h>

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
