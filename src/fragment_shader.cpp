#include <yavin/fragment_shader.h>

#include <yavin/gl_includes.h>

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
