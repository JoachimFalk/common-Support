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

#include "../../DataTypes/Projection.hpp"
#include "../modulo.hpp"

#include "ScalarConstant.hpp"
#include "BinOp.hpp"
#include "UnOp.hpp"

#include <utility>
#include <ostream>
#include <limits>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/mpl/or.hpp>
#include <boost/utility/enable_if.hpp>

namespace CoSupport { namespace Math { namespace Tuple {

  template <class B> class PO;

  namespace Detail {

    template <typename T>
    struct is_po_type: public boost::false_type {};
    template <typename B>
    struct is_po_type<PO<B> >: public boost::true_type {};
    template <typename B>
    struct is_po_type<PO<B> const>: public boost::true_type {};

  } // namespace Detail

  template <class B>
  std::ostream &operator << (std::ostream &out, const PO<B> &t) {
    out << "[PO:";
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
  class PO<DataTypes::Projection<V, I> >: public DataTypes::Projection<V, I> {
    typedef DataTypes::Projection<V, I> base_type;
    typedef PO<base_type>               this_type;
  public:
    PO(V v, I i): base_type(v,i) {}

    using base_type::operator =;
  };

  template<class V, class I>
  inline
  PO<DataTypes::Projection<V const &, I const &> > proj(PO<V> const &v, I const &i)
    { return PO<DataTypes::Projection<V const &, I const &> >(v,i); }
  template<class V, class I>
  inline
  PO<DataTypes::Projection<V const &, I const &> > proj(PO<V> const &v, PO<I> const &i)
    { return PO<DataTypes::Projection<V const &, I const &> >(v,i); }
  template<class V, class I>
  inline
  PO<DataTypes::Projection<V       &, I const &> > proj(PO<V>       &v, I const &i)
    { return PO<DataTypes::Projection<V       &, I const &> >(v,i); }
  template<class V, class I>
  inline
  PO<DataTypes::Projection<V       &, I const &> > proj(PO<V>       &v, PO<I> const &i)
    { return PO<DataTypes::Projection<V       &, I const &> >(v,i); }

  using Math::div;

  // FIXME: negative vectors?!
  template <class B1, class B2>
  typename B1::value_type div(const PO<B1> &i, const PO<B2> &j) {
    typename PO<B1>::const_iterator iter = i.begin();
    typename PO<B1>::const_iterator iend = i.end();
    typename PO<B2>::const_iterator jter = j.begin();
    typename PO<B2>::const_iterator jend = j.end();
    typename B1::value_type ret = std::numeric_limits<typename B1::value_type>::max();
    for (;iter != iend && jter != jend; ++iter, ++jter) {
      ret = std::min(ret, div(*iter, static_cast<typename B1::value_type>(*jter)));
    }
    assert(iter == iend && jter == jend);
    return ret;
  }

  using Math::mod;

  // FIXME: negative vectors?!
  template <class B1, class B2>
  PO<BinOpVector<B1 const &, BinOpVector<B2 const &, ScalarConstant<typename B1::value_type>, Detail::OpMul>, Detail::OpSub> >
  mod(const PO<B1> &i, const PO<B2> &j) {
    return PO<
      BinOpVector<
        B1 const &,
        BinOpVector<
          B2 const &,
          ScalarConstant<typename B1::value_type>,
          Detail::OpMul>,
        Detail::OpSub> >(i, j * div(i,j));
  }

  // Support for pointwise +,-,*,/ of POs
  template <typename T>
  class PO<ScalarConstant<T> >: public ScalarConstant<T> {
  public:
    PO(T t, size_t n): ScalarConstant<T>(t,n) {}
  };

  template <class B1, class B2, template<class, class> class OP>
  class PO<BinOpVector<B1,B2,OP> >: public BinOpVector<B1,B2,OP> {
  public:
    PO(B1 lhs, B2 rhs): BinOpVector<B1,B2,OP>(lhs, rhs) {}
  };

  // Support for +
  template<class B1, class B2>
  inline
  PO<BinOpVector<B1 const &, B2 const &, Detail::OpAdd> > operator +(PO<B1> const &lhs, PO<B2> const &rhs)
    { return PO<BinOpVector<B1 const &, B2 const &, Detail::OpAdd> >(lhs, rhs); }
  template<class B>
  inline
  PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpAdd> > operator +(PO<B> const &lhs, typename B::value_type rhs) {
    return PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpAdd> >
      (lhs, PO<ScalarConstant<typename B::value_type> >(rhs, lhs.size()));
  }
  template<class B>
  inline
  PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpAdd> > operator +(typename B::value_type lhs, PO<B> const &rhs) {
    return PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpAdd> >
      (PO<ScalarConstant<typename B::value_type> >(lhs, rhs.size()), rhs);
  }

  template<class B1, class B2>
  inline
  PO<B1> &operator +=(PO<B1> &lhs, PO<B2> const &rhs) {
    typename PO<B1>::iterator       iter = lhs.begin();
    typename PO<B1>::iterator       iend = lhs.end();
    typename PO<B2>::const_iterator jter = rhs.begin();
    typename PO<B2>::const_iterator jend = rhs.end();
    for (;iter != iend && jter != jend; ++iter, ++jter)
      *iter += *jter;
    assert(iter == iend && jter == jend);
    return lhs;
  }
  template<class B>
  inline
  PO<B> &operator +=(PO<B> &lhs, typename B::value_type rhs) {
    typename PO<B>::iterator iter = lhs.begin();
    typename PO<B>::iterator iend = lhs.end();
    for (;iter != iend; ++iter)
      *iter += rhs;
    return lhs;
  }

  // Support for -
  template<class B1, class B2>
  inline
  PO<BinOpVector<B1 const &, B2 const &, Detail::OpSub> > operator -(PO<B1> const &lhs, PO<B2> const &rhs)
    { return PO<BinOpVector<B1 const &, B2 const &, Detail::OpSub> >(lhs, rhs); }
  template<class B>
  inline
  PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpSub> > operator -(PO<B> const &lhs, typename B::value_type rhs) {
    return PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpSub> >
      (lhs, PO<ScalarConstant<typename B::value_type> >(rhs, lhs.size()));
  }
  template<class B>
  inline
  PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpSub> > operator -(typename B::value_type lhs, PO<B> const &rhs) {
    return PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpAdd> >
      (PO<ScalarConstant<typename B::value_type> >(lhs, rhs.size()), rhs);
  }

  template<class B1, class B2>
  inline
  PO<B1> &operator -=(PO<B1> &lhs, PO<B2> const &rhs) {
    typename PO<B1>::iterator       iter = lhs.begin();
    typename PO<B1>::iterator       iend = lhs.end();
    typename PO<B2>::const_iterator jter = rhs.begin();
    typename PO<B2>::const_iterator jend = rhs.end();
    for (;iter != iend && jter != jend; ++iter, ++jter)
      *iter -= *jter;
    assert(iter == iend && jter == jend);
    return lhs;
  }
  template<class B>
  inline
  PO<B> &operator -=(PO<B> &lhs, typename B::value_type rhs) {
    typename PO<B>::iterator iter = lhs.begin();
    typename PO<B>::iterator iend = lhs.end();
    for (;iter != iend; ++iter)
      *iter -= rhs;
    return lhs;
  }

  // Support for *
  /*template<class B1, class B2>
  inline
  PO<BinOpVector<B1 const &, B2 const &, Detail::OpMul> > operator *(PO<B1> const &lhs, PO<B2> const &rhs)
    { return PO<BinOpVector<B1 const &, B2 const &, Detail::OpMul> >(lhs, rhs); }*/
  template<class B>
  inline
  PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpMul> > operator *(PO<B> const &lhs, typename B::value_type rhs) {
    return PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpMul> >
      (lhs, PO<ScalarConstant<typename B::value_type> >(rhs, lhs.size()));
  }
  template<class B>
  inline
  PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpMul> > operator *(typename B::value_type lhs, PO<B> const &rhs) {
    return PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpMul> >
      (PO<ScalarConstant<typename B::value_type> >(lhs, rhs.size()), rhs);
  }

  /*template<class B1, class B2>
  inline
  PO<B1> &operator *=(PO<B1> &lhs, PO<B2> const &rhs) {
    typename PO<B1>::iterator       iter = lhs.begin();
    typename PO<B1>::iterator       iend = lhs.end();
    typename PO<B2>::const_iterator jter = rhs.begin();
    typename PO<B2>::const_iterator jend = rhs.end();
    for (;iter != iend && jter != jend; ++iter, ++jter)
      *iter *= *jter;
    assert(iter == iend && jter == jend);
    return lhs;
  }*/
  template<class B>
  inline
  PO<B> &operator *=(PO<B> &lhs, typename B::value_type rhs) {
    typename PO<B>::iterator iter = lhs.begin();
    typename PO<B>::iterator iend = lhs.end();
    for (;iter != iend; ++iter)
      *iter *= rhs;
    return lhs;
  }

  // Support for /
  /*template<class B1, class B2>
  inline
  PO<BinOpVector<B1 const &, B2 const &, Detail::OpDiv> > operator /(PO<B1> const &lhs, PO<B2> const &rhs)
    { return PO<BinOpVector<B1 const &, B2 const &, Detail::OpDiv> >(lhs, rhs); }*/
  template<class B>
  inline
  PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpDiv> > operator /(PO<B> const &lhs, typename B::value_type rhs) {
    return PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpDiv> >
      (lhs, PO<ScalarConstant<typename B::value_type> >(rhs, lhs.size()));
  }

  // Support for unary negation
  template <class B, template<class> class OP>
  class PO<UnOpVector<B,OP> >: public UnOpVector<B,OP> {
  public:
    PO(B tuple): UnOpVector<B,OP>(tuple) {}
  };

  template <class B>
  inline
  PO<UnOpVector<B const &, Detail::OpNeg> > operator -(PO<B> const &tuple)
    { return PO<UnOpVector<B const &, Detail::OpNeg> >(tuple); }

  /*template<class B1, class B2>
  inline
  PO<B1> &operator /=(PO<B1> &lhs, PO<B2> const &rhs) {
    typename PO<B1>::iterator       iter = lhs.begin();
    typename PO<B1>::iterator       iend = lhs.end();
    typename PO<B2>::const_iterator jter = rhs.begin();
    typename PO<B2>::const_iterator jend = rhs.end();
    for (;iter != iend && jter != jend; ++iter, ++jter)
      *iter /= *jter;
    assert(iter == iend && jter == jend);
    return lhs;
  }*/
  template<class B>
  inline
  PO<B> &operator /=(PO<B> &lhs, typename B::value_type rhs) {
    typename PO<B>::iterator iter = lhs.begin();
    typename PO<B>::iterator iend = lhs.end();
    for (;iter != iend; ++iter)
      *iter /= rhs;
    return lhs;
  }

  // supremum (least upper bound) and infimum (greatest lower bound)
  template <class B1, class B2>
  inline
  PO<BinOpVector<B1 const &, B2 const &, Detail::OpMax> > supremum(PO<B1> const &lhs, PO<B2> const &rhs)
    { return PO<BinOpVector<B1 const &, B2 const &, Detail::OpMax> >(lhs, rhs); }
  template<class B>
  inline
  PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpMax> > supremum(PO<B> const &lhs, typename B::value_type rhs) {
    return PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpMax> >
      (lhs, PO<ScalarConstant<typename B::value_type> >(rhs, lhs.size()));
  }
  template<class B>
  inline
  PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpMax> > supremum(typename B::value_type lhs, PO<B> const &rhs) {
    return PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpMax> >
      (PO<ScalarConstant<typename B::value_type> >(lhs, rhs.size()), rhs);
  }

  template <class B1, class B2>
  inline
  PO<BinOpVector<B1 const &, B2 const &, Detail::OpMin> > infimum(PO<B1> const &lhs, PO<B2> const &rhs)
    { return PO<BinOpVector<B1 const &, B2 const &, Detail::OpMin> >(lhs, rhs); }
  template<class B>
  inline
  PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpMin> > infimum(PO<B> const &lhs, typename B::value_type rhs) {
    return PO<BinOpVector<B const &, ScalarConstant<typename B::value_type>, Detail::OpMin> >
      (lhs, PO<ScalarConstant<typename B::value_type> >(rhs, lhs.size()));
  }
  template<class B>
  inline
  PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpMin> > infimum(typename B::value_type lhs, PO<B> const &rhs) {
    return PO<BinOpVector<ScalarConstant<typename B::value_type>, B const &, Detail::OpMin> >
      (PO<ScalarConstant<typename B::value_type> >(lhs, rhs.size()), rhs);
  }

  // Partial order comparison operations

  // operator ==
  template <class B1, class B2>
  inline
  bool operator == (const PO<B1> &lhs, const PO<B2> &rhs) {
    typename PO<B1>::const_iterator iter = lhs.begin();
    typename PO<B1>::const_iterator iend = lhs.end();
    typename PO<B2>::const_iterator jter = rhs.begin();
    typename PO<B2>::const_iterator jend = rhs.end();
    while (iter != iend && jter != jend && *iter == *jter) {
      ++iter; ++jter;
    }
    return iter == iend && jter == jend;
  }
  template <class B>
  inline
  bool operator == (const PO<B> &lhs, typename B::value_type rhs) {
    typename PO<B>::const_iterator iter = lhs.begin();
    typename PO<B>::const_iterator iend = lhs.end();
    while (iter != iend && *iter == rhs)
      ++iter;
    return iter == iend;
  }
  template <class B>
  inline
  bool operator == (typename B::value_type lhs, const PO<B> &rhs) {
    typename PO<B>::const_iterator jter = rhs.begin();
    typename PO<B>::const_iterator jend = rhs.end();
    while (jter != jend && lhs == *jter)
      ++jter;
    return jter == jend;
  }

  // operator != based on operator ==
  template <class B1, class B2>
  inline
  bool operator != (const PO<B1> &lhs, const PO<B2> &rhs)
    { return !(lhs == rhs); }
  template <class B>
  inline
  bool operator != (const PO<B> &lhs, typename B::value_type rhs)
    { return !(lhs == rhs); }
  template <class B>
  inline
  bool operator != (typename B::value_type lhs, const PO<B> &rhs)
    { return !(lhs == rhs); }

  // operator <
  template <class B1, class B2>
  inline
  bool operator < (const PO<B1> &lhs, const PO<B2> &rhs) {
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
    assert(iter == iend && jter == jend);
    return less;
  }
  template <class B>
  inline
  bool operator < (const PO<B> &lhs, typename B::value_type rhs) {
    typename PO<B>::const_iterator iter = lhs.begin();
    typename PO<B>::const_iterator iend = lhs.end();
    bool less = false;
    while (iter != iend) {
      if (!(*iter <= rhs))
        return false;
      if (*iter < rhs)
        less = true;
      ++iter;
    }
    return less;
  }
  template <class B>
  inline
  bool operator < (typename B::value_type lhs, const PO<B> &rhs) {
    typename PO<B>::const_iterator jter = rhs.begin();
    typename PO<B>::const_iterator jend = rhs.end();
    bool less = false;
    while (jter != jend) {
      if (!(lhs <= *jter))
        return false;
      if (lhs < *jter)
        less = true;
      ++jter;
    }
    return less;
  }

  // operator > based on operator <
  template <class B1, class B2>
  inline
  bool operator > (const PO<B1> &lhs, const PO<B2> &rhs)
    { return rhs < lhs; }
  template <class B>
  inline
  bool operator > (const PO<B> &lhs, typename B::value_type rhs)
    { return rhs < lhs; }
  template <class B>
  inline
  bool operator > (typename B::value_type lhs, const PO<B> &rhs)
    { return rhs < lhs; }

  // operator <=
  template <class B1, class B2>
  inline
  bool operator <= (const PO<B1> &lhs, const PO<B2> &rhs) {
    typename PO<B1>::const_iterator iter = lhs.begin();
    typename PO<B1>::const_iterator iend = lhs.end();
    typename PO<B2>::const_iterator jter = rhs.begin();
    typename PO<B2>::const_iterator jend = rhs.end();
    while (iter != iend && jter != jend) {
      if (!(*iter <= *jter))
        return false;
      ++iter; ++jter;
    }
    assert(iter == iend && jter == jend);
    return true;
  }
  template <class B>
  inline
  bool operator <= (const PO<B> &lhs, typename B::value_type rhs) {
    typename PO<B>::const_iterator iter = lhs.begin();
    typename PO<B>::const_iterator iend = lhs.end();
    while (iter != iend) {
      if (!(*iter <= rhs))
        return false;
      ++iter;
    }
    return true;
  }
  template <class B>
  inline
  bool operator <= (typename B::value_type lhs, const PO<B> &rhs) {
    typename PO<B>::const_iterator jter = rhs.begin();
    typename PO<B>::const_iterator jend = rhs.end();
    while (jter != jend) {
      if (!(lhs <= *jter))
        return false;
      ++jter;
    }
    return true;
  }

  // operator >= based on operator <=
  template <class B1, class B2>
  inline
  bool operator >= (const PO<B1> &lhs, const PO<B2> &rhs)
    { return rhs <= lhs; }
  template <class B>
  inline
  bool operator >= (const PO<B> &lhs, typename B::value_type rhs)
    { return rhs <= lhs; }
  template <class B>
  inline
  bool operator >= (typename B::value_type lhs, const PO<B> &rhs)
    { return rhs <= lhs; }

} } } // namespace CoSupport::Math::Tuple

//#include <iostream>

namespace std {

