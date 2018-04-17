#ifndef __YAVIN_TUPLE_H__
#define __YAVIN_TUPLE_H__

//==============================================================================
namespace Yavin {
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
}  // namespace Yavin
//==============================================================================

#endif
