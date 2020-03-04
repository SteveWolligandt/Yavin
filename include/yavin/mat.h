#ifndef YAVIN_MAT_H
#define YAVIN_MAT_H
//==============================================================================
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <optional>
#include <ostream>
#include <vector>
#include "utility.h"
#include "vec.h"
//==============================================================================
namespace yavin {
//==============================================================================
/// Matrix with column-major memory layout
template <typename Real, size_t R, size_t C>
struct mat {
 public:
  //============================================================================
  static constexpr auto num_rows() { return R; }
  static constexpr auto num_columns() { return C; }
  static constexpr auto num_components() { return R * C; }
  using dimension_t      = std::array<size_t, 2>;
  using data_container_t = std::vector<Real>;
  using data_block_t     = std::array<Real, num_components()>;

 private:
  //============================================================================
  data_block_t m_data;

 public:
  //============================================================================
  /// copy constructor
  constexpr mat(const mat& other) = default;
  //----------------------------------------------------------------------------
  /// move constructor
  constexpr mat(mat&& other) = default;
  //----------------------------------------------------------------------------
  /// copy assignment
  constexpr mat& operator=(const mat& other) = default;
  //----------------------------------------------------------------------------
  /// move assignment
  constexpr mat& operator=(mat&& other) = default;
  //----------------------------------------------------------------------------
  /// taking variadic list of references to c-style arrays representing rows
  template <typename... Rows>
  constexpr mat(Rows const (&... rows)[C]) : m_data{} {
    static_assert(
        sizeof...(rows) == R,
        "number of given rows does not match specified number of rows");

    // lambda inserting row into data block
    auto insert_row = [r = 0UL, this](const auto& row_data) mutable {
      size_t c = 0;
      for (auto v : row_data) at(r, c++) = static_cast<Real>(v);
      ++r;
    };

    // apply insert_row to each row
    for_each(insert_row, rows...);
  }

 private:
  /// constructor that sets all components to c
  constexpr mat(Real c) : m_data{make_array<Real, num_components()>(c)} {}

