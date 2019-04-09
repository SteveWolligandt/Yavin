#include <yavin/vertexshader.h>

//==============================================================================
namespace yavin {
//==============================================================================

vertexshader::vertexshader(const std::string& filepath)
    : shaderstage(GL_VERTEX_SHADER, filepath) {}

vertexshader::vertexshader(vertexshader&& other)
    : shaderstage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
