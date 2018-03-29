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
#include "error_check.h"
#include "dll_export.h"

#include "windows_undefines.h"

namespace Yavin {

class Shader {
 public:
  DLL_API Shader();
  DLL_API ~Shader();

  template <typename T, typename... Args>
  void add_shader_stage(Args... args) {
	  m_shader_stages.push_back(T(args...));
	  glAttachShader(m_id, m_shader_stages.back().id());
	  gl_error_check("glAttachShader - " + m_shader_stages.back().stage());
	  for (const auto& stage : m_shader_stages)
		  for (const auto& var : stage.glsl_vars()) {
			  if (var.modifier == GLSLVar::UNIFORM)
				  m_uniform_var_names.insert(var.name);
			  else if (std::is_same<VertexShader, T>::value && var.modifier == GLSLVar::IN)
				  m_attribute_var_names.insert(var.name);
		  }
  }
  DLL_API void create();

  DLL_API virtual void bind();
  DLL_API virtual void unbind();
  DLL_API void add_uniform(const std::string& uniformVarName);
  DLL_API void add_attribute(const std::string& attributeVarName);
  DLL_API GLint uniform(const std::string& uniformVarName);
  DLL_API GLint attribute(const std::string& attributeVarName);

  DLL_API void set_uniform(const std::string& name, float value);
  DLL_API void set_uniform(const std::string& name, int value);
  DLL_API void set_uniform(const std::string& name, const glm::mat4x4& value);
  DLL_API void set_uniform(const std::string& name, const glm::mat3x3& value);
  DLL_API void set_uniform(const std::string& name, const glm::mat2x2& value);
  DLL_API void set_uniform(const std::string& name, const glm::vec4& value);
  DLL_API void set_uniform(const std::string& name, const glm::vec3& value);
  DLL_API void set_uniform(const std::string& name, const glm::vec2& value);

  DLL_API void info_log();

 private:
  std::map<std::string, GLint> m_uniform_locations;
  std::map<std::string, GLint> m_attribute_locations;
  std::vector<Yavin::ShaderStage> m_shader_stages;
  GLuint                          m_id;
  std::unordered_set<std::string> m_uniform_var_names;
  std::unordered_set<std::string> m_attribute_var_names;
};

}  // namespace Yavin

#endif