// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

/*
 * $Log: sassert.h,v $
 * Revision 1.3  2003/01/24 13:49:25  joachim
 * Now use new build system headers in cosupport/xxxx
 *
 * Revision 1.2  2001/01/08 15:25:55  joachim
 * Added sassert_h_rcsid
 *
 */
#ifndef _INCLUDED_COSUPPORT_DATATYPES_CONTAINER_INSERT_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_CONTAINER_INSERT_HPP

#include <cassert>
#include <utility>

namespace CoSupport { namespace DataTypes {

/// inserts a new value into a unique associative container,
/// checks the correct insertion and returns an iterator to the newly
/// inserted  (uses provided Value)
template<class UAC>
typename UAC::iterator
uac_insert(UAC &c, const typename UAC::value_type& v)
{
  std::pair<typename UAC::iterator, bool>
    ret = c.insert(v);
#ifndef NDEBUG
  assert(ret.second);
#endif
  return ret.first;
}

/// inserts a new value into a unique associative container,
/// checks the correct insertion and returns an iterator to the newly
/// inserted  (uses default Value)
template<class UAC>
typename UAC::iterator
uac_insert(UAC& c)
{
  std::pair<typename UAC::iterator, bool>
    ret = c.insert(UAC::value_type());
#ifndef NDEBUG
  assert(ret.second);
#endif
  return ret.first;
}

/// special case where containers contain pairs of key_type, mapped_type
/// (uses provided Key and Mapped>
template<class PAC, class K, class M>
typename PAC::iterator
pac_insert(PAC& c, const K& k, const M& m)
{
  return uac_insert(
      c, typename PAC::value_type(k, m));
}

/// special case where containers contain pairs of key_type, mapped_type
/// (uses default Mapped>
template<class PAC, class K>
typename PAC::iterator
pac_insert(PAC& c, const K& k)
{
  return uac_insert(
      c, typename PAC::value_type(k, typename PAC::mapped_type()));
}

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_CONTAINER_INSERT_HPP
