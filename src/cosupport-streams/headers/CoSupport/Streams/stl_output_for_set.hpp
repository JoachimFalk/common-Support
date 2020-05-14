// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STREAMS_STL_OUTPUT_FOR_SET_HPP
#define _INCLUDED_COSUPPORT_STREAMS_STL_OUTPUT_FOR_SET_HPP

#include <ostream>
//#include <set>

namespace CoSupport { namespace DataTypes {

  template <
    class DERIVED,                          // The derived set container being constructed
    template <class CONTAINER> class ITER,  // The iterator used by the derived set container
    class KEY,
    class REFERENCE,
    class CONSTREFERENCE,
    class PTR,
    class CONSTPTR
  >
  class SetInterface;

} } // namespace CoSupport::DataTypes

namespace std {
                                                                                
  template <typename T, class C, class A> class set;

  template <typename T, class C, class A>
  std::ostream &operator << (std::ostream &out, const std::set<T,C,A> &l) {
    out << "[Set:";
    for ( typename std::set<T,C,A>::const_iterator iter = l.begin();
          iter != l.end();
          ++iter )
      out << (iter == l.begin() ? "" : ", ") << *iter;
    out << "]";
    return out;
  }

  template<
    class D, template<class> class I,
    class V, class R, class CR, class P, class CP
  >
  std::ostream &operator << (
      std::ostream &out,
      const CoSupport::DataTypes::SetInterface<D,I,V,R,CR,P,CP> &l)
  {
    out << "[Set:";
    for (typename CoSupport::DataTypes::SetInterface<D,I,V,R,CR,P,CP>::const_iterator iter = l.begin();
         iter != l.end();
         ++iter )
      out << (iter == l.begin() ? "" : ", ") << *iter;
    out << "]";
    return out;
  }

} // namespace std

#endif // _INCLUDED_COSUPPORT_STREAMS_STL_OUTPUT_FOR_SET_HPP
