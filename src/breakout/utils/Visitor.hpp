#ifndef BREAKOUT_UTILS_VISITOR_HPP
#define BREAKOUT_UTILS_VISITOR_HPP

// A helper class for visiting variants. This allows for
// defining a set of overloaded callables for a variant
// type, as opposed to a single callable capable of handling
// any variant type.
// See https://en.cppreference.com/w/cpp/utility/variant/visit
template<typename... Ts>
struct Visitor : Ts... { using Ts::operator()...; };
// An deduction guide for the Visitor class. This allows the
// compiler to deduce template arguments rather than requiring
// them to explicitly defined.
template<typename... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

#endif
