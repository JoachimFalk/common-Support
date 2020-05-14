// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Christian Zebelein <christian.zebelein@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2014 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#ifndef INCLUDED_COSUPPORT_MODULO_HPP
#define INCLUDED_COSUPPORT_MODULO_HPP

#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/utility/enable_if.hpp>

#include <cassert>

namespace CoSupport { namespace Math {

// Invariants:
//   assert(m != 0)
//   mod(n,m) == n-div(n,m)*m
//   mod(n,m) \in \{0, 1,...m-1\} if m > 0
//   mod(n,m) \in \{m+1...,-1,0\} if m < 0
//   div(n,m) == (int) floor((double)n/m);

template <typename T>
typename boost::enable_if<boost::is_unsigned<T>, T>::type
mod(T n, T m) { assert(m != 0); return n % m; }

template <typename T>
typename boost::enable_if<boost::is_unsigned<T>, T>::type
div(T n, T m) { assert(m != 0); return n / m; }

template <typename T>
typename boost::enable_if<boost::is_signed<T>, T>::type
mod(T n, T m) {
  assert(m != 0);
#if -1/2 == 0
  // rounding towards zero
  T r = n % m;
  if (((r < 0) && (m > 0)) || ((r > 0) && (m < 0)))
    r += m;
  return r;
#elif -1/2 == -1
  // rounding down
  return n % m;
#else
# error "WTF?! -1/2 neither 0 nor -1!"
#endif
}

template <typename T>
typename boost::enable_if<boost::is_signed<T>, T>::type
div(T n, T m) {
  assert(m != 0);
#if -1/2 == 0
  // rounding towards zero
  if ((n >= 0) == (m >= 0))
    return n / m;
  else if (n < 0) // n  < 0 && m >= 0
    return (n+1)/m-1;
  else if (n > 0) // n  > 0 && m  < 0
    return (n-1)/m-1;
  else // n == 0
    return 0;
#elif -1/2 == -1
  // rounding down
  return n / m;
#else
# error "WTF?! -1/2 neither 0 nor -1!"
#endif
}

} } // namespace CoSupport::Math

#endif // INCLUDED_COSUPPORT_MODULO_HPP
