#ifndef __YAVIN_VBOHELPERS_H__
#define __YAVIN_VBOHELPERS_H__

#include <array>
#include <initializer_list>
#include <iostream>
#include <vector>
#include "gl_includes.h"

namespace Yavin {

/**
 * @brief      Base Attribute
 *
 * @tparam     Ts    Data in attribute
 */
template <typename... Ts>
struct attrib {};

/**
 * @brief      float
 */
template <>
struct attrib<float> {
  float                         val;
  static constexpr GLenum       type     = GL_FLOAT;
  static constexpr unsigned int num_dims = 1;
};

/**
 * @brief      float vec2
 */
template <>
struct attrib<float, float> {
  float                         x, y;
  static constexpr GLenum       type     = GL_FLOAT;
  static constexpr unsigned int num_dims = 2;
};

/**
 * @brief      float vec3
 */
template <>
struct attrib<float, float, float> {
  float                         x, y, z;
  static constexpr GLenum       type     = GL_FLOAT;
  static constexpr unsigned int num_dims = 3;
};

/**
 * @brief      float vec4
 */
template <>
struct attrib<float, float, float, float> {
  float                         x, y, z, w;
  static constexpr GLenum       type     = GL_FLOAT;
  static constexpr unsigned int num_dims = 4;
};

using scalar = attrib<float>;
using vec2   = attrib<float, float>;
using vec3   = attrib<float, float, float>;
using vec4   = attrib<float, float, float, float>;

template <typename... Ss>
constexpr auto plus_fold(Ss... ss) {
  return (... + ss);
}

/**
 * @brief      value contains number of bytes of attribute
 *
 * @tparam     Ts    data of attribute
 */
template <typename... Ts>
struct attrib_size {
  static constexpr size_t value = plus_fold(sizeof(Ts)...);
};

/**
 * @brief      value contains number of bytes of attributes
 *
 * @tparam     Ts    attributes
 */
template <typename... Ts>
struct attrib_pack_size {
  static constexpr size_t value = plus_fold(attrib_size<Ts>::value...);
};

///////////// at compile time the attribute offsets, sizes, types and dimensions
///////////// are generated and saved into an std::array
template <unsigned int num_attrs, class... Ts>
struct attr_generator_offset;

template <unsigned int num_attrs>
struct attr_generator_offset<num_attrs> {
  constexpr static std::array<unsigned long, num_attrs> gen(unsigned long,
                                                            unsigned int) {
    return std::array<unsigned long, num_attrs>();
  }
};

template <unsigned num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_generator_offset<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static std::array<unsigned long, num_attrs> gen(unsigned long off,
                                                            unsigned int  idx) {
    auto arr = attr_generator_offset<num_attrs, RestAttribs...>::gen(
        off + attrib_pack_size<FirstAttrib>::value, idx + 1);
    arr[idx] = off;
    return arr;
  }
};

template <unsigned int num_attrs, class... Ts>
struct attr_generator_sizes;

template <unsigned int num_attrs>
struct attr_generator_sizes<num_attrs> {
  constexpr static std::array<unsigned int, num_attrs> gen(unsigned int) {
    return std::array<unsigned int, num_attrs>();
  }
};

template <unsigned num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_generator_sizes<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static std::array<unsigned int, num_attrs> gen(unsigned int idx) {
    auto arr = attr_generator_sizes<num_attrs, RestAttribs...>::gen(idx + 1);
    arr[idx] = attrib_size<FirstAttrib>::value;
    return arr;
  }
};

template <unsigned int num_attrs, class... Ts>
struct attr_generator_num_dims;

template <unsigned int num_attrs>
struct attr_generator_num_dims<num_attrs> {
  constexpr static std::array<unsigned int, num_attrs> gen(unsigned int) {
    return std::array<unsigned int, num_attrs>();
  }
};

template <unsigned num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_generator_num_dims<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static std::array<unsigned int, num_attrs> gen(unsigned int idx) {
    auto arr = attr_generator_num_dims<num_attrs, RestAttribs...>::gen(idx + 1);
    arr[idx] = FirstAttrib::num_dims;
    return arr;
  }
};

template <unsigned int num_attrs, class... Ts>
struct attr_generator_types;

template <unsigned int num_attrs>
struct attr_generator_types<num_attrs> {
  constexpr static std::array<GLenum, num_attrs> gen(unsigned int) {
    return std::array<GLenum, num_attrs>();
  }
};

