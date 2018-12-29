#include "tesselation_evaluation_shader.h"

#include "gl_includes.h"

//==============================================================================
namespace yavin {
//==============================================================================

TesselationEvaluationShader::TesselationEvaluationShader(
    const std::string& filepath)
    : ShaderStage(GL_TESS_EVALUATION_SHADER, filepath) {}

TesselationEvaluationShader::TesselationEvaluationShader(
    TesselationEvaluationShader&& other)
    : ShaderStage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
