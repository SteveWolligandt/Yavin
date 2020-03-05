#ifndef YAVIN_UTILITY_H
#define YAVIN_UTILITY_H
//==============================================================================
#include <array>
#include <utility>
//==============================================================================
namespace yavin {
//==============================================================================
/// creates an array of size N filled with val of type T
template <typename T, size_t N, size_t... Is>
auto make_array(const T& val, std::index_sequence<Is...>) {
  return std::array{((void)Is, val)...};
}
//------------------------------------------------------------------------------
/// creates an array of size N filled with val of type T
template <typename T, size_t N>
auto make_array(const T& val) {
  return make_array<T, N>(val, std::make_index_sequence<N>{});
}
//------------------------------------------------------------------------------
/// Applies function F to all elements of parameter pack ts
template <typename F, typename... Ts>
void for_each(F&& f, Ts&&... ts) {
  using discard_t = int[];
  // creates an array filled with zeros. while doing this f gets called with
  // elements of ts
  (void)discard_t{0, ((void)f(std::forward<Ts>(ts)), 0)...};
}
//==============================================================================
template <typename... Ts>
struct promote;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <typename... Ts>
using promote_t = typename promote<Ts...>::type;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <typename T0, typename T1>
struct promote<T0, T1> {
  using type =
      std::decay_t<decltype(true ? std::declval<T0>() : std::declval<T1>())>;
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <typename T0, typename T1, typename T2, typename... Ts>
struct promote<T0, T1, T2, Ts...> {
  using type = promote_t<T0, promote_t<T1, T2, Ts...>>;
};
//==============================================================================
}  // namespace yavin
//==============================================================================

#endif