template <unsigned num_attrs, class FirstAttrib, class... RestAttribs>
struct attr_generator_types<num_attrs, FirstAttrib, RestAttribs...> {
  constexpr static std::array<GLenum, num_attrs> gen(unsigned int idx) {
    auto arr = attr_generator_types<num_attrs, RestAttribs...>::gen(idx + 1);
    arr[idx] = FirstAttrib::type;
    return arr;
  }
};

template <class... Ts>
struct attr_prefs {
  constexpr static unsigned int num_attrs = sizeof...(Ts);
  constexpr static std::array<unsigned long, num_attrs> offsets =
      attr_generator_offset<num_attrs, Ts...>::gen(0, 0);
  constexpr static std::array<unsigned int, num_attrs> sizes =
      attr_generator_sizes<num_attrs, Ts...>::gen(0);
  constexpr static std::array<unsigned int, num_attrs> num_dims =
      attr_generator_num_dims<num_attrs, Ts...>::gen(0);
  constexpr static std::array<unsigned int, num_attrs> types =
      attr_generator_types<num_attrs, Ts...>::gen(0);
};

/////// Following structs are used for indexing vbo memory
template <typename Data_t, unsigned int M, unsigned int N,
          unsigned int PackSize, typename... Ts>
struct tuple_constructor;

template <typename Data_t, unsigned int M, unsigned int N,
          unsigned int PackSize, typename T>
struct tuple_constructor<Data_t, M, N, PackSize, T> {
  static std::tuple<T&> construct(std::vector<Data_t>& buffer, unsigned int idx,
                                  const std::array<unsigned long, M>& offsets) {
    unsigned char* temp_buf = reinterpret_cast<unsigned char*>(&buffer[0]);
    return std::tuple<T&>(
        *reinterpret_cast<T*>(&temp_buf[idx * PackSize + offsets[N]]));
  }

  static std::tuple<const T&> construct(
      const std::vector<Data_t>& buffer, unsigned int idx,
      const std::array<unsigned long, M>& offsets) {
    const unsigned char* temp_buf =
        reinterpret_cast<const unsigned char*>(&buffer[0]);
    return std::tuple<const T&>(
        *reinterpret_cast<const T*>(&temp_buf[idx * PackSize + offsets[N]]));
  }
};

template <typename Data_t, unsigned int M, unsigned int N,
          unsigned int PackSize, typename T, typename... Ts>
struct tuple_constructor<Data_t, M, N, PackSize, T, Ts...> {
  static std::tuple<T&, Ts&...> construct(
      std::vector<Data_t>& buffer, unsigned int idx,
      const std::array<unsigned long, M>& offsets) {
    return std::tuple_cat(
        tuple_constructor<Data_t, M, N, PackSize, T>::construct(buffer, idx,
                                                                offsets),
        tuple_constructor<Data_t, M, N + 1, PackSize, Ts...>::construct(
            buffer, idx, offsets));
  }
  static std::tuple<const T&, const Ts&...> construct(
      const std::vector<Data_t>& buffer, unsigned int idx,
      const std::array<unsigned long, M>& offsets) {
    return std::tuple_cat(
        tuple_constructor<Data_t, M, N, PackSize, T>::construct(buffer, idx,
                                                                offsets),
        tuple_constructor<Data_t, M, N + 1, PackSize, Ts...>::construct(
            buffer, idx, offsets));
  }
};

template <int N, typename... Ts>
struct index_param_pack;

template <typename T, typename... Ts>
struct index_param_pack<0, T, Ts...> {
  typedef T type;
};
template <int N, typename T, typename... Ts>
struct index_param_pack<N, T, Ts...> {
  typedef typename index_param_pack<N - 1, Ts...>::type type;
};

/**
 * data attribute acceptance. These structs are used for compile-time safety for
 * actual data specification
 */
template <class Attrib, class T>
struct data_attrib_pair {
  static constexpr bool exists = false;
};

// the following is needed for static assertions of types
template <class... Attribs>
struct AttribHolder {};

template <class... Params>
struct ParamHolder {};

// this is used for the last step in recursion
template <class Attrib, class Param>
constexpr bool can_apply(AttribHolder<Attrib>, ParamHolder<Param>) {
  return data_attrib_pair<Attrib, Param>::exists;
}

