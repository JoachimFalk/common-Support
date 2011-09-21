/* vim: set sw=2 ts=8: */
/*
 * Copyright (c) 2011 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_MATH_TUPLE_PO_HPP
#define _INCLUDED_COSUPPORT_MATH_TUPLE_PO_HPP

#include "exceptions.hpp"

namespace CoSupport { namespace Math { namespace Tuple {

template <class B> class PO;

template <class B1, class B2>
inline
bool operator == (const PO<B1> &lhs, const PO<B2> &rhs)
{
  typename PO<B1>::const_iterator iter = lhs.begin();
  typename PO<B1>::const_iterator iend = lhs.end();
  typename PO<B2>::const_iterator jter = rhs.begin();
  typename PO<B2>::const_iterator jend = rhs.end();
  while (iter != iend && jter != jend && *iter == *jter) {
    ++iter; ++jter;
  }
  return iter == iend && jter == jend;
}

/// Based on operator ==
template <class B1, class B2>
inline
bool operator != (const PO<B1> &lhs, const PO<B2> &rhs)
  { return !(lhs == rhs); }

template <class B1, class B2>
inline
bool operator < (const PO<B1> &lhs, const PO<B2> &rhs)
{
  typename PO<B1>::const_iterator iter = lhs.begin();
  typename PO<B1>::const_iterator iend = lhs.end();
  typename PO<B2>::const_iterator jter = rhs.begin();
  typename PO<B2>::const_iterator jend = rhs.end();
  bool less = false;
  while (iter != iend && jter != jend) {
    if (!(*iter <= *jter))
      return false;
    if (*iter < *jter)
      less = true;
    ++iter; ++jter;
  }
  if (iter != iend || jter != jend)
    throw Exception::DifferentSize();
  return less;
}

/// Based on operator <
template <class B1, class B2>
inline
bool operator > (const PO<B1> &lhs, const PO<B2> &rhs)
  { return rhs < lhs; }

template <class B1, class B2>
inline
bool operator <= (const PO<B1> &lhs, const PO<B2> &rhs)
{
  typename PO<B1>::const_iterator iter = lhs.begin();
  typename PO<B1>::const_iterator iend = lhs.end();
  typename PO<B2>::const_iterator jter = rhs.begin();
  typename PO<B2>::const_iterator jend = rhs.end();
  while (iter != iend && jter != jend) {
    if (!(*iter <= *jter))
      return false;
    ++iter; ++jter;
  }
  if (iter != iend || jter != jend)
    throw Exception::DifferentSize();
  return true;
}

/// Based on operator <=
template <class B1, class B2>
inline
bool operator >= (const PO<B1> &lhs, const PO<B2> &rhs)
  { return rhs <= lhs; }

} } } // namespace CoSupport::Math::Tuple

#endif // _INCLUDED_COSUPPORT_MATH_TUPLE_PO_HPP
