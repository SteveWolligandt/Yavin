#include "Mesh.h"

namespace Yavin {

template class BaseMesh<vec3>;
template class BaseMesh<vec3, vec2>;
template class BaseMesh<vec3, vec3>;
template class BaseMesh<vec3, vec3, vec2>;

template class Mesh<vec3>;
template class Mesh<vec3, vec2>;
template class Mesh<vec3, vec3>;
template class Mesh<vec3, vec3, vec2>;

template <typename... Ts>
void BaseMesh<Ts...>::draw() {
  for (unsigned int i = 0; i < m_vaos.size(); ++i) {
    m_vaos[i].draw(Yavin::TRIANGLES, m_ibos[i].gpu_size());
  }
}

// template <typename... Ts>
// void BaseMesh<Ts...>::load_obj(
//     const std::string& filepath,
//     std::function<void(size_t, const tinyobj::attrib_t&, const
//     std::vector<tinyobj::shape_t>&,
//                        const std::vector<tinyobj::material_t>&, const
//                        std::vector<size_t>&, const std::vector<size_t>&)>
//         f) {
//   std::cout << filepath << '\n';
//   std::string err;

//   tinyobj::attrib_t                attrib;
//   std::vector<tinyobj::shape_t>    shapes;
//   std::vector<tinyobj::material_t> materials;

//   auto ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
//   filepath.c_str());

//   if (!ok) {
//     std::cerr << "not ok\n";
//     return;
//   }
//   if (!err.empty()) {  // `err` may contain warning message.
//     std::cerr << err << std::endl;
//     return;
//   }

//   std::vector<size_t> normal_indices;
//   std::vector<size_t> texcoord_indices;

//   if (attrib.normals.size() > 0)
//   normal_indices.resize(attrib.vertices.size(), -1); if
//   (attrib.texcoords.size() > 0)
//   texcoord_indices.resize(attrib.vertices.size(), -1);

//   for (size_t s = 0; s < shapes.size(); s++)
//     for (size_t i = 0; i < shapes[s].mesh.indices.size(); ++i) {
//       if (attrib.normals.size() > 0)
//         normal_indices[shapes[s].mesh.indices[i].vertex_index] =
//             shapes[s].mesh.indices[i].normal_index;
//       if (attrib.texcoords.size() > 0)
//         texcoord_indices[shapes[s].mesh.indices[i].vertex_index] =
//             shapes[s].mesh.indices[i].texcoord_index;
//     }

//   // Loop over faces(polygon)
//   m_vaos.emplace_back();
//   m_vbos.emplace_back();
//   m_ibos.emplace_back();

//   m_vbos.back().reserve(attrib.vertices.size() / 3.0);

//   for (size_t i = 0; i < attrib.vertices.size() / 3; ++i)
//     f(i, attrib, shapes, materials, normal_indices, texcoord_indices);

//   m_vbos.back().upload_data();

//   for (size_t s = 0; s < shapes.size(); s++)
//     for (size_t i = 0; i < shapes[s].mesh.indices.size(); ++i)
//       m_ibos.back().push_back(shapes[s].mesh.indices[i].vertex_index);
//   m_ibos.back().upload_data();

//   m_vaos.back().bind();
//   m_vbos.back().bind();
//   m_ibos.back().bind();
//   vbo_t::activate_attributes();
// }

Mesh<vec3>::Mesh(const std::string& filepath) { /*this->load_obj(filepath);*/
}
Mesh<vec3, vec3>::Mesh(const std::string& filepath) { /*load_obj(filepath);*/
}
Mesh<vec3, vec2>::Mesh(const std::string& filepath) { /*load_obj(filepath);*/
}
Mesh<vec3, vec3, vec2>::Mesh(
    const std::string& filepath) { /*load_obj(filepath);*/
}
/*
void Mesh<vec3>::load_obj(const std::string& filepath) {
  BaseMesh<vec3>::load_obj(
      filepath, [&](size_t i, const tinyobj::attrib_t& attrib, const
std::vector<tinyobj::shape_t>&, const std::vector<tinyobj::material_t>&, const
std::vector<size_t>&, const std::vector<size_t>&) {
        m_vbos.back().push_back({attrib.vertices[i * 3], attrib.vertices[i * 3 +
1], attrib.vertices[i * 3 + 2]});
      });
}

void Mesh<vec3, vec2>::load_obj(const std::string& filepath) {
  BaseMesh<vec3, vec2>::load_obj(
      filepath, [&](size_t i, const tinyobj::attrib_t& attrib, const
std::vector<tinyobj::shape_t>&, const std::vector<tinyobj::material_t>&, const
std::vector<size_t>&, const std::vector<size_t>& texcoord_indices) {
        m_vbos.back().push_back({attrib.vertices[i * 3], attrib.vertices[i * 3 +
1], attrib.vertices[i * 3 + 2]},
                                ((attrib.texcoords.size() > 0) ?
vec2{attrib.texcoords[texcoord_indices[i] * 2],
                                                                      attrib.texcoords[texcoord_indices[i]
* 2 + 1]} : vec2{0, 0}));
      });
}

void Mesh<vec3, vec3>::load_obj(const std::string& filepath) {
  BaseMesh<vec3, vec3>::load_obj(
      filepath, [&](size_t i, const tinyobj::attrib_t& attrib, const
std::vector<tinyobj::shape_t>&, const std::vector<tinyobj::material_t>&, const
std::vector<size_t>& normal_indices, const std::vector<size_t>&) {
        m_vbos.back().push_back({attrib.vertices[i * 3], attrib.vertices[i * 3 +
1], attrib.vertices[i * 3 + 2]},
                                ((attrib.normals.size() > 0) ?
vec3{attrib.normals[normal_indices[i] * 3], attrib.normals[normal_indices[i] * 3
+ 1], attrib.normals[normal_indices[i] * 3 + 2]} : vec3{0, 0, 0}));
      });
}

void Mesh<vec3, vec3, vec2>::load_obj(const std::string& filepath) {
  BaseMesh<vec3, vec3, vec2>::load_obj(
      filepath, [&](size_t i, const tinyobj::attrib_t& attrib, const
std::vector<tinyobj::shape_t>&, const std::vector<tinyobj::material_t>&, const
std::vector<size_t>& normal_indices, const std::vector<size_t>&
texcoord_indices) { m_vbos.back().push_back( {attrib.vertices[i * 3],
attrib.vertices[i * 3 + 1], attrib.vertices[i * 3 + 2]},
            ((attrib.normals.size() > 0)  //
                 ? vec3{attrib.normals[normal_indices[i] * 3],
attrib.normals[normal_indices[i] * 3 + 1], attrib.normals[normal_indices[i] * 3
+ 2]}                                                 // : vec3{0, 0, 0}),
//
            ((attrib.texcoords.size() > 0) // ?
vec2{attrib.texcoords[texcoord_indices[i] * 2],
attrib.texcoords[texcoord_indices[i] * 2 + 1]}  // : vec2{0, 0}));
//
      });
}*/
}  // namespace Yavin