  // we need this to be able to put PO<B> into std::map and std::set
  template<class B>
  struct less<CoSupport::Math::Tuple::PO<B> > {
    bool operator()(
        const CoSupport::Math::Tuple::PO<B> &lhs,
        const CoSupport::Math::Tuple::PO<B> &rhs) const
      { return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
  };

  /*
  template<class B, class X>
  struct less<pair<CoSupport::Math::Tuple::PO<B>,X> > {
    bool operator()(
        const pair<CoSupport::Math::Tuple::PO<B>,X> &lhs,
        const pair<CoSupport::Math::Tuple::PO<B>,X> &rhs)
    {
      cout << "bool less<pair<CoSupport::Math::Tuple::PO<B>,X> >::operator()(lhs,rhs)" << endl;
      return less<CoSupport::Math::Tuple::PO<B> >()(lhs.first, rhs.first) ||
        (!less<CoSupport::Math::Tuple::PO<B> >()(rhs.first, lhs.first) &&
          less<X>()(lhs.second, rhs.second));
    }
  };

  template<class X, class B>
  struct less<pair<X,CoSupport::Math::Tuple::PO<B> > > {
    bool operator()(
        const pair<X,CoSupport::Math::Tuple::PO<B> > &lhs,
        const pair<X,CoSupport::Math::Tuple::PO<B> > &rhs)
    {
      cout << "bool less<pair<X,CoSupport::Math::Tuple::PO<B> >::operator()(lhs,rhs)" << endl;
      return less<X>()(lhs.first, rhs.first) ||
        (!less<X>()(rhs.first, lhs.first) &&
          less<CoSupport::Math::Tuple::PO<B> >()(lhs.second, rhs.second));
    }
  };
   */

