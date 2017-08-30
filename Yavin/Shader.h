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

namespace Yavin {

class Shader {
 public:
  Shader();
  ~Shader();

  template <typename T, typename... Args>
  void add_shader_stage(Args...);
  void create();

  virtual void bind();
  virtual void unbind();
  void add_uniform(const std::string& uniformVarName);
  void add_attribute(const std::string& attributeVarName);
  GLint uniform(const std::string& uniformVarName);
  GLint attribute(const std::string& attributeVarName);

  void set_uniform(const std::string& name, float value);
  void set_uniform(const std::string& name, int value);
  void set_uniform(const std::string& name, const glm::mat4x4& value);
  void set_uniform(const std::string& name, const glm::mat3x3& value);
  void set_uniform(const std::string& name, const glm::mat2x2& value);
  void set_uniform(const std::string& name, const glm::vec4& value);
  void set_uniform(const std::string& name, const glm::vec3& value);
  void set_uniform(const std::string& name, const glm::vec2& value);

  void info_log();

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