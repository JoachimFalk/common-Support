/*
 * Copyright (c) 2013-2013 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
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
