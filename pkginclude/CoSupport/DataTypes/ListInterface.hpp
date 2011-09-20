/*
 * Copyright (c) 2004-2011 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_LISTINTERFACE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_LISTINTERFACE_HPP

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include "Detail/BidirectionalTraversalIterTemplate.hpp"

namespace CoSupport { namespace DataTypes {

/// ListInterface is used as a public base class for defining new
/// standard-conforming, e.g., std::list<VALUE>, list containers.
/// The derived class DERIVED must implement first(), last(),
/// add(), and del() as protected methods. These will be used
/// by this template to implement the std::list interface.
template <
  class DERIVED, // The derived list container being constructed
  class ITER_,
  class VALUE,
  class REFERENCE = typename boost::add_reference<VALUE>::type,
  class CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<VALUE>::type>::type,
  class PTR_ = typename boost::add_pointer<VALUE>::type,
  class CONSTPTR_ = typename boost::add_pointer<typename boost::add_const<VALUE>::type>::type
>
class ListInterface: protected Detail::BidirectionalTraversalIterTemplateAccess {
  typedef ListInterface                                     this_type;
  typedef Detail::BidirectionalTraversalIterTemplateAccess  base_type;

  friend class Detail::BidirectionalTraversalIterTemplate<this_type>;
  friend class Detail::BidirectionalTraversalIterTemplate<const this_type>;
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
public:

  typedef VALUE           value_type;
  typedef REFERENCE       reference;
  typedef CONSTREFERENCE  const_reference;
  typedef PTR_            pointer;
  typedef CONSTPTR_       const_pointer;

  typedef Detail::BidirectionalTraversalIterTemplate<this_type>       iterator;
  typedef Detail::BidirectionalTraversalIterTemplate<const this_type> const_iterator;

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

  bool empty() const
    { return derived().begin() == derived().end(); }

  reference front()
    { return *derived().begin(); }
  const_reference front() const
    { return *derived().begin(); }

  reference back()
    { return *--derived().end(); }
  const_reference back() const
    { return *--derived().end(); }

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
  void push_back(const value_type &v)
    { derived().insert(derived().end(), v); }
  // const value_type &v is correct here this is also used by std::list
  void push_front(const value_type &v) 
    { derived().insert(derived().begin(), v); }

  void pop_back()
    { derived().erase(--derived().end()); }
  void pop_front()
    { derived().erase(derived().begin()); }

  size_t size() const {
    size_t retval = 0;
    
    for (const_iterator iter = derived().begin(); iter != derived().end(); ++iter)
      ++retval;
    return retval;
  }
/*
  template<class Predicate>
  const_iterator find_if(Predicate pred) const
    { return std::find_if(begin(), end(), pred); }
  
  template<class Predicate>
  iterator find_if(Predicate pred)
    { return std::find_if(begin(), end(), pred); }
  
  template<class Predicate>
  bool erase_if(Predicate pred) {
    iterator i = find_if(pred);
    if(i == end())
      return false;
    erase(i);
    return true;
  }
  
  template<class Predicate>
  typename Type::Const<pointer_type>::type lookup(Predicate pred) const {
    const_iterator i = find_if(pred);
    if(i == end())
      return NULL;
    typename Type::Const<reference_type>::type ref = *i;
    return &ref;
  }
  
  template<class Predicate>
  pointer_type lookup(Predicate pred) {
    iterator i = find_if(pred);
    if(i == end())
      return NULL;
    reference_type ref = *i;
    return &ref;
  }
*/
protected:
  // Default implementation.
  IterImpl delRange(IterImpl iter1, const IterImpl &iter2) {
    while (!iter1.equal(iter2))
      iter1 = derived().del(iter1);
    return iter1;
  }
};

/**
 *  @brief  List equality comparison.
 *  @param  lhs A %ListInterface.
 *  @param  rhs A %ListInterface of the same type as @a x.
 *  @return  True iff the size and elements of the ListInterfaces are equal.
 *
 *  This is an equivalence relation. It is linear in the size of
 *  the list. Lists are considered equivalent if their sizes are
 *  equal, and if corresponding elements compare equal.
 */
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class R1, class R2,
  class CR1, class CR2,
  class P1, class P2,
  class CP1, class CP2
>
inline
bool operator == (
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
{
  typename ListInterface<D1,I1,V,R1,CR1,P1,CP1>::const_iterator iter = lhs.begin();
  typename ListInterface<D1,I1,V,R1,CR1,P1,CP1>::const_iterator iend = lhs.end();
  typename ListInterface<D2,I2,V,R2,CR2,P2,CP2>::const_iterator jter = rhs.begin();
  typename ListInterface<D2,I2,V,R2,CR2,P2,CP2>::const_iterator jend = rhs.end();
  while (iter != iend && jter != jend && *iter == *jter) {
    ++iter; ++jter;
  }
  return iter == iend && jter == jend;
}

/**
 *  @brief  List ordering relation.
 *  @param  x  A %ListInterface.
 *  @param  y  A %ListInterface of the same type as @a x.
 *  @return  True iff @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  ListInterfaces.  The elements must be comparable with @c <.
 *
 *  See std::lexicographical_compare() for how the determination is made.
 */
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class R1, class R2,
  class CR1, class CR2,
  class P1, class P2,
  class CP1, class CP2
>
inline
bool operator < (
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
  { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

/// Based on operator==
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class R1, class R2,
  class CR1, class CR2,
  class P1, class P2,
  class CP1, class CP2
>
inline
bool operator != (
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
  { return !(lhs == rhs); }

/// Based on operator<
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class R1, class R2,
  class CR1, class CR2,
  class P1, class P2,
  class CP1, class CP2
>
inline
bool operator > (
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
  { return rhs < lhs; }

/// Based on operator<
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class R1, class R2,
  class CR1, class CR2,
  class P1, class P2,
  class CP1, class CP2
>
inline
bool operator <= (
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
  { return !(rhs < lhs); }

/// Based on operator<
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class R1, class R2,
  class CR1, class CR2,
  class P1, class P2,
  class CP1, class CP2
>
inline
bool operator >= (
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
  { return !(lhs < rhs); }

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_LISTINTERFACE_HPP
