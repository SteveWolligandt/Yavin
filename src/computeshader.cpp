#include <yavin/glincludes.h>
#include <yavin/computeshader.h>
//==============================================================================
namespace yavin {
//==============================================================================
computeshader::computeshader(const std::string& filepath,
                             shadersourcetype   string_type)
    : shaderstage(GL_COMPUTE_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
computeshader::computeshader(computeshader&& other)
    : shaderstage(std::move(other)) {}
//==============================================================================
}  // namespace yavin
//==============================================================================
