// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2010 FAU -- Nina Muehleis <nina.muehleis@cs.fau.de>
 *   2010 FAU -- Sebastian Graf <sebastian.graf@fau.de>
 *   2011 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_ITER_DETAIL_BIDIRECTIONALTRAVERSALBASE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_ITER_DETAIL_BIDIRECTIONALTRAVERSALBASE_HPP

#include <boost/iterator/iterator_facade.hpp>

#include "../../../Type/STLIteratorSelector.hpp"
#include "../../../Type/STLReferenceSelector.hpp"
#include "../../../Type/STLPointerSelector.hpp"

namespace CoSupport { namespace DataTypes { namespace Iter { namespace Detail {

template <class CONTAINER>
class BidirectionalTraversalBase
: public boost::iterator_facade<
    typename Type::STLIteratorSelector<CONTAINER>::type,
    typename CONTAINER::value_type,
    boost::bidirectional_traversal_tag,
    typename Type::STLReferenceSelector<CONTAINER>::type,
    typename CONTAINER::difference_type>
{
  typedef BidirectionalTraversalBase<CONTAINER> this_type;
public:
  // overwrite pointer type from boost which thinks it knows best
  typedef typename Type::STLPointerSelector<CONTAINER>::type pointer;

  // overwrite operator -> from boost which thinks it knows best
  pointer operator->() const {
    typename this_type::reference ref =
      static_cast<typename Type::STLIteratorSelector<CONTAINER>::type const *>(this)
      ->dereference();
    return &ref;
  }
};

} } } } // namespace CoSupport::DataTypes::Iter::Detail

#endif // _INCLUDED_COSUPPORT_DATATYPES_ITER_DETAIL_BIDIRECTIONALTRAVERSALBASE_HPP
