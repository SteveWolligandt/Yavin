#include <yavin/vertexshader.h>
//==============================================================================
namespace yavin {
//==============================================================================
vertexshader::vertexshader(std::string const& filepath,
                           shadersourcetype         string_type)
    : shaderstage{GL_VERTEX_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
vertexshader::vertexshader(std::string_view const& filepath,
                           shadersourcetype         string_type)
    : shaderstage{GL_VERTEX_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
vertexshader::vertexshader(char const* filepath, shadersourcetype string_type)
    : shaderstage{GL_VERTEX_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
vertexshader::vertexshader(vertexshader&& other)
    : shaderstage{std::move(other)} {}
//==============================================================================
}  // namespace yavin
//==============================================================================
