// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2019 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_BOOST_PROPERTY_MAP_COMPOSE_PROPERTY_MAP_HPP
#define _INCLUDED_COSUPPORT_BOOST_PROPERTY_MAP_COMPOSE_PROPERTY_MAP_HPP

#include "../init_namespace.hpp"

#include <boost/property_map/property_map.hpp>

namespace CoSupport { namespace boost {

template <class FPMap, class GPMap>
class compose_property_map {
public:
  typedef typename boost::property_traits<GPMap>::key_type   key_type;
  typedef typename boost::property_traits<FPMap>::value_type value_type;
  typedef typename boost::property_traits<FPMap>::reference  reference;
  typedef typename boost::property_traits<FPMap>::category   category;

  compose_property_map(FPMap const &f, GPMap const &g)
    : f(f), g(g) {}

  reference operator[](const key_type &key) const
    { return get(f, get(g, key)); }
public:
  FPMap f;
  GPMap g;
};

using ::boost::get;
template <class FPMap, class GPMap>
typename boost::property_traits<FPMap>::reference
get(compose_property_map<FPMap, GPMap> const &pmap,
    typename boost::property_traits<GPMap>::key_type const &key)
  { return get(pmap.f, get(pmap.g, key)); }

using ::boost::put;
template <class FPMap, class GPMap>
void
put(compose_property_map<FPMap, GPMap> const &pmap,
    typename boost::property_traits<GPMap>::key_type   const &key,
    typename boost::property_traits<FPMap>::value_type const &value)
  { put(pmap.f, get(pmap.g, key), value); }

template <class FPMap, class GPMap>
compose_property_map<FPMap, GPMap>
make_compose_property_map(FPMap const &f, GPMap const &g)
  { return compose_property_map<FPMap, GPMap>(f, g); }

} } // namespace CoSupport::boost

#endif //_INCLUDED_COSUPPORT_BOOST_PROPERTY_MAP_COMPOSE_PROPERTY_MAP_HPP