// for every attribute parameter tuple check if it data_attrib_pair is defined
template <class FirstAttrib, class SecondAttrib, class... Attribs,
          class FirstParam, class SecondParam, class... Params>
constexpr bool can_apply(AttribHolder<FirstAttrib, SecondAttrib, Attribs...>,
                         ParamHolder<FirstParam, SecondParam, Params...>) {
  return data_attrib_pair<FirstAttrib, FirstParam>::exists &&
         can_apply(AttribHolder<SecondAttrib, Attribs...>(),
                   ParamHolder<SecondParam, Params...>());
}

//////// basic templates for data

/**
 * @brief      data_size for more complicated types
 */
template <class T>
struct data_size {
  static constexpr unsigned int value = sizeof(T);
};

/**
 * @brief      basic data to buffer inserter
 */
template <class T>
struct data_inserter {
  static unsigned int insert(const T& data, char* buffer, unsigned int offset) {
    *((T*)(&buffer[offset])) = data;
    return data_size<T>::value;
  }
};

// variadic template struct taking several attribs
template <class... Attribs>
struct data_representation {
  // constructor takes variadic number of arguments
  // and checks if the size of parameters matches
  // the size of the attribs
  template <class... DataTs>
  data_representation(const DataTs&... data) {
    static_assert(
        can_apply(AttribHolder<Attribs...>(), ParamHolder<DataTs...>()),
        "Data values do not match");

    set_data<DataTs...>(0, data...);
  }

  data_representation() = default;

  template <class... DataTs>
  void operator=(const DataTs&... data) {
    static_assert(
        can_apply(AttribHolder<Attribs...>(), ParamHolder<DataTs...>()),
        "Data values do not match");

    set_data<DataTs...>(0, data...);
  }

  // assigns a value to a part of the internal memory buffer
  template <class T>
  void set_data(const unsigned int offset, const T& data) {
    data_inserter<T>::insert(data, m_buffer, offset);
  }

  // recursively assigns values to parts of the internal memory buffer
  template <class FirstDataT, class SecondDataT, class... DataTs>
  void set_data(const unsigned int offset, const FirstDataT& first,
                const SecondDataT& second, const DataTs&... data) {
    auto num_data_inserted =
        data_inserter<FirstDataT>::insert(first, m_buffer, offset);
    set_data<SecondDataT, DataTs...>(offset + num_data_inserted, second,
                                     data...);
  }

  char m_buffer[attrib_pack_size<Attribs...>::value];
};

//////// yavin implementation

template <>
struct data_size<vec2> {
  static constexpr unsigned int value = sizeof(float) * 2;
};
template <>
struct data_size<vec3> {
  static constexpr unsigned int value = sizeof(float) * 3;
};
template <>
struct data_size<vec4> {
  static constexpr unsigned int value = sizeof(float) * 4;
};

template <>
struct data_attrib_pair<vec2, vec2> {
  static constexpr bool exists = true;
};

template <>
struct data_attrib_pair<vec3, vec3> {
  static constexpr bool exists = true;
};

template <>
struct data_attrib_pair<vec4, vec4> {
  static constexpr bool exists = true;
};

/**
 * specialized data to buffer inserter for vec2
 */
template <>
struct data_inserter<vec2> {
  static unsigned int insert(const vec2& data, char* buffer,
                             unsigned int offset) {
    float* temp_buf = (float*)(&buffer[offset]);
    temp_buf[0]     = data.x;
    temp_buf[1]     = data.y;
    return data_size<vec2>::value;
  }
};

/**
 * specialized data to buffer inserter for vec3
 */
template <>
struct data_inserter<vec3> {
  static unsigned int insert(const vec3& data, char* buffer,
                             unsigned int offset) {
    float* temp_buf = (float*)(&buffer[offset]);
    temp_buf[0]     = data.x;
    temp_buf[1]     = data.y;
    temp_buf[2]     = data.z;
    return data_size<vec3>::value;
  }
};

/**
 * specialized data to buffer inserter for vec4
 */
template <>
struct data_inserter<vec4> {
  static unsigned int insert(const vec4& data, char* buffer,
                             unsigned int offset) {
    float* temp_buf = (float*)(&buffer[offset]);
    temp_buf[0]     = data.x;
    temp_buf[1]     = data.y;
    temp_buf[2]     = data.z;
    temp_buf[3]     = data.w;
    return data_size<vec4>::value;
  }
};
}  // namespace Yavin
#endif
