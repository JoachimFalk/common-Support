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
#include "Projection.hpp"
#include "BinOp.hpp"
//#include "../../DataTypes/EMPTY.hpp"

#include <utility>
#include <ostream>

//#include <boost/type_traits/is_base_of.hpp>
//#include <boost/utility/enable_if.hpp>

namespace CoSupport { namespace Math { namespace Tuple {

//class POTag;

template <class B> class PO;

template <class B>
std::ostream &operator << (std::ostream &out, const PO<B> &t) {
  out << "[";
  for (typename PO<B>::const_iterator iter = t.begin(); iter != t.end(); ++iter) {
    if (iter != t.begin())
      out << ", ";
    out << *iter;
  }
  out << "]";
  return out;
}

// Support for projection of POs
template <class V, class I>
class PO<Projection<V, I> >: public Projection<V, I> {
public:
  PO(V v, I i): Projection<V, I>(v,i) {}
};

template<class V, class I>
inline
PO<Projection<V const &, I const &> > proj(PO<V> const &v, I const &i)
  { return PO<Projection<V const &, I const &> >(v,i); }
template<class V, class I>
inline
PO<Projection<V       &, I const &> > proj(PO<V>       &v, I const &i)
  { return PO<Projection<V       &, I const &> >(v,i); }

// Support for pointwise +,-,*,/ of POs
template <class V1, class V2, template<class, class> class OP>
class PO<BinOpVector<V1,V2,OP> >: public BinOpVector<V1,V2,OP> {
public:
  PO(V1 v1, V2 v2): BinOpVector<V1,V2,OP>(v1, v2) {}
};

template<class V1, class V2>
inline
PO<BinOpVector<V1 const &, V2 const &, Detail::OpAdd> > operator +(PO<V1> const &v1, PO<V2> const &v2)
  { return PO<BinOpVector<V1 const &, V2 const &, Detail::OpAdd> >(v1, v2); }


// Partial order comparison operations
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

namespace std {

  // we need this to be able to put PO<B> into std::map and std::set
  template<class B>
  struct less<CoSupport::Math::Tuple::PO<B> > {
    bool operator()(const CoSupport::Math::Tuple::PO<B> &lhs, const CoSupport::Math::Tuple::PO<B> &rhs) const {
//    std::cout << "bool std::less<CoSupport::Math::Tuple::PO<B> >::operator()(a,b)" << std::endl;
      return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
  };

  template<class B, class X>
  struct less<std::pair<CoSupport::Math::Tuple::PO<B>, X> > {
    bool operator()(
        const std::pair<CoSupport::Math::Tuple::PO<B>, X> &lhs,
        const std::pair<CoSupport::Math::Tuple::PO<B>, X> &rhs)
    {
      return std::less<CoSupport::Math::Tuple::PO<B> >()(lhs.first, rhs.first) ||
        (!std::less<CoSupport::Math::Tuple::PO<B> >()(rhs.first, lhs.first) &&
          std::less<X>()(lhs.second, rhs.second));
    }
  };

  template<class B, class X>
  struct less<std::pair<X, CoSupport::Math::Tuple::PO<B> > > {
    bool operator()(
        const std::pair<X, CoSupport::Math::Tuple::PO<B> > &lhs,
        const std::pair<X, CoSupport::Math::Tuple::PO<B> > &rhs)
    {
      return std::less<X>()(lhs.first, rhs.first) ||
        (!std::less<X>()(rhs.first, lhs.first) &&
          std::less<CoSupport::Math::Tuple::PO<B> >()(lhs.second, rhs.second));
    }
  };

};

#endif // _INCLUDED_COSUPPORT_MATH_TUPLE_PO_HPP
