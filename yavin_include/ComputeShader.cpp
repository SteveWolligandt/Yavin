#include "ComputeShader.h"

#include "gl_includes.h"

namespace Yavin {
ComputeShader::ComputeShader(const std::string& filepath)
    : Yavin::ShaderStage(GL_COMPUTE_SHADER, filepath) {}

ComputeShader::ComputeShader(ComputeShader&& other) : ShaderStage(std::move(other)) {}
}  // namespace Yavin