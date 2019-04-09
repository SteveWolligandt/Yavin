#include <yavin/fragmentshader.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

FragmentShader::FragmentShader(const std::string& filepath)
    : yavin::ShaderStage(GL_FRAGMENT_SHADER, filepath) {}

FragmentShader::FragmentShader(FragmentShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
