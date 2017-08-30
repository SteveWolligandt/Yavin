#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <png++/png.hpp>
#include <regex>
#include <tuple>
#include <yavin>

int obj_loader() {
  std::string filepath = "bunny.obj";
  std::string err;

  tinyobj::attrib_t                attrib;
  std::vector<tinyobj::shape_t>    shapes;
  std::vector<tinyobj::material_t> materials;

  auto ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str());

  if (!ok) return 1;
  if (!err.empty()) {  // `err` may contain warning message.
    std::cerr << err << std::endl;
    return 1;
  }

  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    auto index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      auto fv = shapes[s].mesh.num_face_vertices[f];

      auto has_normals    = attrib.normals.size() > 0;
      auto has_tex_coords = attrib.texcoords.size() > 0;
      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++) {
        auto idx = shapes[s].mesh.indices[index_offset + v];
        auto vx  = attrib.vertices[3 * idx.vertex_index + 0];
        auto vy  = attrib.vertices[3 * idx.vertex_index + 1];
        auto vz  = attrib.vertices[3 * idx.vertex_index + 2];
      }

      if (has_normals) {
        for (size_t v = 0; v < fv; v++) {
          auto idx = shapes[s].mesh.indices[index_offset + v];

          auto nx = attrib.normals[3 * idx.normal_index + 0];
          auto ny = attrib.normals[3 * idx.normal_index + 1];
          auto nz = attrib.normals[3 * idx.normal_index + 2];
        }
      }

      if (has_tex_coords) {
        for (size_t v = 0; v < fv; v++) {
          auto idx = shapes[s].mesh.indices[index_offset + v];
          auto tx  = attrib.texcoords[2 * idx.texcoord_index + 0];
          auto ty  = attrib.texcoords[2 * idx.texcoord_index + 1];
        }
      }
      index_offset += fv;
    }
  }
}

int main(int argc, char** argv) { obj_loader(); }