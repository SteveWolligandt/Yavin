#ifndef YAVIN_VEC_H
#define YAVIN_VEC_H
//==============================================================================
#include <algorithm>
#include <cmath>
#include <ostream>
#include <random>
#include <vector>
#include "glincludes.h"
#include "gltype.h"
#include "utility.h"
//==============================================================================
namespace yavin {
//==============================================================================
template <typename Real, size_t N>
struct vec {
 public:
  //===========================================================================
  static constexpr auto num_components() { return N; }
  using this_t                 = vec<Real, N>;
  using data_container_t       = std::array<Real, N>;
  using iterator               = typename data_container_t::iterator;
  using const_iterator         = typename data_container_t::const_iterator;
  static constexpr GLenum type = gl_type<Real>::type;

 private:
  //===========================================================================
  data_container_t m_data;

 public:
  //===========================================================================
  /// copy constructor
  constexpr vec(const vec& other) = default;
  //----------------------------------------------------------------------------
  /// move constructor
  constexpr vec(vec&& other) = default;
  //----------------------------------------------------------------------------
  /// copy assignment
  constexpr vec& operator=(const vec& other) = default;
  //----------------------------------------------------------------------------
  /// move assignment
  constexpr vec& operator=(vec&& other) = default;
  //----------------------------------------------------------------------------
  /// creates a vector with specified components in initializer list
  template <
      typename... Components,
      std::enable_if_t<(std::is_arithmetic_v<Components> && ...), bool> = true>
  constexpr vec(Components... cs) : m_data{static_cast<Real>(cs)...} {
    static_assert((std::is_arithmetic_v<Components> && ...),
                  "aritmetic values are needed");
  }

 private:
  //----------------------------------------------------------------------------
  /// creates empty vector
  explicit constexpr vec(Real c) : m_data{make_array<Real, N>(c)} {}