 public:
  //----------------------------------------------------------------------------
  /// creates a matrix filled with zeros
  constexpr static auto zeros() { return mat{0.0}; }
  //----------------------------------------------------------------------------
  /// creates a matrix filled with ones
  constexpr static auto ones() { return mat{1.0}; }
  //----------------------------------------------------------------------------
  /// creates an identity matrix
  constexpr static auto eye() {
    auto m = zeros();
    for (size_t i = 0; i < std::min(R, C); ++i) m(i, i) = 1;
    return m;
  }
  //============================================================================
  /// adds another matrix
  constexpr auto& operator+=(const mat& other) {
    std::transform(begin(m_data), end(m_data), begin(other.m_data),
                   begin(m_data), [](auto l, auto r) { return l + r; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// subtracts another matrix
  constexpr auto& operator-=(const mat& other) {
    std::transform(begin(m_data), end(m_data), begin(other.m_data),
                   begin(m_data), [](auto l, auto r) { return l - r; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// scales matrix
  constexpr auto& operator*=(Real scale) {
    std::transform(begin(m_data), end(m_data), begin(m_data),
                   [scale](auto c) { return c * scale; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// inverse-scales matrix
  constexpr auto& operator/=(Real scale) {
    std::transform(begin(m_data), end(m_data), begin(m_data),
                   [scale](auto c) { return c / scale; });
    return *this;
  }
  //----------------------------------------------------------------------------
  /// access to component at position [row, col]
  constexpr auto& at(size_t row, size_t col) {
    assert(row < R);
    assert(col < C);
    return m_data[row + R * col];
  }
  //----------------------------------------------------------------------------
  /// access to component at position [row, col]
  constexpr auto at(size_t row, size_t col) const {
    assert(row < R);
    assert(col < C);
    return m_data[row + R * col];
  }
  //----------------------------------------------------------------------------
  /// access to component at position [row, col]
  constexpr auto& operator()(size_t row, size_t col) {
    assert(row < R);
    assert(col < C);
    return at(row, col);
  }
  //----------------------------------------------------------------------------
  /// access to component at position [row, col]
  constexpr auto operator()(size_t row, size_t col) const {
    assert(row < R);
    assert(col < C);
    return at(row, col);
  }
  //----------------------------------------------------------------------------
  /// access to raw data block
  constexpr auto& operator[](size_t i) {return m_data[i];} 
  //----------------------------------------------------------------------------
  /// access to raw data block
  constexpr auto operator[](size_t i) const {return m_data[i];} 
  //----------------------------------------------------------------------------
  /// returns reference to data block
  constexpr auto& data() { return m_data; }
  //----------------------------------------------------------------------------
  constexpr Real* data_ptr() { return m_data.data(); }
  constexpr const Real* data_ptr() const { return m_data.data(); }
};
//==============================================================================
template <typename Real>
using Mat2 = mat<Real, 2, 2>;
template <typename Real>
using Mat3 = mat<Real, 3, 3>;
template <typename Real>
using Mat4 = mat<Real, 4, 4>;
//------------------------------------------------------------------------------ 
using mat2   = Mat2<float>;
using mat3   = Mat3<float>;
using mat4   = Mat4<float>;
//------------------------------------------------------------------------------
using dmat2   = Mat2<double>;
using dmat3   = Mat3<double>;
using dmat4   = Mat4<double>;
//------------------------------------------------------------------------------
using i32mat2   = Mat2<int32_t>;
using i32mat3   = Mat3<int32_t>;
using i32mat4   = Mat4<int32_t>;
//------------------------------------------------------------------------------
using ui32mat2   = Mat2<uint32_t>;
using ui32mat3   = Mat3<uint32_t>;
using ui32mat4   = Mat4<uint32_t>;
//------------------------------------------------------------------------------
using i16mat2   = Mat2<int16_t>;
using i16mat3   = Mat3<int16_t>;
using i16mat4   = Mat4<int16_t>;
//------------------------------------------------------------------------------
using ui16mat2   = Mat2<uint16_t>;
using ui16mat3   = Mat3<uint16_t>;
using ui16mat4   = Mat4<uint16_t>;
//------------------------------------------------------------------------------
using i8mat2   = Mat2<int8_t>;
using i8mat3   = Mat3<int8_t>;
using i8mat4   = Mat4<int8_t>;
//------------------------------------------------------------------------------
using ui8mat2   = Mat2<uint8_t>;
using ui8mat3   = Mat3<uint8_t>;
using ui8mat4   = Mat4<uint8_t>;
//==============================================================================
/// deduction guide gets matrix dimensions when creating object
template <size_t C, typename... Ts>
mat(Ts const (&... arr)[C])->mat<promote_t<Ts...>, sizeof...(Ts), C>;

//==============================================================================
// operations
//==============================================================================
/// component-wise addition of two matrices
template <typename Real, size_t R, size_t C>
constexpr auto operator+(const mat<Real, R, C>& m0, const mat<Real, R, C>& m1) {
  mat m{m0};
  return m += m1;
}
//------------------------------------------------------------------------------
/// component-wise subtraction of two matrices
template <typename Real, size_t R, size_t C>
constexpr auto operator-(const mat<Real, R, C>& m0, const mat<Real, R, C>& m1) {
  mat m{m0};
  return m -= m1;
}
//------------------------------------------------------------------------------
/// negation of a matrix
template <typename Real, size_t R, size_t C>
constexpr auto operator-(const mat<Real, R, C>& m) {
  mat neg{m};
  for (auto& c : neg.data()) c *= -1;
  return neg;
}
//------------------------------------------------------------------------------
/// scaling of a matrix
template <typename Real, size_t R, size_t C>
constexpr auto operator*(const mat<Real, R, C>& m, Real s) {
  mat copy{m};
  return copy *= s;
}
//------------------------------------------------------------------------------
/// scaling of a matrix
template <typename Real, size_t R, size_t C>
constexpr auto operator*(Real s, const mat<Real, R, C>& m) {
  mat copy{m};
  return copy /= s;
}
//------------------------------------------------------------------------------
/// inverse scaling of a matrix
template <typename Real, size_t R, size_t C>
constexpr auto operator/(const mat<Real, R, C>& m, Real s) {
  mat copy{m};
  return copy /= s;
}
//------------------------------------------------------------------------------
/// inverse scaling of a matrix
template <typename Real, size_t R, size_t C>
constexpr auto operator/(Real s, const mat<Real, R, C>& m) {
  mat copy{m};
  return copy /= s;
}
//------------------------------------------------------------------------------
/// matrix-vector-multiplication
template <typename Real, size_t R, size_t C, size_t N>
constexpr auto operator*(const mat<Real, R, C>& lhs, const vec<Real, N>& rhs) {
  static_assert(C == N,
                "size of vector does not match number of columns of matrix");
  vec<Real, R> multiplicated;

  for (size_t m_row = 0; m_row < R; ++m_row) {
    for (size_t i = 0; i < C; ++i) {
      multiplicated(m_row) += lhs(m_row, i) * rhs(i);
    }
  }
  return multiplicated;
}
//------------------------------------------------------------------------------
/// matrix-matrix-multiplication
template <typename Real, size_t R0, size_t C0, size_t R1, size_t C1>
constexpr auto operator*(const mat<Real, R0, C0>& lhs,
                         const mat<Real, R1, C1>& rhs) {
  static_assert(C0 == R1,
                "left number of columns does not match right number of rows");
  auto m = mat<Real, R0, C1>::zeros() ;

  for (size_t lhs_row = 0; lhs_row < R0; ++lhs_row) {
    for (size_t rhs_col = 0; rhs_col < C1; ++rhs_col) {
      for (size_t i = 0; i < C0; ++i) {
        m(lhs_row, rhs_col) += lhs(lhs_row, i) * rhs(i, rhs_col);
      }
    }
  }
  return m;
}
//------------------------------------------------------------------------------
/// creates transposed version of m
template <typename Real, size_t R, size_t C>
constexpr auto transpose(const mat<Real, R,C>& m) {
  auto t = mat<Real, C, R>::zeros();
  for (size_t r = 0; r < R; ++r) {
    for (size_t c = 0; c < C; ++c) { t(c, r) = m(r, c); }
  }
  return t;
}
//------------------------------------------------------------------------------
/// Inverse of a 2x2 matrix.
/// Returns nothing if determinant is 0.
template <typename Real>
inline std::optional<Mat2<Real>> inverse(const Mat2<Real>& m) {
  auto det = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1));
  if (std::abs(det) < 1e-7) { return {}; }

  Mat2<Real> inv{{m(1, 1), -m(0, 1)}, {-m(1, 0), m(0, 0)}};

  return inv / det;
}
//------------------------------------------------------------------------------
/// Inverse of a 3x3 matrix.
/// Returns nothing if determinant is 0.
template <typename Real>
inline std::optional<Mat3<Real>> inverse(const Mat3<Real>& m) {
  auto det = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
             m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
             m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
  if (std::abs(det) < 1e-7) { return {}; }
  Mat3<Real> inv{{(m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)),
                       -(m(0, 1) * m(2, 2) - m(0, 2) * m(2, 1)),
                       (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1))},
                      {-(m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)),
                       (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)),
                       -(m(0, 0) * m(1, 2) - m(1, 0) * m(0, 2))},
                      {(m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)),
                       -(m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)),
                       (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1))}};
  return inv / det;
}
//------------------------------------------------------------------------------
/// Inverse of a 4x4 matrix.
/// Returns nothing if determinant is 0.
template <typename Real>
inline std::optional<Mat4<Real>> inverse(const Mat4<Real>& m) {
  auto inv = Mat4<Real>::zeros();

  inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
           m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

  inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
           m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

  inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
           m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

  inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

  inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
           m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

  inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
           m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

  inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
           m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

  inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

  inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
           m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

  inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
           m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

  inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

  inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

  inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
           m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

  inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
           m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

  inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

  inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

  auto det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
  if (std::abs(det) < 1e-7) { return {}; }
  return inv / det;
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr auto translation_matrix(Real x, Real y, Real z) {
  auto m  = Mat4<Real>::eye();
  m(0, 3) = x; m(1, 3) = y; m(2, 3) = z;
  return m;
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr auto translation_matrix(vec<Real, 3> t) {
  auto m  = Mat4<Real>::eye();
  m(0, 3) = t(0); m(1, 3) = t(1); m(2, 3) = t(2);
  return m;
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr auto scale_matrix(Real s) {
  auto m  = Mat4<Real>::zeros();
  m(0, 0) = s; m(1, 1) = s; m(2, 2) = s; m(3, 3) = 1;
  return m;
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr auto scale_matrix(Real x, Real y, Real z) {
  auto m  = Mat4<Real>::zeros();
  m(0, 0) = x;
  m(1, 1) = y;
  m(2, 2) = z;
  m(3, 3) = 1;
  return m;
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr auto scale_matrix(const vec<Real, 3>& s) {
  auto m  = Mat4<Real>::zeros();
  m(0, 0) = s(0);
  m(1, 1) = s(1);
  m(2, 2) = s(2);
  m(3, 3) = 1;
  return m;
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr Mat4<Real> rotation_matrix(Real angle, Real u, Real v, Real w) {
  const Real s = std::sin(angle);
  const Real c = std::cos(angle);
  return {{u * u + (v * v + w * w) * c,
           u * v * (1 - c) - w * s,
           u * w * (1 - c) + v * s,
           Real(0)},
          {u * v + (1 - c) + w * s,
           v * v * (u * u + w * w) * c,
           v * w * (1 - c) + u * s,
           Real(0)},
          {u * w + (1 - c) - v * s,
           v * v * (1 - c) + u * s,
           w * w + (u * u + v * v) * c,
           Real(0)},
          {Real(0), Real(0), Real(0), Real(1)}};
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr auto rotation_matrix(Real angle, const vec<Real, 3>& axis) {
  return rotation_matrix(angle, axis(0), axis(1), axis(2));
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr Mat4<Real> orthographic_matrix(const Real l, const Real r,
                                         const Real b, const Real t,
                                         const Real n, const Real f) {
  return {
    {2 / (r - l),     Real(0),      Real(0), -(r + l) / (r - l)},
    {    Real(0), 2 / (t - b),      Real(0), -(t + b) / (t - b)},
    {    Real(0),     Real(0), -2 / (f - n), -(f + n) / (f - n)},
    {    Real(0),     Real(0),      Real(0),            Real(1)}
  };
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr Mat4<Real> perspective_matrix(const Real l, const Real r,
                                        const Real b, const Real t,
                                        const Real n, const Real f) {
  return {
   {2 * n / (r - l),         Real(0),  (r + l) / (r - l),              Real(0)},
   {        Real(0), 2 * n / (t - b),  (t + b) / (t - b),              Real(0)},
   {        Real(0),         Real(0), -(f + n) / (f - n), -2 * f * n / (f - n)},
   {        Real(0),         Real(0),           Real(-1),              Real(0)}
  };
}
//------------------------------------------------------------------------------
template <typename Real>
constexpr auto perspective_matrix(const Real angleOfView,
                                  const Real imageAspectRatio, const Real n,
                                  const Real f) {
  const Real scale = std::tan(angleOfView * 0.5 * M_PI / 180) * n;
  const Real r     = imageAspectRatio * scale;
  const Real l     = -r;
  const Real t     = scale;
  const Real b     = -t;
  return perspective_matrix(l, r, b, t, n, f);
}
//------------------------------------------------------------------------------
template <typename Real>
Mat4<Real> look_at_matrix(const vec<Real, 3>& eye,
                          const vec<Real, 3>& center,
                          const vec<Real, 3>& up) {
  const auto z = normalize(eye - center);
  const auto y = cross(up, z);
  const auto x = cross(z, y);
  return {{x(0), y(0), z(0), eye(0)},
          {x(1), y(1), z(1), eye(1)},
          {x(2), y(2), z(2), eye(2)},
          {   0,    0,    0,      1}};
}
//==============================================================================
// I/O
//==============================================================================
/// printing a matrix into a stream
template <typename Real, size_t R, size_t C>
constexpr auto& operator<<(std::ostream& str, const mat<Real, R, C>& m) {
  for (size_t row = 0; row != m.R; ++row) {
    str << "[ ";
    for (size_t col = 0; col != m.num_columns; ++col) {
      str << m(row, col) << ' ';
    }
    str << "]\n";
  }
  return str;
}
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
