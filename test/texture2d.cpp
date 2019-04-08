#include <boost/range/algorithm/generate.hpp>
#include <catch2/catch.hpp>
#include <random>
#include <type_traits>
#include <yavin>

//==============================================================================
namespace yavin::test {
//==============================================================================
extern Window window;

template <typename type, typename C>
struct RandomTexture2D : tex2D<type, C> {
  using parent_t = tex2D<type, C>;
  using dist_t   = std::conditional_t<std::is_floating_point_v<type>,
                                    std::uniform_real_distribution<type>,
                                    std::uniform_int_distribution<type>>;

  static constexpr auto   num_components = parent_t::num_components;
  static constexpr size_t w = 20, h = 20;
  // static constexpr type min = std::is_floating_point_v<type> ? -100 : 0;
  static constexpr type min = 0;
  static constexpr type max = 100;

  std::mt19937_64   eng;
  std::vector<type> data;

  //----------------------------------------------------------------------------
  RandomTexture2D()
      : eng{std::random_device{}()}, data(w * h * num_components) {
    dist_t rand{min, max};
    auto   random_val = [&] { return rand(eng); };
    boost::generate(data, random_val);
    this->upload_data(data, w, h);
  }
};

TEMPLATE_PRODUCT_TEST_CASE(
    "[tex2D] upload and download", "[texture2d][texture]", RandomTexture2D,
    ((uint8_t , R)  , (uint8_t , RG)  , (uint8_t , RGB) ,
     (uint8_t , BGR), (uint8_t , RGBA), (uint8_t , BGRA),
     (int8_t  , R)  , (int8_t  , RG)  , (int8_t  , RGB) ,
     (int8_t  , BGR), (int8_t  , RGBA), (int8_t  , BGRA),
     (uint16_t, R)  , (uint16_t, RG)  , (uint16_t, RGB) ,
     (uint16_t, BGR), (uint16_t, RGBA), (uint16_t, BGRA),
     (int16_t , R)  , (int16_t , RG)  , (int16_t , RGB) ,
     (int16_t , BGR), (int16_t , RGBA), (int16_t , BGRA),
     (uint32_t, R)  , (uint32_t, RG)  , (uint32_t, RGB) ,
     (uint32_t, BGR), (uint32_t, RGBA), (uint32_t, BGRA),
     (int32_t , R)  , (int32_t , RG)  , (int32_t , RGB) ,
     (int32_t , BGR), (int32_t , RGBA), (int32_t , BGRA),
     (float   , R)  , (float   , RG)  , (float   , RGB) ,
     (float   , BGR), (float   , RGBA), (float   , BGRA))) {
  TestType tex;
  auto     downloaded_data = tex.download_data();
  REQUIRE(tex.data.size() == downloaded_data.size());
  auto it      = begin(tex.data);
  auto it_down = begin(downloaded_data);
  for (; it != end(tex.data); ++it, ++it_down) REQUIRE(*it == *it_down);
}

//==============================================================================
}  // namespace yavin::test
//==============================================================================
