/*
 * Copyright (c) 2014-2014 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifndef INCLUDED_COSUPPORT_MODULO_HPP
#define INCLUDED_COSUPPORT_MODULO_HPP

#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/utility/enable_if.hpp>

namespace CoSupport { namespace Math {

// Invariants:
//   assert(m != 0)
//   mod(n,m) == n-div(n,m)*m
//   mod(n,m) \in \{0, 1,...m-1\} if m > 0
//   mod(n,m) \in \{m+1...,-1,0\} if m < 0
//   div(n,m) == (int) floor((double)n/m);

template <typename T>
typename boost::enable_if<boost::is_unsigned<T>, T>::type
mod(T n, T m) { return n % m; }

template <typename T>
typename boost::enable_if<boost::is_unsigned<T>, T>::type
div(T n, T m) { return n / m; }

template <typename T>
typename boost::enable_if<boost::is_signed<T>, T>::type
mod(T n, T m) {
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
