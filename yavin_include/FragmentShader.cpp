#include "FragmentShader.h"

#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

FragmentShader::FragmentShader(const std::string& filepath)
    : Yavin::ShaderStage(GL_FRAGMENT_SHADER, filepath) {}

FragmentShader::FragmentShader(FragmentShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace Yavin
//==============================================================================
