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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_ITER_DETAIL_BIDIRECTIONALTRAVERSALVIMPL_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_ITER_DETAIL_BIDIRECTIONALTRAVERSALVIMPL_HPP

#include "../BidirectionalTraversalVIf.hpp"

namespace CoSupport { namespace DataTypes { namespace Iter { namespace Detail {

/// Implementation of the BidirectionalTraversalVIf interface by a given iter of type ITER
template <class REFERENCE, class ITER>
class BidirectionalTraversalVImpl: public BidirectionalTraversalVIf<REFERENCE> {
  typedef BidirectionalTraversalVImpl<REFERENCE,ITER>   this_type;
  typedef BidirectionalTraversalVIf<REFERENCE>          ifac_type;
public:
  ITER iter;

  BidirectionalTraversalVImpl(ITER const &iter): iter(iter) {}

  void        increment()
    { ++iter; }
  void        decrement()
    { --iter; }
  bool        equal(ifac_type const &rhs) const
    { return iter == static_cast<this_type const &>(rhs).iter; }
  REFERENCE   dereference() const
    { return *iter; }
  ifac_type  *duplicate() const
    { return new this_type(iter); }
};

} } } } // namespace CoSupport::DataTypes::Iter::Detail

#endif // _INCLUDED_COSUPPORT_DATATYPES_ITER_DETAIL_BIDIRECTIONALTRAVERSALVIMPL_HPP
