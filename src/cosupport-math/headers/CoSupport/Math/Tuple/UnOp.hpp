// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2014 FAU -- Tobias Schwarzer <tobias.schwarzer@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_MATH_TUPLE_UNOP_HPP
#define _INCLUDED_COSUPPORT_MATH_TUPLE_UNOP_HPP

#include <CoSupport/compatibility-glue/nullptr.h>
#include <CoSupport/DataTypes/VectorInterface.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <boost/typeof/typeof.hpp>

namespace CoSupport { namespace Math { namespace Tuple {

namespace Detail {

  template <typename ITER, typename OP>
  class UnOpRandomAccessTraversalIter {
    typedef UnOpRandomAccessTraversalIter<ITER, OP> this_type;
  public:
    ITER iter;

    UnOpRandomAccessTraversalIter(const ITER &iter): iter(iter) {}

    void advance(int n) { iter += n; }
    int  distance_to(const this_type &rhs) const { return rhs.iter-iter; }
    bool equal(const this_type &rhs) const { return  iter==rhs.iter; }

    typename OP::result_type deref() const { return OP::apply(iter); }
  };

#define MAKE_UN_OP(NAME,op)                                                   \
  template<class T>                                                           \
  struct NAME {                                                               \
    BOOST_TYPEOF_NESTED_TYPEDEF_TPL(nested, op (*(T*)(nullptr)))                 \
    typedef typename nested::type result_type;                                \
                                                                              \
    template<class A>                                                         \
    static inline                                                             \
    result_type apply(const A &a) { return op *a; }                           \
  };

  MAKE_UN_OP(OpNeg, -)

#undef MAKE_UN_OP

} // namespace Detail

template<class V, template<class> class OP>
class UnOpVector: public DataTypes::VectorInterface<
    UnOpVector<V, OP>,
    Detail::UnOpRandomAccessTraversalIter<
      typename boost::remove_reference<V>::type::const_iterator,
      OP<typename boost::remove_reference<V>::type::value_type> >,
    // value type
    typename OP<typename boost::remove_reference<V>::type::value_type>::result_type,
    // reference type
    typename OP<typename boost::remove_reference<V>::type::value_type>::result_type,
    // const reference type
    typename boost::add_const<
      typename OP<typename boost::remove_reference<V>::type::value_type>::result_type>::type,
    // pointer type
    typename boost::remove_reference<V>::type::pointer, // FIXME
    // const pointer type
    typename boost::remove_reference<V>::type::const_pointer // FIXME
  >
{
  typedef UnOpVector<V, OP> this_type;
  friend class DataTypes::VectorInterface<
      UnOpVector<V, OP>,
      Detail::UnOpRandomAccessTraversalIter<
        typename boost::remove_reference<V>::type::const_iterator,
        OP<typename boost::remove_reference<V>::type::value_type> >,
      // value type
      typename OP<typename boost::remove_reference<V>::type::value_type>::result_type,
      // reference type
      typename OP<typename boost::remove_reference<V>::type::value_type>::result_type,
      // const reference type
      typename boost::add_const<
        typename OP<typename boost::remove_reference<V>::type::value_type>::result_type>::type,
      // pointer type
      typename boost::remove_reference<V>::type::pointer, // FIXME
      // const pointer type
      typename boost::remove_reference<V>::type::const_pointer // FIXME
    >;
public:
  UnOpVector(V v): v(v) {}
protected:
  typedef Detail::UnOpRandomAccessTraversalIter<
    typename boost::remove_reference<V>::type::const_iterator,
    OP<typename boost::remove_reference<V>::type::value_type> > IterImpl;

  V v;

  IterImpl first() const
    { return IterImpl(v.begin()); }
  IterImpl last() const
    { return IterImpl(v.end()); }
};

} } } // namespace CoSupport::Math::Tuple

#endif //_INCLUDED_COSUPPORT_MATH_TUPLE_UNOP_HPP
