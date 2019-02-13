#ifndef __YAVIN_INDEXED_DATA_H__
#define __YAVIN_INDEXED_DATA_H__

#include <memory>
#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename... Ts>
class IndexedData {
 public:
  using vbo_t        = VertexBuffer<Ts...>;
  using vbo_ptr_t    = std::shared_ptr<vbo_t>;
  using vbo_data_t   = typename vbo_t::data_t;
  using ibo_data_t   = unsigned int;
  using vbo_data_vec = std::vector<vbo_data_t>;
  using ibo_data_vec = std::vector<ibo_data_t>;

  //============================================================================

  IndexedData() : m_vbo(std::make_shared<vbo_t>()), m_ibo() { setup_vao(); }

  //----------------------------------------------------------------------------

  IndexedData(size_t num_vbo_data, size_t num_ibo_data)
      : m_vbo(std::make_shared<vbo_t>(num_vbo_data)), m_ibo(num_ibo_data) {
    setup_vao();
  }

  //----------------------------------------------------------------------------

  IndexedData(const vbo_data_vec& vbo_data, const ibo_data_vec& ibo_data)
      : m_vbo(std::make_shared<vbo_t>(vbo_data)), m_ibo(ibo_data) {
    setup_vao();
  }

  //----------------------------------------------------------------------------

  IndexedData(vbo_ptr_t& vbo, const ibo_data_vec& ibo_data)
      : m_vbo{vbo}, m_ibo{ibo_data} {
    setup_vao();
  }

  //----------------------------------------------------------------------------

  IndexedData(const IndexedData& other)
      : m_vbo{std::shared_ptr<vbo_t>{new vbo_t(*other.m_vbo.get())}},
        m_ibo{other.m_ibo} {
    setup_vao();
  }

  //----------------------------------------------------------------------------

  IndexedData(IndexedData&& other)
      : m_vao{std::move(other.m_vao)},
        m_vbo{std::move(other.m_vbo)},
        m_ibo{std::move(other.m_ibo)} {}

  //----------------------------------------------------------------------------

  auto& operator=(const IndexedData& other) {
    m_vbo = std::make_shared<vbo_t>(new vbo_t{*other.m_vbo.get()});
    m_ibo = other.m_ibo;
    setup_vao();
  }

  //----------------------------------------------------------------------------

  auto& operator=(IndexedData&& other) {
    m_vao = std::move(other.m_vao);
    m_vbo = std::move(other.m_vbo);
    m_ibo = std::move(other.m_ibo);
    setup_vao();
  }

  //----------------------------------------------------------------------------

  void setup_vao() {
    m_vao.bind();
    m_vbo->bind();
    vbo_t::activate_attributes();
    m_ibo.bind();

    m_vao.unbind();
    m_vbo->unbind();
    m_ibo.unbind();
  }

  //----------------------------------------------------------------------------

  void draw_points() const {
    m_vao.bind();
    m_vao.draw_points(m_ibo.size());
  }
  void draw_line_strip() const {
    m_vao.bind();
    m_vao.draw_line_strip(m_ibo.size());
  }
  void draw_line_loop() const {
    m_vao.bind();
    m_vao.draw_line_loop(m_ibo.size());
  }
  void draw_lines() const {
    m_vao.bind();
    m_vao.draw_lines(m_ibo.size());
  }
  void draw_line_strip_adjacency() const {
    m_vao.bind();
    m_vao.draw_line_strip_adjacency(m_ibo.size());
  }
  void draw_triangle_strip() const {
    m_vao.bind();
    m_vao.draw_triangle_strip(m_ibo.size());
  }
  void draw_triangle_fan() const {
    m_vao.bind();
    m_vao.draw_triangle_fan(m_ibo.size());
  }
  void draw_triangles() const {
    m_vao.bind();
    m_vao.draw_triangles(m_ibo.size());
  }
  void draw_triangle_strip_adjacency() const {
    m_vao.bind();
    m_vao.draw_triangle_strip_adjacency(m_ibo.size());
  }
  void draw_triangles_adjacency() const {
    m_vao.bind();
    m_vao.draw_triangles_adjacency(m_ibo.size());
  }
  void draw_patches() const {
    m_vao.bind();
    m_vao.draw_patches(m_ibo.size());
  }

  //----------------------------------------------------------------------------

  auto&       index_buffer() { return m_ibo; }
  const auto& index_buffer() const { return m_ibo; }

  //----------------------------------------------------------------------------

  auto&       vertex_buffer() { return *m_vbo.get(); }
  const auto& vertex_buffer() const { return *m_vbo.get(); }
  auto        shared_vertex_buffer() { return m_vbo; }

 private:
  VertexArray m_vao;
  vbo_ptr_t   m_vbo;
  IndexBuffer m_ibo;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
