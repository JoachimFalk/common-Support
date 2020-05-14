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

#ifndef _INCLUDED_COSUPPORT_BOOST_PENDING_PROPERTY_HPP
#define _INCLUDED_COSUPPORT_BOOST_PENDING_PROPERTY_HPP

#include "../init_namespace.hpp"

#include <boost/pending/property.hpp>

#include <boost/mpl/bool.hpp>

namespace CoSupport { namespace boost {

template <typename PList, typename SearchTag>
struct is_property_present
  : public mpl::false_ {};

template <typename Tag, typename Value, typename Base, typename SearchTag>
struct is_property_present<boost::property<Tag, Value, Base>, SearchTag>
  : public is_property_present<Base, SearchTag> {};

template <typename SearchTag, typename Value, typename Base>
struct is_property_present<boost::property<SearchTag, Value, Base>, SearchTag>
  : public mpl::true_ {};

} } // namespace CoSupport::boost

#endif // _INCLUDED_COSUPPORT_BOOST_GRAPH_UNDIRECT_GRAPH_HPP
