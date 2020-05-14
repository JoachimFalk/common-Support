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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_CONTAINER_RETRIEVE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_CONTAINER_RETRIEVE_HPP

#include <cassert>
#include <utility>

namespace CoSupport { namespace DataTypes {

/// Retrieve value from an associative container.
/// The value must exist.
template<class AC>
typename std::iterator_traits<typename AC::iterator>::reference
ac_retrieve(AC &c, const typename AC::value_type &v) {
  typename AC::iterator iter = c.find(v);
  assert(iter != c.end());
  return *iter;
}
template<class AC>
typename std::iterator_traits<typename AC::iterator>::const_reference
ac_retrieve(AC const &c, const typename AC::value_type &v) {
  typename AC::const_iterator iter = c.find(v);
  assert(iter != c.end());
  return *iter;
}

/// Retrieve value from a pair associative container.
/// The value must exist.
template<class PAC>
typename PAC::mapped_type &
pac_retrieve(PAC &c, const typename PAC::key_type &v) {
  typename PAC::iterator iter = c.find(v);
  assert(iter != c.end());
  return iter->second;
}
template<class PAC>
typename PAC::mapped_type const &
pac_retrieve(PAC const &c, const typename PAC::key_type &v) {
  typename PAC::const_iterator iter = c.find(v);
  assert(iter != c.end());
  return iter->second;
}

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_CONTAINER_RETRIEVE_HPP
