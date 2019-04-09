#include <yavin/tesselationcontrolshader.h>

#include <yavin/glincludes.h>

//==============================================================================
namespace yavin {
//==============================================================================

tesselationcontrolshader::tesselationcontrolshader(const std::string& filepath)
    : shaderstage(GL_TESS_CONTROL_SHADER, filepath) {}

tesselationcontrolshader::tesselationcontrolshader(
    tesselationcontrolshader&& other)
    : shaderstage(std::move(other)) {}

//==============================================================================
}  // namespace yavin
//==============================================================================
