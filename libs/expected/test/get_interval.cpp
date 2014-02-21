//! \file num_get_3.cpp

// Copyright Vicente J. Botet Escriba 2014.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <boost/expected/expected.hpp>
#include <iostream>
#include <streambuf>
#include <locale>
#include <sstream>
#include <type_traits>

#define expect(V, EXPR) \
auto BOOST_JOIN(expected,V) = EXPR; \
if (! BOOST_JOIN(expected,V).valid()) return BOOST_JOIN(expected,V).get_exceptional(); \
auto V =BOOST_JOIN(expected,V).value()


#define expect_void(V, EXPR) \
auto V = EXPR; \
if (! V.valid()) return V.get_exceptional(); \

template <class CharT=char, class InputIterator = std::istreambuf_iterator<CharT> >
struct ios_range {
  InputIterator begin;
  InputIterator end;
  std::ios_base& ios;
  ios_range(InputIterator f, InputIterator e, std::ios_base& ios)
  : begin(f), end(e), ios(ios) {}
  ios_range(std::basic_stringstream<CharT>& is)
  : begin(is), end(), ios(is) {}
};


/**
 *
 * @param r the range
 * @Returns the expect Num value if the parse succeeds or the iostate is the parse fails.
 */

template <class Num, class CharT=char, class InputIterator = std::istreambuf_iterator<CharT> >
boost::expected<Num, std::ios_base::iostate> get_num(ios_range<CharT, InputIterator>& r) {

  std::ios_base::iostate err = std::ios_base::goodbit;
  Num v;
  r.begin = std::use_facet<std::num_get<CharT, InputIterator> >(r.ios.getloc()).get(r.begin, r.end, r.ios, err, v);
  if (err & (std::ios_base::badbit | std::ios_base::failbit)) {
    return boost::make_unexpected_error(err);
  }
  return v;
}

// todo finish this function
template <class CharT=char, class InputIterator = std::istreambuf_iterator<CharT> >
boost::expected<void, std::ios_base::iostate>
matchedString(std::string str, ios_range<CharT, InputIterator>& r) {
  if (*r.begin != str[0]) {
      return boost::make_unexpected_error(std::ios_base::goodbit);
  }
  ++r.begin;
  if (*r.begin != str[1]) {
      return boost::make_unexpected_error(std::ios_base::goodbit);
  }
  ++r.begin;
  return boost::expected<void, std::ios_base::iostate>();
}

/**
 *
 * @param r the range
 * @Returns the expect pair<Num,Num> if the parse succeeds or the iostate is the parse fails.
 */

template <class Num, class CharT=char, class InputIterator = std::istreambuf_iterator<CharT> >
boost::expected<std::pair<Num,Num>, std::ios_base::iostate> get_interval(ios_range<CharT, InputIterator>& r) {
  auto  f = get_num<Num>(r);
  if (! f.valid()) return f.get_exceptional();

  auto  m = matchedString("..", r);
  if (! m.valid()) return m.get_exceptional();

  auto  l = get_num<Num>(r);
  if (! l.valid()) return l.get_exceptional();

  return std::make_pair(f.value(), l.value());
}

template <class Num, class CharT=char, class InputIterator = std::istreambuf_iterator<CharT> >
boost::expected<std::pair<Num,Num>, std::ios_base::iostate> get_interval2(ios_range<CharT, InputIterator>& r)
{
  expect(f, get_num<Num>(r));
  expect_void(m, matchedString("..", r));
  expect(l, get_num<Num>(r));
  return std::make_pair(f, l);
}

#if 0
template <class Num, class CharT=char, class InputIterator = std::istreambuf_iterator<CharT> >
boost::expected<std::pair<Num,Num>, std::ios_base::iostate> get_interval2(ios_range<CharT, InputIterator>& r)
{
  auto f = expect get_num<Num>(r);
           expect matchedString("..", r);
  auto l = expect get_num<Num>(r);
  return std::make_pair(f, l);
}
#endif

int main()
{
  {
    std::stringstream is("1..3");
    ios_range<> r(is);
    auto x = get_interval<long>(r);
    if (!x.valid()) {
      std::cout << x.error() << std::endl;
      return 1;
    }
    std::cout << x.value().first << ".." << x.value().second << std::endl;
  }

  {
    std::stringstream is("1..3");
    ios_range<> r(is);
    auto x = get_interval2<long>(r);
    if (!x.valid()) {
      std::cout << x.error() << std::endl;
      return 1;
    }

    std::cout << x.value().first << ".." << x.value().second << std::endl;
  }

  return 0;
}
