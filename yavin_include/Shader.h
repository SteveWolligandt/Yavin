#ifndef __YAVIN_SHADERGL_H__
#define __YAVIN_SHADERGL_H__

#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <type_traits>
#include <unordered_set>
#include "ComputeShader.h"
#include "FragmentShader.h"
#include "GeometryShader.h"
#include "TesselationControlShader.h"
#include "TesselationEvaluationShader.h"
#include "VertexShader.h"
#include "attribute.h"
#include "dll_export.h"

#include "windows_undefines.h"

//==============================================================================
namespace Yavin {
//==============================================================================

class Shader {
 public:
  Shader() = default;
  ~Shader() {
    if (m_delete) delete_shader();
  }

  template <typename T, typename... Args>
  void add_shader_stage(Args... args) {
    m_shader_stages.push_back(T(args...));
  }

  DLL_API void create();
  DLL_API void delete_shader();

  DLL_API virtual void bind();
  DLL_API virtual void unbind();
  DLL_API void         add_uniform(const std::string& uniformVarName);
  DLL_API void         add_attribute(const std::string& attributeVarName);
  DLL_API GLint uniform(const std::string& uniformVarName);
  DLL_API GLint attribute(const std::string& attributeVarName);

  DLL_API void set_uniform(const std::string&, const glm::vec2&);
  DLL_API void set_uniform(const std::string&, const glm::vec3&);
  DLL_API void set_uniform(const std::string&, const glm::vec4&);

  DLL_API void set_uniform(const std::string&, const glm::mat2x2&);
  DLL_API void set_uniform(const std::string&, const glm::mat3x3&);
  DLL_API void set_uniform(const std::string&, const glm::mat4x4&);

  DLL_API void set_uniform(const std::string&, float);
  DLL_API void set_uniform(const std::string&, int);
  DLL_API void set_uniform(const std::string&, unsigned int);

  DLL_API void set_uniform(const std::string&, float, float);
  DLL_API void set_uniform(const std::string&, float, float, float);
  DLL_API void set_uniform(const std::string&, float, float, float, float);

  DLL_API void set_uniform(const std::string&, int32_t, int32_t);
  DLL_API void set_uniform(const std::string&, int32_t, int32_t, int32_t);
  DLL_API void set_uniform(const std::string&, int32_t, int32_t, int32_t,
                           int32_t);

  DLL_API void set_uniform(const std::string&, uint32_t, uint32_t);
  DLL_API void set_uniform(const std::string&, uint32_t, uint32_t, uint32_t);
  DLL_API void set_uniform(const std::string&, uint32_t, uint32_t, uint32_t,
                           uint32_t);

  DLL_API void set_uniform(const std::string&, Scalar<float>);
  DLL_API void set_uniform(const std::string&, Scalar<int32_t>);
  DLL_API void set_uniform(const std::string&, Scalar<uint32_t>);

  DLL_API void set_uniform(const std::string&, const Vec2<float>&);
  DLL_API void set_uniform(const std::string&, const Vec3<float>&);
  DLL_API void set_uniform(const std::string&, const Vec4<float>&);

  DLL_API void set_uniform(const std::string&, const Vec2<int32_t>&);
  DLL_API void set_uniform(const std::string&, const Vec3<int32_t>&);
  DLL_API void set_uniform(const std::string&, const Vec4<int32_t>&);

  DLL_API void set_uniform(const std::string&, const Vec2<uint32_t>&);
  DLL_API void set_uniform(const std::string&, const Vec3<uint32_t>&);
  DLL_API void set_uniform(const std::string&, const Vec4<uint32_t>&);

  DLL_API void info_log();

 private:
  std::map<std::string, GLint>    m_uniform_locations;
  std::map<std::string, GLint>    m_attribute_locations;
  std::vector<Yavin::ShaderStage> m_shader_stages;
  GLuint                          m_id = 0;
  std::unordered_set<std::string> m_uniform_var_names;
  std::unordered_set<std::string> m_attribute_var_names;

  bool m_delete = true;
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
