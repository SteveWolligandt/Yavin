#ifndef YAVIN_TRANSFORM_H
#define YAVIN_TRANSFORM_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include "vec.h"
#include "mat.h"

//==============================================================================
namespace yavin {
//==============================================================================
class transform {
 public:
  using vec3 = vec<float, 3>;
  using vec4 = vec<float, 4>;
  using mat3 = mat<float, 3, 3>;
  using mat4 = mat<float, 4, 4>;
 
 protected:
  mat4  m_matrix;
  float m_first_person_pitch;
  float m_first_person_yaw;

 public:
  transform() : m_matrix{mat4::eye()} {}
  transform(const transform& other)
      : m_matrix(other.m_matrix),
        m_first_person_pitch(other.m_first_person_pitch),
        m_first_person_yaw(other.m_first_person_yaw) {}
  transform(transform&& other)
      : m_matrix(std::move(other.m_matrix)),
        m_first_person_pitch(other.m_first_person_pitch),
        m_first_person_yaw(other.m_first_person_yaw) {}
  virtual ~transform() {}

  mat4& matrix() { return m_matrix; }

  const mat4& matrix() const { return m_matrix; }

  void look_at(const vec3& eye, const vec3& center,
               const vec3& up = {0, 1, 0}) {
    m_matrix = *inverse(look_at_matrix(eye, center, up));
  }

  /// \brief      Pitch should be in the range of [-90 ... 90] degrees and yaw
  /// should be in the range of [0 ... 360] degrees.
  ///
  /// \param[in]  eye    The eye
  /// \param[in]  pitch  The pitch
  /// \param[in]  yaw    The yaw
  void first_person(const vec3& eye, float pitch, float yaw) {
    m_matrix(0, 3)       = eye(0);
    m_matrix(1, 3)       = eye(1);
    m_matrix(2, 3)       = eye(2);
    m_matrix(3, 3)       = 1;
    m_first_person_pitch = pitch;
    m_first_person_yaw   = yaw;
    first_person();
  }

  void first_person() {
    auto  eye      = vec3(translation());
    float cosPitch = cos(m_first_person_pitch * float(M_PI) / 180.0f);
    float sinPitch = sin(m_first_person_pitch * float(M_PI) / 180.0f);
    float cosYaw   = cos(m_first_person_yaw * float(M_PI) / 180.0f);
    float sinYaw   = sin(m_first_person_yaw * float(M_PI) / 180.0f);

    vec3 xaxis = {cosYaw, 0, -sinYaw};
    vec3 yaxis = {sinYaw * sinPitch, cosPitch, cosYaw * sinPitch};
    vec3 zaxis = {sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw};

    m_matrix = *inverse(mat4{{xaxis(0), xaxis(1), xaxis(2), -dot(xaxis, eye)},
                             {yaxis(0), yaxis(1), yaxis(2), -dot(yaxis, eye)},
                             {zaxis(0), zaxis(1), zaxis(2), -dot(zaxis, eye)},
                             {0, 0, 0, 1}});
  }

  void translate(const vec3& t) {
    m_matrix = m_matrix * translation_matrix(t);
  }
  void translate(const float x, const float y, const float z) {
    m_matrix = m_matrix * translation_matrix(x,y,z);
  }
  vec3 translation() {
    return {m_matrix(0, 3), m_matrix(1, 3), m_matrix(2, 3)};
  }

  void scale(const vec3& s) {
    m_matrix = m_matrix * scale_matrix(s);
  }
  void scale(const float x, const float y, const float z) {
    m_matrix = m_matrix * scale_matrix(x, y, z);
  }
  void scale(const float s) {
    m_matrix = m_matrix * scale_matrix(s);
  }

  void rotate(const float angle, const vec3& axis) {
    m_matrix = m_matrix * rotation_matrix(angle, axis);
  }
  void rotate(const float angle, const float axisX, const float axisY,
              const float axisZ) {
    rotate(angle, vec3(axisX, axisY, axisZ));
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
};
//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