  // Disable operator <=, >=, <, and > for std::pairs containing at least one PO element.
  // The unmodified semantics of operator < defined in utility does not make any sense as
  // it mixes partial order with lexicographical order. So disable the operators to warn
  // of this conflict!

  // Operators intentionally left unimplemented to force linker error!
  // This will lead to an "ambiguous overload for »operator<=«" if pairs
  // with an PO element are compared!
  template<class TA, class TB>
  typename boost::enable_if<boost::mpl::or_<
    CoSupport::Math::Tuple::Detail::is_po_type<TA>,
    CoSupport::Math::Tuple::Detail::is_po_type<TB> >, bool>::type
  operator<=(const pair<TA, TB> &, const pair<TA, TB> &);
  // This will lead to an "ambiguous overload for »operator>=«" if pairs
  // with an PO element are compared!
  template<class TA, class TB>
  typename boost::enable_if<boost::mpl::or_<
    CoSupport::Math::Tuple::Detail::is_po_type<TA>,
    CoSupport::Math::Tuple::Detail::is_po_type<TB> >, bool>::type
  operator>=(const pair<TA, TB> &, const pair<TA, TB> &);
  // This will lead to an "ambiguous overload for »operator<«" if pairs
  // with an PO element are compared!
  template<class TA, class TB>
  typename boost::enable_if<boost::mpl::or_<
    CoSupport::Math::Tuple::Detail::is_po_type<TA>,
    CoSupport::Math::Tuple::Detail::is_po_type<TB> >, bool>::type
  operator< (const pair<TA, TB> &, const pair<TA, TB> &);
  // This will lead to an "ambiguous overload for »operator>«" if pairs
  // with an PO element are compared!
  template<class TA, class TB>
  typename boost::enable_if<boost::mpl::or_<
    CoSupport::Math::Tuple::Detail::is_po_type<TA>,
    CoSupport::Math::Tuple::Detail::is_po_type<TB> >, bool>::type
  operator> (const pair<TA, TB> &, const pair<TA, TB> &);

};

#endif // _INCLUDED_COSUPPORT_MATH_TUPLE_PO_HPP
