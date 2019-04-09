#include <yavin/tesselationevaluationshader.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

tesselationevaluationshader::tesselationevaluationshader(
    const std::string& filepath)
    : shaderstage(GL_TESS_EVALUATION_SHADER, filepath) {}

tesselationevaluationshader::tesselationevaluationshader(
    tesselationevaluationshader&& other)
    : shaderstage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
