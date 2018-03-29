#ifndef __YAVIN_MESH_H__
#define __YAVIN_MESH_H__

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "tiny_obj_loader.h"
#include "dll_export.h"

namespace Yavin {
template <typename... Ts>
class BaseMesh {
 public:
  using vbo_t    = VertexBuffer<Ts...>;
  using vertex_t = typename vbo_t::data_t;

  DLL_API void draw();

 protected:
  // void load_obj(
  //     const std::string& filepath,
  //     std::function<void(size_t, const tinyobj::attrib_t&, const std::vector<tinyobj::shape_t>&,
  //                        const std::vector<tinyobj::material_t>&,
  //                        const std::vector<size_t>& normal_indices,
  //                        const std::vector<size_t>& texcoord_indices)>
  //         f);

  std::vector<VertexArray>         m_vaos;
  std::vector<VertexBuffer<Ts...>> m_vbos;
  std::vector<IndexBuffer>         m_ibos;
};

template <typename... Ts>
class Mesh;

template <>
class Mesh<vec3> : public BaseMesh<vec3> {
 public:
  DLL_API Mesh(const std::string& filepath);

 protected:
  DLL_API void load_obj(const std::string& filepath);
};

template <>
class Mesh<vec3, vec3> : public BaseMesh<vec3, vec3> {
 public:
  DLL_API Mesh(const std::string& filepath);

 protected:
  DLL_API void load_obj(const std::string& filepath);
};

template <>
class Mesh<vec3, vec2> : public BaseMesh<vec3, vec2> {
 public:
  DLL_API Mesh(const std::string& filepath);

 protected:
  DLL_API void load_obj(const std::string& filepath);
};

template <>
class Mesh<vec3, vec3, vec2> : public BaseMesh<vec3, vec3, vec2> {
 public:
  DLL_API Mesh(const std::string& filepath);

 protected:
  DLL_API void load_obj(const std::string& filepath);
};

}  // namespace Yavin

#endif
