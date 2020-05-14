// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_TYPE_CONSTPROPAGATE_HPP
#define _INCLUDED_COSUPPORT_TYPE_CONSTPROPAGATE_HPP

#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>

namespace CoSupport { namespace Type {

  template <typename TSRC, typename TDEST>
  struct ConstPropagate {
    typedef typename boost::mpl::if_<boost::is_const<TSRC>,
      typename boost::add_const<TDEST>::type,
      TDEST>::type  type;
  };

} } // namespace CoSupport::Type

#endif // _INCLUDED_COSUPPORT_TYPE_CONSTPROPAGATE_HPP
