#ifndef __YAVIN_TUPLE_H__
#define __YAVIN_TUPLE_H__

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

template <std::size_t I, typename Head, typename... Tail>
struct _tuple_get_t {
  static auto get(const tuple<Head, Tail...>& t) {
    return _tuple_get_t<I - 1, Tail...>::get(t.tail);
  }
};

//------------------------------------------------------------------------------

template <typename Head, typename... Tail>
struct _tuple_get_t<0, Head, Tail...> {
  static auto get(const tuple<Head, Tail...>& t) { return t.head; }
};

//------------------------------------------------------------------------------

template <std::size_t I, typename... Ts>
auto get(const tuple<Ts...>& t) {
  return _tuple_get_t<I, Ts...>::get(t);
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
