#include <yavin/glincludes.h>
#include <yavin/tesselationcontrolshader.h>
//==============================================================================
namespace yavin {
//==============================================================================
tesselationcontrolshader::tesselationcontrolshader(std::string const& filepath,
                                                   shadersourcetype string_type)
    : shaderstage{GL_TESS_CONTROL_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
tesselationcontrolshader::tesselationcontrolshader(
    std::string_view const& filepath, shadersourcetype string_type)
    : shaderstage{GL_TESS_CONTROL_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
tesselationcontrolshader::tesselationcontrolshader(char const*      filepath,
                                                   shadersourcetype string_type)
    : shaderstage{GL_TESS_CONTROL_SHADER, filepath, string_type} {}
//------------------------------------------------------------------------------
tesselationcontrolshader::tesselationcontrolshader(
    tesselationcontrolshader&& other)
    : shaderstage{std::move(other)} {}
//==============================================================================
}  // namespace yavin
//==============================================================================
