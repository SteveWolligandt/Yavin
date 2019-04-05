#include <yavin/tesselation_evaluation_shader.h>

#include <yavin/gl_includes.h>

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
