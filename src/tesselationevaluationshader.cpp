#include <yavin/glincludes.h>
#include <yavin/tesselationevaluationshader.h>
//==============================================================================
namespace yavin {
//==============================================================================
tesselationevaluationshader::tesselationevaluationshader(
    std::string const& filepath, shadersourcetype string_type)
    : shaderstage(GL_TESS_EVALUATION_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
tesselationevaluationshader::tesselationevaluationshader(
    std::string_view const& filepath, shadersourcetype string_type)
    : shaderstage(GL_TESS_EVALUATION_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
tesselationevaluationshader::tesselationevaluationshader(
    char const* filepath, shadersourcetype string_type)
    : shaderstage(GL_TESS_EVALUATION_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
tesselationevaluationshader::tesselationevaluationshader(
    tesselationevaluationshader&& other)
    : shaderstage(std::move(other)) {}
//==============================================================================
}  // namespace yavin
//==============================================================================
