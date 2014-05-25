// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2014 Vicente J. Botet Escriba

#ifndef BOOST_EXPECTED_ALGORITHMS_IF_THEN_ELSE_HPP
#define BOOST_EXPECTED_ALGORITHMS_IF_THEN_ELSE_HPP

#include <boost/expected/expected.hpp>
#include <boost/expected/algorithms/unwrap.hpp>

namespace boost
{
namespace expected_alg
{

  template <class V, class E, class T, class True, class False>
  auto if_then_else(expected<E,V,T> e, True&& t, False&& f)
  -> decltype(unwrap(e.map(std::forward<True>(t)).catch_error(std::forward<False>(f))))
  {
    return unwrap(e.map(std::forward<True>(t)).catch_error(std::forward<False>(f)));
  }

} // namespace expected_alg
} // namespace boost

#endif // BOOST_EXPECTED_ALGORITHMS_IF_THEN_ELSE_HPP
