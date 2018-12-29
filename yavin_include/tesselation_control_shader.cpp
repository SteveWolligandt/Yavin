#include "tesselation_control_shader.h"

#include "gl_includes.h"

//==============================================================================
namespace yavin {
//==============================================================================

TesselationControlShader::TesselationControlShader(const std::string& filepath)
    : ShaderStage(GL_TESS_CONTROL_SHADER, filepath) {}

TesselationControlShader::TesselationControlShader(
    TesselationControlShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
