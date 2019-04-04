#include <catch2/catch.hpp>
#include <yavin>
#include <type_traits>
#include <random>
#include <boost/range/algorithm/generate.hpp>

//==============================================================================
namespace yavin::test {
//==============================================================================
extern Window window;

TEMPLATE_PRODUCT_TEST_CASE(
    "[Texture2D] upload and download",
    "[texture2d]",
    Texture2D,
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

  std::mt19937_64 eng{std::random_device{}()};
  const size_t w = 200, h = 200;
  using type = typename TestType::type;
  using dist_t = std::conditional_t<
                   std::is_floating_point_v<type>,
                   std::uniform_real_distribution<type>,
                   std::uniform_int_distribution<type>>;
  const type min = std::is_floating_point_v<type> ? -100 : 0;
  const type max = 100;
  dist_t rand{min, max};


  // generate random data
  std::vector<type> data(w * h * TestType::num_components);
  boost::generate(data, [&]{return rand(eng);});

  // create texture and upload data
  TestType tex(w, h, data);

  auto downloaded_data = tex.download_data();
  REQUIRE(data.size() == downloaded_data.size());
  auto d = begin(data);
  auto dc = begin(downloaded_data);
  for (; d != end(data); ++d, ++dc) REQUIRE (*d == *dc);
}

//==============================================================================
}  // namespace yavin::test
//==============================================================================
