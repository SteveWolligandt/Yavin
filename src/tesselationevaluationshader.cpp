#include <yavin/tesselationevaluationshader.h>

#include <yavin/glincludes.h>

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
