#include <yavin/vertexshader.h>

//==============================================================================
namespace yavin {
//==============================================================================

vertexshader::vertexshader(const std::string& filepath,
                           StringType         string_type)
    : shaderstage{GL_VERTEX_SHADER, filepath, string_type} {}

vertexshader::vertexshader(vertexshader&& other)
    : shaderstage{std::move(other)} {}

//==============================================================================
}  // namespace yavin
//==============================================================================
