// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_MATH_TUPLE_POVECTOR_HPP
#define _INCLUDED_COSUPPORT_MATH_TUPLE_POVECTOR_HPP

#include "PO.hpp"

#include <vector>

namespace CoSupport { namespace Math { namespace Tuple {

template <class T, class A = std::allocator<T> >
struct POVector { typedef PO<std::vector<T,A> > type; };

// specialize PO
template <class T, class A>
class PO<std::vector<T,A> >: public std::vector<T,A> {
  typedef std::vector<T,A> base_type;
  typedef PO<base_type>    this_type;
public:
  // Default constructor
  PO(const A &a = A()): base_type(a) {}
  PO(base_type const &v): base_type(v) {}
  PO(size_t n, const T &t = T(), const A &a = A()): base_type(n, t, a) {}

  template <class ITER>
  PO(ITER first, ITER last, const A &a = A()): base_type(first, last, a) {}

  // General copy constructor
  template <class B>
  PO(PO<B> const &v): base_type(v.begin(), v.end()) {}
};

} } } // namespace CoSupport::Math::Tuple

#endif // _INCLUDED_COSUPPORT_MATH_TUPLE_POVECTOR_HPP
