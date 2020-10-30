#include <yavin/glincludes.h>
#include <yavin/computeshader.h>
//==============================================================================
namespace yavin {
//==============================================================================
computeshader::computeshader(std::string const& filepath,
                             shadersourcetype   string_type)
    : shaderstage(GL_COMPUTE_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
computeshader::computeshader(std::string_view const& filepath,
                             shadersourcetype        string_type)
    : shaderstage(GL_COMPUTE_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
computeshader::computeshader(char const* filepath, shadersourcetype string_type)
    : shaderstage(GL_COMPUTE_SHADER, filepath, string_type) {}
//------------------------------------------------------------------------------
computeshader::computeshader(computeshader&& other)
    : shaderstage(std::move(other)) {}
//==============================================================================
}  // namespace yavin
//==============================================================================
