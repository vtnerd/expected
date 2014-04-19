// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2014 Vicente J. Botet Escriba

#ifndef BOOST_EXPECTED_MONADS_ALGORITHMS_HAVE_VALUE_HPP
#define BOOST_EXPECTED_MONADS_ALGORITHMS_HAVE_VALUE_HPP

#include <boost/config.hpp>
#include <boost/functional/monads/valued.hpp>
#include <utility>

namespace boost
{
namespace functional
{
namespace valued
{
  template < class M, class = std::enable_if<value_traits_t<M>::value> >
  BOOST_CONSTEXPR bool have_value( M&& m )
  {
    return has_value(std::forward<M>(m));
  }
  template< class M1, class ...Ms, class = std::enable_if<value_traits_t<M1>::value> >
  BOOST_CONSTEXPR bool have_value( M1&& m1, Ms&& ...ms )
  {
    return has_value(std::forward<M1>(m1)) && have_value( std::forward<Ms>(ms)... );
  }

}
}
}

#endif // BOOST_EXPECTED_MONADS_ALGORITHMS_HAVE_VALUE_HPP
