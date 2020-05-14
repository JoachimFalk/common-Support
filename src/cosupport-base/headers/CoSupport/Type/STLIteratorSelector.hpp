// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_TYPE_STLITERATORSELECTOR_HPP
#define _INCLUDED_COSUPPORT_TYPE_STLITERATORSELECTOR_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>

namespace CoSupport { namespace Type {

/// Map iterator to const_iterator if the container type is const itself.
/// This behaviour is useful for generic programming if the implemented
/// algorithm is independent of the constness of the provided STL container.
template <typename Container>
struct STLIteratorSelector {
  typedef typename boost::mpl::if_<
      boost::is_const<Container>,
      typename Container::const_iterator,
      typename Container::iterator
    >::type type;
};

} } // namespace CoSupport::Type

#endif // _INCLUDED_COSUPPORT_TYPE_STLITERATORSELECTOR_HPP
