#ifndef YAVIN_TUPLE_H
#define YAVIN_TUPLE_H
//==============================================================================
namespace yavin {
//==============================================================================
template <typename... Ts>
struct tuple;
//------------------------------------------------------------------------------
template <typename Head, typename... Tail>
struct tuple<Head, Tail...> {
  Head           head;
  tuple<Tail...> tail;
};
//------------------------------------------------------------------------------
template <typename Head>
struct tuple<Head> {
  Head head;
};
//==============================================================================
template <typename Head>
auto make_tuple(Head&& head) {
  return tuple<Head>{std::forward<Head>(head)};
}
//------------------------------------------------------------------------------
template <typename Head, typename... Tail>
auto make_tuple(Head&& head, Tail&&... tail) {
  return tuple<Head, Tail...>{std::forward<Head>(head),
                              make_tuple<Tail...>(std::forward<Tail>(tail)...)};
}
//==============================================================================
template <std::size_t Idx, typename Head, typename... Tail>
struct _tuple_get_t {
  static const auto& get(const tuple<Head, Tail...>& t) {
    return _tuple_get_t<Idx - 1, Tail...>::get(t.tail);
  }
  static auto& get(tuple<Head, Tail...>& t) {
    return _tuple_get_t<Idx - 1, Tail...>::get(t.tail);
  }
};
//------------------------------------------------------------------------------
template <typename Head, typename... Tail>
struct _tuple_get_t<0, Head, Tail...> {
  static const auto& get(const tuple<Head, Tail...>& t) { return t.head; }
  static auto&       get(tuple<Head, Tail...>& t) { return t.head; }
};
//------------------------------------------------------------------------------
template <std::size_t Idx, typename... Ts>
const auto& get(const tuple<Ts...>& t) {
  return _tuple_get_t<Idx, Ts...>::get(t);
}
//------------------------------------------------------------------------------
template <std::size_t Idx, typename... Ts>
auto& get(tuple<Ts...>& t) {
  return _tuple_get_t<Idx, Ts...>::get(t);
}
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
