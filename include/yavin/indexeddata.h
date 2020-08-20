#ifndef YAVIN_INDEXED_DATA_H
#define YAVIN_INDEXED_DATA_H
//==============================================================================
#include <yavin/indexbuffer.h>
#include <yavin/vertexarray.h>
#include <yavin/vertexbuffer.h>

#include <mutex>
//==============================================================================
namespace yavin {
//==============================================================================
template <typename... Ts>
class indexeddata {
  //============================================================================
  // TYPEDEFS
  //============================================================================
 public:
  using vbo_t        = yavin::vertexbuffer<Ts...>;
  using ibo_t        = yavin::indexbuffer;
  using vbo_data_t   = typename vbo_t::data_t;
  using ibo_data_t   = unsigned int;
  using vbo_data_vec = std::vector<vbo_data_t>;
  using ibo_data_vec = std::vector<ibo_data_t>;
  //============================================================================
  // MEMBERS
  //============================================================================
 private:
  vbo_t              m_vbo;
  ibo_t              m_ibo;
  mutable std::mutex m_mutex;
  //============================================================================
  // CONSTRUCTORS
  //============================================================================
 public:
  indexeddata() = default;
  //----------------------------------------------------------------------------
  indexeddata(indexeddata const& other)     = default;
  indexeddata(indexeddata&& other) noexcept = default;
  //----------------------------------------------------------------------------
  auto operator=(indexeddata const& other) -> indexeddata& = default;
  auto operator=(indexeddata&& other) noexcept -> indexeddata& = default;
  //----------------------------------------------------------------------------
  indexeddata(vbo_data_vec const& vbo_data, ibo_data_vec const& ibo_data)
      : m_vbo(vbo_data), m_ibo(ibo_data) {}
  //----------------------------------------------------------------------------
  indexeddata(size_t const vbo_size, size_t const ibo_size)
      : m_vbo(vbo_size), m_ibo(ibo_size) {}
  //============================================================================
  // METHODS
  //============================================================================
  auto create_vao() const {
    vertexarray vao;
    vao.bind();
    m_vbo.bind();
    vbo_t::activate_attributes();
    m_ibo.bind();
    return vao;
  }
  //----------------------------------------------------------------------------
  void clear() {
    //std::lock_guard lock{m_mutex};
    m_vbo.clear();
    m_ibo.clear();
  }
  //----------------------------------------------------------------------------
  void draw_points() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_points(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_line_strip() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_line_strip(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_line_loop() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_line_loop(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_lines() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_lines(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_line_strip_adjacency() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_line_strip_adjacency(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_triangle_strip() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_triangle_strip(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_triangle_fan() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_triangle_fan(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_triangles() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_triangles(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_triangle_strip_adjacency() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_triangle_strip_adjacency(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_triangles_adjacency() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_triangles_adjacency(m_ibo.size());
  }
  //----------------------------------------------------------------------------
  void draw_patches() const {
    std::lock_guard lock{m_mutex};
    auto vao = create_vao();
    vao.draw_patches(m_ibo.size());
  }
  //============================================================================
  // GETTER
  //============================================================================
  auto indexbuffer() -> auto& { return m_ibo; }
  auto indexbuffer() const -> auto const& { return m_ibo; }
  //----------------------------------------------------------------------------
  auto vertexbuffer() -> auto& { return m_vbo; }
  auto vertexbuffer() const -> auto const& { return m_vbo; }
  //----------------------------------------------------------------------------
  auto mutex() -> auto& { return m_mutex; }
  auto mutex() const -> auto const& { return m_mutex; }
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
