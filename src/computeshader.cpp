#include <yavin/computeshader.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

computeshader::computeshader(const std::string& filepath)
    : yavin::shaderstage(GL_COMPUTE_SHADER, filepath) {}

computeshader::computeshader(computeshader&& other)
    : shaderstage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