 public:
  //----------------------------------------------------------------------------
  /// creates a vector filled with zeros
  constexpr static auto zeros() { return vec{0.0}; }
  //----------------------------------------------------------------------------
  /// creates a vector filled with ones
  constexpr static auto ones() { return vec{1.0}; }
  //----------------------------------------------------------------------------
  /// creates a vector filled with random values between min and max
  template <typename RandomEngine = std::mt19937_64>
  static auto randu(Real min = 0, Real max = 1,
                    RandomEngine&& e = RandomEngine{
                        std::random_device{}()}) {
    this_t                               v;
    std::uniform_real_distribution<Real> dist{min, max};
    std::generate(v.begin(), v.end(), [&] { return dist(e); });
    return v;
  }
  //----------------------------------------------------------------------------
  /// adds another vector
  constexpr vec& operator+=(const vec& other) {
    std::transform(begin(), end(), other.begin(), begin(),
                   [](auto l, auto r) { return l + r; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// adds scalar to every component
  constexpr vec& operator+=(double add) {
    std::transform(begin(), end(), begin(),
                   [add](auto c) { return c + add; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// subtracts another vector
  constexpr vec& operator-=(const vec& other) {
    std::transform(m_data.begin(), m_data.end(), other.m_data.begin(),
                   m_data.begin(), [](auto l, auto r) { return l - r; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// subtracts scalar to every component
  constexpr vec& operator-=(double add) {
    std::transform(begin(), end(), begin(),
                   [add](auto c) { return c - add; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// scales vector
  constexpr vec& operator*=(vec other) {
    std::transform(begin(), end(), other.begin(), begin(),
                   [](auto l, auto r) { return l * r; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// inverse-scales vector
  constexpr vec& operator/=(vec other) {
    std::transform(begin(), end(), other.begin(), begin(),
                   [](auto l, auto r) { return l / r; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// scales vector
  constexpr vec& operator*=(Real scale) {
    std::transform(begin(), end(), begin(),
                   [scale](auto c) { return c * scale; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// inverse-scales vector
  constexpr vec& operator/=(Real scale) {
    std::transform(begin(), end(), begin(),
                   [scale](auto c) { return c / scale; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// access to component at position i
  constexpr Real& at(size_t i) { return m_data[i]; }
  //----------------------------------------------------------------------------
  /// access to component at position i
  constexpr Real at(size_t i) const { return m_data[i]; }
  //----------------------------------------------------------------------------
  /// access to component at position i
  constexpr Real& operator()(size_t i) { return at(i); }
  //----------------------------------------------------------------------------
  /// access to component at position i
  constexpr Real operator()(size_t i) const { return at(i); }
  //----------------------------------------------------------------------------
  constexpr Real sqr_length() const {
    Real l = 0.0;
    for (auto c : m_data) { l += c * c; }
    return l;
  }
  //----------------------------------------------------------------------------
  constexpr Real length() const { return std::sqrt(sqr_length()); }
  //----------------------------------------------------------------------------
  constexpr auto begin() { return m_data.begin(); }
  constexpr auto begin() const { return m_data.begin(); }
  constexpr auto end() { return m_data.end(); }
  constexpr auto end() const { return m_data.end(); }
  //----------------------------------------------------------------------------
  /// returns reference to data block
  constexpr auto& data() { return m_data; }
  //----------------------------------------------------------------------------
  constexpr Real* data_ptr() { return m_data.data(); }
  constexpr const Real* data_ptr() const { return m_data.data(); }
};
//==============================================================================
/// deduction guide gets matrix dimensions when creating object
template <typename... Ts>
vec(Ts...)->vec<promote_t<Ts...>, sizeof...(Ts)>;
//==============================================================================
/// dot product of two vectors
template <typename Real, size_t N>
constexpr auto dot(const vec<Real, N>& v0, const vec<Real, N>& v1) {
  Real dp = 0;
  for (size_t i = 0; i < N; ++i) { dp += v0(i) * v1(i); }
  return dp;
}
//------------------------------------------------------------------------------
/// euclidean distance between two points of two vectors
template <typename Real, size_t N>
constexpr auto distance(const vec<Real, N>& v0, const vec<Real, N>& v1) {
  auto distance_vec = v1 - v0;
  return std::sqrt(dot(distance_vec, distance_vec));
}
//------------------------------------------------------------------------------
/// cross product of two vectors
template <typename Real>
constexpr inline vec<Real, 3> cross(const vec<Real, 3>& v0,
                                    const vec<Real, 3>& v1) {
  return {v0(1) * v1(2) - v0(2) * v1(1),
          v0(2) * v1(0) - v0(0) * v1(2),
          v0(0) * v1(1) - v0(1) * v1(0)};
}
//------------------------------------------------------------------------------
/// returns a normalized copy of v
template <typename Real, size_t N>
constexpr auto normalize(const vec<Real, N>& v) {
  auto n = v;
  const auto l = n.length();
  for (auto& c : n) { c /= l; }
  return n;
}
//------------------------------------------------------------------------------
/// component-wise addition of two vectors
template <typename Real, size_t N>
constexpr auto operator+(const vec<Real, N>& v0, const vec<Real, N>& v1) {
  vec<Real, N> v{v0};
  return v += v1;
}
//------------------------------------------------------------------------------
/// addition of vector and scalar
template <typename Real, size_t N>
constexpr auto operator+(const vec<Real, N>& v0, double scalar) {
  vec<Real, N> v{v0};
  return v += scalar;
}
//------------------------------------------------------------------------------
/// addition of scalar and vector
template <typename Real, size_t N>
constexpr auto operator+(double scalar, const vec<Real, N>& v0) {
  vec<Real, N> v{v0};
  return v += scalar;
}
//------------------------------------------------------------------------------
/// component-wise subtraction of two vectors
template <typename Real, size_t N>
constexpr auto operator-(const vec<Real, N>& v0, const vec<Real, N>& v1) {
  vec<Real, N> v{v0};
  return v -= v1;
}
//------------------------------------------------------------------------------
/// addition of vector and scalar
template <typename Real, size_t N>
constexpr auto operator-(const vec<Real, N>& v0, double scalar) {
  vec<Real, N> v{v0};
  return v -= scalar;
}
//------------------------------------------------------------------------------
/// addition of scalar and vector
template <typename Real, size_t N>
constexpr auto operator-(double scalar, const vec<Real, N>& v0) {
  vec<Real, N> v{v0};
  return v -= scalar;
}
//------------------------------------------------------------------------------
/// negation of a vector
template <typename Real, size_t N>
constexpr auto operator-(const vec<Real, N>& v) {
  vec<Real, N> neg{v};
  for (auto& c : neg) c *= -1;
  return neg;
}
//------------------------------------------------------------------------------
/// component-wise mutliplication of two vectors
template <typename Real, size_t N, size_t M>
constexpr auto operator*(const vec<Real, N>& lhs, const vec<Real, M>& rhs) {
  static_assert(N == M, "vector dimensions do not match");
  vec<Real, N> v{lhs};
  return v *= rhs;
}
//------------------------------------------------------------------------------
/// component-wise division of two vectors
template <typename Real, size_t N, size_t M>
constexpr auto operator/(const vec<Real, N>& lhs, const vec<Real, M>& rhs) {
  static_assert(N == M, "vector dimensions do not match");
  vec<Real, N> v{lhs};
  return v /= rhs;
}
//------------------------------------------------------------------------------
/// scaling of a vector
template <typename Real, size_t N>
constexpr vec<Real, N> operator*(const vec<Real, N>& v, Real s) {
  vec<Real, N> scaled{v};
  return scaled *= s;
}
//------------------------------------------------------------------------------
/// scaling of a vector
template <typename Real, size_t N>
constexpr auto operator*(Real s, const vec<Real, N>& v) {
  vec<Real, N> scaled{v};
  return scaled *= s;
}
//------------------------------------------------------------------------------
/// inverse scaling of a vector
template <typename Real, size_t N>
constexpr auto operator/(const vec<Real, N>& v, Real s) {
  vec<Real, N> scaled{v};
  return scaled /= s;
}
//------------------------------------------------------------------------------
/// inverse scaling of a vector
template <typename Real, size_t N>
constexpr auto operator/(Real s, const vec<Real, N>& v) {
  vec<Real, N> scaled{v};
  return scaled /= s;
}
//------------------------------------------------------------------------------
/// printing a vector into a stream
template <typename Real, size_t N>
auto& operator<<(std::ostream& str, const vec<Real, N>& v) {
  for (auto c : v) str << "[ " << c << " ]\n";
  return str;
}
//==============================================================================
template <typename Real>
using Scalar = vec<Real, 1>;
template <typename Real>
using Vec2 = vec<Real, 2>;
template <typename Real>
using Vec3 = vec<Real, 3>;
template <typename Real>
using Vec4 = vec<Real, 4>;
//------------------------------------------------------------------------------ 
using scalar = vec<float, 1>;
using vec2   = vec<float, 2>;
using vec3   = vec<float, 3>;
using vec4   = vec<float, 4>;
//------------------------------------------------------------------------------
using dscalar = vec<double, 1>;
using dvec2   = vec<double, 2>;
using dvec3   = vec<double, 3>;
using dvec4   = vec<double, 4>;
//------------------------------------------------------------------------------
using i32scalar = vec<int32_t, 1>;
using i32vec2   = vec<int32_t, 2>;
using i32vec3   = vec<int32_t, 3>;
using i32vec4   = vec<int32_t, 4>;
//------------------------------------------------------------------------------
using ui32scalar = vec<uint32_t, 1>;
using ui32vec2   = vec<uint32_t, 2>;
using ui32vec3   = vec<uint32_t, 3>;
using ui32vec4   = vec<uint32_t, 4>;
//------------------------------------------------------------------------------
using i16scalar = vec<int16_t, 1>;
using i16vec2   = vec<int16_t, 2>;
using i16vec3   = vec<int16_t, 3>;
using i16vec4   = vec<int16_t, 4>;
//------------------------------------------------------------------------------
using ui16scalar = vec<uint16_t, 1>;
using ui16vec2   = vec<uint16_t, 2>;
using ui16vec3   = vec<uint16_t, 3>;
using ui16vec4   = vec<uint16_t, 4>;
//------------------------------------------------------------------------------
using i8scalar = vec<int8_t, 1>;
using i8vec2   = vec<int8_t, 2>;
using i8vec3   = vec<int8_t, 3>;
using i8vec4   = vec<int8_t, 4>;
//------------------------------------------------------------------------------
using ui8scalar = vec<uint8_t, 1>;
using ui8vec2   = vec<uint8_t, 2>;
using ui8vec3   = vec<uint8_t, 3>;
using ui8vec4   = vec<uint8_t, 4>;
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
