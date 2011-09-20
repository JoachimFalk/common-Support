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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VECTORINTERFACE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VECTORINTERFACE_HPP

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include "Detail/RandomAccessTraversalIterTemplate.hpp"

namespace CoSupport { namespace DataTypes {

/// VectorInterface is used as a public base class for defining new
/// standard-conforming, e.g., std::vector<VALUE>, vector containers.
/// The derived class DERIVED must implement .... FIXME.
/// These will be used by this template to implement the std::vector interface.
template<
  class DERIVED, // The derived vector container being constructed
  class ITER_,
  class VALUE,
  class REFERENCE = typename boost::add_reference<VALUE>::type,
  class CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<VALUE>::type>::type,
  class PTR_ = typename boost::add_pointer<VALUE>::type,
  class CONSTPTR_ = typename boost::add_pointer<typename boost::add_const<VALUE>::type>::type
>
class VectorInterface: protected Detail::RandomAccessTraversalIterTemplateAccess {
  typedef VectorInterface                                 this_type;
  typedef Detail::RandomAccessTraversalIterTemplateAccess base_type;

  friend class Detail::RandomAccessTraversalIterTemplate<this_type>;
  friend class Detail::RandomAccessTraversalIterTemplate<const this_type>;
private:
  // This is not a standard container type definition => hide it!
  typedef ITER_  IterImpl;      // for usage by Detail::BidirectionalTraversalIterTemplate
  //
  // Curiously Recurring Template interface.
  //
  DERIVED &derived()
    { return *static_cast<DERIVED *>(this); }

  DERIVED const &derived() const
    { return *static_cast<DERIVED const *>(this); }
  template<class P, class V, class R> class Iterator;
public:

  typedef VALUE           value_type;
  typedef REFERENCE       reference;
  typedef CONSTREFERENCE  const_reference;
  typedef PTR_            pointer;
  typedef CONSTPTR_       const_pointer;

  typedef Detail::RandomAccessTraversalIterTemplate<this_type>       iterator;
  typedef Detail::RandomAccessTraversalIterTemplate<const this_type> const_iterator;

  typedef std::ptrdiff_t                                        difference_type;
  typedef typename boost::make_unsigned<difference_type>::type  size_type;

  iterator begin()
    { return base_type::construct<iterator>(derived().first()); }
  const_iterator begin() const
    { return base_type::construct<const_iterator>(derived().first()); }

  iterator end()
    { return base_type::construct<iterator>(derived().last()); }
  const_iterator end() const
    { return base_type::construct<const_iterator>(derived().last()); }

  reference front()
    { return *begin(); }
  const_reference front() const
    { return *begin(); }

  reference back()
    { return *end(); }
  const_reference back() const
    { return *end(); }

  size_type size() const  { return end() - begin(); }
  bool      empty() const { return begin() == end(); }

  const_reference operator[](size_t i) const { return *(begin()+i); }
  reference       operator[](size_t i)       { return *(begin()+i); }

  iterator erase(const iterator &iter) {
    return base_type::construct<iterator>
      (derived().del(base_type::retrieve(iter)));
  }
  iterator erase(const iterator &iter1, const iterator &iter2) {
    return base_type::construct<iterator>
      (derived().delRange(base_type::retrieve(iter1), base_type::retrieve(iter2)));
  }

  // const value_type &v is correct here this is also used by std::list
  iterator insert(const iterator &iter, const value_type &v) {
    return base_type::construct<iterator>
      (derived().add(base_type::retrieve(iter), v));
  }

  void clear() { erase(begin(), end()); }

  // const value_type &v is correct here this is also used by std::list
  void push_back(const value_type &v) { insert(end(), v); }
  void pop_back()                     { erase(--end()); }
protected:
  // Default implementation. This should be overwritten for
  // efficiency reasons in DERIVED.
  IterImpl delRange(const IterImpl iter1, IterImpl iter2) {
    while (!iter1.equal(iter2)) {
      iter2.advance(-1); iter2 = derived().del(iter2);
    }
    return iter1;
  }
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VECTORINTERFACE_HPP
