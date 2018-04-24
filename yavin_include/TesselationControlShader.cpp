#include "TesselationControlShader.h"

#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

TesselationControlShader::TesselationControlShader(const std::string& filepath)
    : ShaderStage(GL_TESS_CONTROL_SHADER, filepath) {}

TesselationControlShader::TesselationControlShader(
    TesselationControlShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace Yavin
//==============================================================================
