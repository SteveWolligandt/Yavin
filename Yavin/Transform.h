#ifndef __YAVIN_TRANSFORM_H__
#define __YAVIN_TRANSFORM_H__

#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Yavin {
class Transform {
 public:
  Transform() : m_matrix(glm::mat4()) {}
  virtual ~Transform() {}

  glm::mat4x4& matrix() { return m_matrix; }

  const glm::mat4x4& matrix() const { return m_matrix; }

  void look_at(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
    m_matrix = glm::inverse(glm::lookAt(eye, center, up));
  }

  /**
   * @brief      Pitch should be in the range of [-90 ... 90] degrees and yaw
   * should be in the range of [0 ... 360] degrees.
   *
   * @param[in]  eye    The eye
   * @param[in]  pitch  The pitch
   * @param[in]  yaw    The yaw
   */
  void first_person(const glm::vec3& eye, float pitch, float yaw) {
    m_matrix[3]          = glm::vec4(eye, 1);
    m_first_person_pitch = pitch;
    m_first_person_yaw   = yaw;
    first_person();
  }

  void first_person() {
    auto  eye      = translation();
    float cosPitch = cos(m_first_person_pitch * M_PI / 180);
    float sinPitch = sin(m_first_person_pitch * M_PI / 180);
    float cosYaw   = cos(m_first_person_yaw * M_PI / 180);
    float sinYaw   = sin(m_first_person_yaw * M_PI / 180);

    glm::vec3 xaxis = {cosYaw, 0, -sinYaw};
    glm::vec3 yaxis = {sinYaw * sinPitch, cosPitch, cosYaw * sinPitch};
    glm::vec3 zaxis = {sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw};

    m_matrix = glm::inverse(
        glm::mat4{glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0), glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0),
                  glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0),
                  glm::vec4(-dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1)});
  }

  void translate(const glm::vec3& translation) { m_matrix = glm::translate(m_matrix, translation); }
  void translate(const float x, const float y, const float z) { translate(glm::vec3(x, y, z)); }
  glm::vec3 translation() { return glm::vec3(glm::vec3(m_matrix[3])); }

  void scale(const glm::vec3& scale) { m_matrix = glm::scale(m_matrix, scale); }
  void scale(const float x, const float y, const float z) { scale(glm::vec3(x, y, z)); }
  void scale(const float s) { scale(glm::vec3(s, s, s)); }

  void rotate(const float angle, const glm::vec3& axis) {
    m_matrix = glm::rotate(m_matrix, angle, axis);
  }
  void rotate(const float angle, const float axisX, const float axisY, const float axisZ) {
    rotate(angle, glm::vec3(axisX, axisY, axisZ));
  }

  void inc_pitch(float amount) {
    m_first_person_pitch += amount;
    m_first_person_pitch = std::fmin(m_first_person_pitch, 90.0f);
    m_first_person_pitch = std::fmax(m_first_person_pitch, -90.0f);
    first_person();
  }
  void dec_pitch(float amount) {
    inc_pitch(-amount);
    first_person();
  }
  void inc_yaw(float amount) {
    m_first_person_yaw += amount;
    first_person();
  }
  void dec_yaw(float amount) {
    inc_yaw(-amount);
    first_person();
  }

 protected:
  glm::mat4x4 m_matrix;

  float m_first_person_pitch;
  float m_first_person_yaw;
};
}  // namespace Yavin

#endif