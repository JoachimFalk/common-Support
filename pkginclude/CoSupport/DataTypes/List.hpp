// vim: set sw=2 sts=2 ts=8 et syn=cpp:
/*
 * Copyright (c) 2011-2011 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_LIST_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_LIST_HPP

#include "ListInterface.hpp"

#include <list>

namespace CoSupport { namespace DataTypes {

namespace Detail {

  template <typename ITER>
  class BidirectionalTraversalIter {
    typedef BidirectionalTraversalIter<ITER> this_type;
  public:
    ITER iter;

    BidirectionalTraversalIter(const ITER &iter): iter(iter) {}

    void next() { ++iter; }
    void prev() { --iter; }
    bool equal(const this_type &rhs) const { return iter == rhs.iter; }

    typename std::iterator_traits<ITER>::reference deref() const { return *iter; }
  };

} // namespace Detail

template <typename T>
class List: public ListInterface<List<T>, Detail::BidirectionalTraversalIter<typename std::list<T>::iterator>, T> {
  typedef List<T> this_type;
private:
  friend class ListInterface<List<T>, Detail::BidirectionalTraversalIter<typename std::list<T>::iterator>, T>;
protected:
  std::list<T> list;

  Detail::BidirectionalTraversalIter<typename std::list<T>::iterator> first() const
    { return const_cast<this_type *>(this)->list.begin(); }
  Detail::BidirectionalTraversalIter<typename std::list<T>::iterator> last() const
    { return const_cast<this_type *>(this)->list.end(); }

  Detail::BidirectionalTraversalIter<typename std::list<T>::iterator> del(const Detail::BidirectionalTraversalIter<typename std::list<T>::iterator> &iter)
    { return list.erase(iter.iter); }
  Detail::BidirectionalTraversalIter<typename std::list<T>::iterator> add(const Detail::BidirectionalTraversalIter<typename std::list<T>::iterator> &iter, const typename this_type::value_type &value)
    { return list.insert(iter.iter, value); }
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_LIST_HPP
