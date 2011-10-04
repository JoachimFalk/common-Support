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

#ifndef _INCLUDED_COSUPPORT_MATH_TUPLE_BINOP_HPP
#define _INCLUDED_COSUPPORT_MATH_TUPLE_BINOP_HPP

#include "../../DataTypes/VectorInterface.hpp"

#include "exceptions.hpp"

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/static_assert.hpp>

namespace CoSupport { namespace Math { namespace Tuple {

namespace Detail {

  template <typename ITER1, typename ITER2, typename OP>
  class BinOpRandomAccessTraversalIter {
    typedef BinOpRandomAccessTraversalIter<ITER1, ITER2, OP> this_type;
  public:
    ITER1 iter1;
    ITER2 iter2;

    BinOpRandomAccessTraversalIter(const ITER1 &iter1, const ITER2 &iter2)
      : iter1(iter1), iter2(iter2) {}

    void advance(int n) { iter1 += n; iter2 += n; }
    int  distance_to(const this_type &rhs) const {
      assert(rhs.iter1-iter1 == rhs.iter2-iter2);
      return rhs.iter1-iter1;
    }
    bool equal(const this_type &rhs) const {
      assert((iter1==rhs.iter1) == (iter2==rhs.iter2));
      return  iter1==rhs.iter1;
    }

    typename OP::result_type deref() const { return OP::apply(iter1, iter2); }
  };

#define MAKE_BIN_OP(NAME,op)                                                  \
  template<class TA, class TB>                                                \
  struct NAME {                                                               \
    BOOST_TYPEOF_NESTED_TYPEDEF_TPL(nested, (*(TA*)(NULL)) op (*(TB*)(NULL))) \
    typedef typename nested::type result_type;                                \
                                                                              \
    template<class ITERA, class ITERB>                                        \
    static inline                                                             \
    result_type apply(const ITERA &aIter, const ITERB &bIter)                 \
      { return *aIter op *bIter; }                                            \
  };

  MAKE_BIN_OP(OpAdd, +);
  MAKE_BIN_OP(OpSub, -);
  MAKE_BIN_OP(OpMul, *);
  MAKE_BIN_OP(OpDiv, /);

#undef MAKE_BIN_OP

  template<class TA, class TB>
  struct OpMax {
    BOOST_STATIC_ASSERT((boost::is_same<TA, TB>::value));
    // Should be the same as TB. See BOOST_STATIC_ASSERT above.
    typedef TA result_type;

    template<class ITERA, class ITERB>
    static inline
    result_type apply(const ITERA &aIter, const ITERB &bIter)
      { return std::max(*aIter, *bIter); }
  };

  template<class TA, class TB>
  struct OpMin {
    BOOST_STATIC_ASSERT((boost::is_same<TA, TB>::value));
    // Should be the same as TB. See BOOST_STATIC_ASSERT above.
    typedef TA result_type;

    template<class ITERA, class ITERB>
    static inline
    result_type apply(const ITERA &aIter, const ITERB &bIter)
      { return std::min(*aIter, *bIter); }
  };

} // namespace Detail

template<class V1, class V2, template<class, class> class OP>
class BinOpVector: public DataTypes::VectorInterface<
    BinOpVector<V1, V2, OP>,
    Detail::BinOpRandomAccessTraversalIter<
      typename boost::remove_reference<V1>::type::const_iterator,
      typename boost::remove_reference<V2>::type::const_iterator,
      OP<typename boost::remove_reference<V1>::type::value_type,
         typename boost::remove_reference<V2>::type::value_type> >,
    // value type
    typename OP<typename boost::remove_reference<V1>::type::value_type,
                typename boost::remove_reference<V2>::type::value_type>::result_type,
    // reference type
    typename OP<typename boost::remove_reference<V1>::type::value_type,
                typename boost::remove_reference<V2>::type::value_type>::result_type,
    // const reference type
    typename boost::add_const<
      typename OP<typename boost::remove_reference<V1>::type::value_type,
                  typename boost::remove_reference<V2>::type::value_type>::result_type>::type,
    // pointer type
    typename boost::remove_reference<V1>::type::pointer, // FIXME
    // const pointer type
    typename boost::remove_reference<V1>::type::const_pointer // FIXME
  >
{
  typedef BinOpVector<V1, V2, OP> this_type;
  friend class DataTypes::VectorInterface<
      BinOpVector<V1, V2, OP>,
      Detail::BinOpRandomAccessTraversalIter<
        typename boost::remove_reference<V1>::type::const_iterator,
        typename boost::remove_reference<V2>::type::const_iterator,
        OP<typename boost::remove_reference<V1>::type::value_type,
           typename boost::remove_reference<V2>::type::value_type> >,
      // value type
      typename OP<typename boost::remove_reference<V1>::type::value_type,
                  typename boost::remove_reference<V2>::type::value_type>::result_type,
      // reference type
      typename OP<typename boost::remove_reference<V1>::type::value_type,
                  typename boost::remove_reference<V2>::type::value_type>::result_type,
      // const reference type
      typename boost::add_const<
        typename OP<typename boost::remove_reference<V1>::type::value_type,
                    typename boost::remove_reference<V2>::type::value_type>::result_type>::type,
      // pointer type
      typename boost::remove_reference<V1>::type::pointer, // FIXME
      // const pointer type
      typename boost::remove_reference<V1>::type::const_pointer // FIXME
    >;
public:
  BinOpVector(V1 v1, V2 v2): v1(v1), v2(v2) {
    if (v1.size() != v2.size())
      throw Exception::DifferentSize();
  }
protected:
  typedef Detail::BinOpRandomAccessTraversalIter<
    typename boost::remove_reference<V1>::type::const_iterator,
    typename boost::remove_reference<V2>::type::const_iterator,
    OP<typename boost::remove_reference<V1>::type::value_type,
       typename boost::remove_reference<V2>::type::value_type> > IterImpl;

  V1 v1;
  V2 v2;

  IterImpl first() const
    { return IterImpl(v1.begin(), v2.begin()); }
  IterImpl last() const
    { return IterImpl(v1.end(), v2.end()); }
};

} } } // namespace CoSupport::Math::Tuple

#endif //_INCLUDED_COSUPPORT_MATH_TUPLE_BINOP_HPP
