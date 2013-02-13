/*
 * Copyright (c) 2012-2012 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_SETINTERFACE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_SETINTERFACE_HPP

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <boost/utility/enable_if.hpp>

#include "Detail/BidirectionalTraversalIterTemplate.hpp"

namespace CoSupport { namespace DataTypes {

// WARNING: If you change SetInterface also adapt output operator in
// stl_output_for_set.hpp

/*
constructor/destructor
  set::set
  set::~set

member functions:

  set::begin
  set::cbegin           // this is C++11
  set::cend             // this is C++11
  set::clear
  set::count
  set::crbegin          // this is C++11
  set::crend            // this is C++11
  set::emplace          // this is C++11
  set::emplace_hint     // this is C++11
  set::empty
  set::end
  set::equal_range
  set::erase
  set::find
  set::get_allocator    // Ignore this for now
  set::insert
  set::key_comp         // Ignore this for now
  set::lower_bound
  set::max_size
  set::operator=
  set::rbegin           // Ignore this for now
  set::rend             // Ignore this for now
  set::size
  set::swap
  set::upper_bound
  set::value_comp       // Ignore this for now

non-member overloads:

  relational operators (set)
    template <class T, class Compare, class Alloc>
    bool operator== ( const set<T,Compare,Alloc>& lhs,
    const set<T,Compare,Alloc>& rhs );

    template <class T, class Compare, class Alloc>
    bool operator!= ( const set<T,Compare,Alloc>& lhs,
    const set<T,Compare,Alloc>& rhs );

    template <class T, class Compare, class Alloc>
    bool operator<  ( const set<T,Compare,Alloc>& lhs,
    const set<T,Compare,Alloc>& rhs );

    template <class T, class Compare, class Alloc>
    bool operator<= ( const set<T,Compare,Alloc>& lhs,
    const set<T,Compare,Alloc>& rhs );

    template <class T, class Compare, class Alloc>
    bool operator>  ( const set<T,Compare,Alloc>& lhs,
    const set<T,Compare,Alloc>& rhs );

    template <class T, class Compare, class Alloc>
    bool operator>= ( const set<T,Compare,Alloc>& lhs,
    const set<T,Compare,Alloc>& rhs );

  swap
*/

/// SetInterface is used as a public base class for defining new
/// standard-conforming, e.g., std::set<KEY>, set containers.  The derived class
/// DERIVED must implement the following protected methods which are required by
/// the SetInterface to implement the std::set interface:
/// \li implBegin()
/// \li implEnd()
/// \li implInsert(value v)
/// \li implErase(iter i)
///
/// The derived class DERIVED may implement the following protected methods for performance reasons:
/// \li implSize()
/// \li implLowerBound(key k)
/// \li implFind(key k)
/// \li implUpperBound(key k)
/// \li implErase(iter first, iter last)
template <
  class DERIVED, // The derived set container being constructed
  class ITER_,
  class KEY,
  class REFERENCE = typename boost::add_reference<KEY>::type,
  class CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<KEY>::type>::type,
  class PTR_ = typename boost::add_pointer<KEY>::type,
  class CONSTPTR_ = typename boost::add_pointer<typename boost::add_const<KEY>::type>::type
>
class SetInterface: protected Detail::BidirectionalTraversalIterTemplateAccess {
  typedef SetInterface                                     this_type;
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

  typedef KEY             value_type;
  typedef KEY             key_type;
  typedef REFERENCE       reference;
  typedef CONSTREFERENCE  const_reference;
  typedef PTR_            pointer;
  typedef CONSTPTR_       const_pointer;

  typedef Detail::BidirectionalTraversalIterTemplate<const this_type> iterator;
  typedef Detail::BidirectionalTraversalIterTemplate<const this_type> const_iterator;

  typedef std::ptrdiff_t                                        difference_type;
  typedef typename boost::make_unsigned<difference_type>::type  size_type;

  iterator begin()
    { return base_type::construct<iterator>(derived().implBegin()); }
  const_iterator begin() const
    { return base_type::construct<const_iterator>(derived().implBegin()); }
//const_iterator cbegin() const
//  { return base_type::construct<const_iterator>(derived().implBegin()); }

  iterator end()
    { return base_type::construct<iterator>(derived().implEnd()); }
  const_iterator end() const
    { return base_type::construct<const_iterator>(derived().implEnd()); }
//const_iterator cend() const
//  { return base_type::construct<const_iterator>(derived().implEnd()); }

  bool empty() const
    { return derived().implBegin().equal(derived().implEnd()); }

  void erase(const iterator &iter)
    { derived().implErase(base_type::retrieve(iter)); }
  void erase(const iterator &iter1, const iterator &iter2)
    { derived().implErase(base_type::retrieve(iter1), base_type::retrieve(iter2)); }

  // const key_type &k is correct here this is also used by std::set
  size_type erase(const key_type &k) {
    IterImpl iter(implFind(k));
    IterImpl end(derived().implEnd());
    if (iter.equal(end)) {
      return 0;
    } else {
      derived().implErase(iter);
      return 1;
    }
  }

  void clear() { erase(begin(), end()); }

  // const value_type &v is correct here this is also used by std::set
  std::pair<iterator, bool>
  insert(const value_type &v) {
    std::pair<IterImpl, bool> retval(derived().implInsert(v));
    return std::make_pair(
      base_type::construct<iterator>(retval.first),
      retval.second);
  }
  // const value_type &v is correct here this is also used by std::set
  iterator
  insert(iterator iter, const value_type &v) {
    return base_type::construct<iterator>(derived().implInsert(v).first);
  }
  template <class IITER>
  typename boost::disable_if<boost::is_integral<IITER>, void>::type
  insert(iterator iter, IITER implBegin, IITER const &implEnd) {
    for (; implBegin != implEnd; ++implBegin)
      derived().implInsert(*implBegin);
  }

  // const key_type &k is correct here this is also used by std::set
  iterator       lower_bound(const key_type &k)
    { return base_type::construct<iterator      >(derived().implLowerBound(k)); }
  // const key_type &k is correct here this is also used by std::set
  const_iterator lower_bound(const key_type &k) const
    { return base_type::construct<const_iterator>(derived().implLowerBound(k)); }

  // const key_type &k is correct here this is also used by std::set
  iterator       upper_bound(const key_type &k)
    { return base_type::construct<iterator      >(derived().implUpperBound(k)); }
  // const key_type &k is correct here this is also used by std::set
  const_iterator upper_bound(const key_type &k) const
    { return base_type::construct<const_iterator>(derived().implUpperBound(k)); }

  // const key_type &k is correct here this is also used by std::set
  std::pair<iterator, iterator>
  equal_range(const key_type &k) {
    std::pair<iterator, iterator> retval(lower_bound(k), iterator());
    retval.second = retval.first;
    if (*retval.first == k)
      ++retval.second;
    return retval;
  }
  // const key_type &k is correct here this is also used by std::set
  std::pair<const_iterator, const_iterator>
  equal_range(const key_type &k) const {
    std::pair<const_iterator, const_iterator> retval(lower_bound(k), const_iterator());
    retval.second = retval.first;
    if (*retval.first == k)
      ++retval.second;
    return retval;
  }

  // const key_type &k is correct here this is also used by std::set
  iterator       find(const key_type &k)
    { return base_type::construct<iterator      >(derived().implFind(k)); }
  const_iterator find(const key_type &k) const
    { return base_type::construct<const_iterator>(derived().implFind(k)); }

  // const key_type &k is correct here this is also used by std::set
  size_type count(const key_type &k) const
    { return find(k) != end() ? 1 : 0; }

  size_type size() const
    { return implSize(); }

protected:
  // Default implementation.
  size_t implSize() const {
    size_t   retval = 0;
    IterImpl iter(derived().implBegin());
    IterImpl end(derived().implEnd());
    for (; !iter.equal(end); iter.next())
      ++retval;
    return retval;
  }
  // Default implementation.
  void implErase(IterImpl iter1, const IterImpl &iter2) {
    while (!iter1.equal(iter2)) {
      IterImpl iter1l = iter1; iter1.next();
      derived().implErase(iter1l);
    }
  }
  // Default implementation.
  IterImpl implLowerBound(const key_type &k) const {
    IterImpl iter(derived().implBegin());
    IterImpl end(derived().implEnd());
    for (; !iter.equal(end); iter.next())
      if (!(iter.deref() < k))
        break;
    return iter;
  }
  // Default implementation.
  IterImpl implUpperBound(const key_type &k) const {
    IterImpl retval = implLowerBound(k);
    if (retval.deref() == k)
      retval.next();
    return retval;
  }
  // Default implementation.
  IterImpl implFind(const key_type &k) const {
    IterImpl retval(implLowerBound(k));
    IterImpl end(derived().implEnd());
    if (!retval.equal(end) && !(retval.deref() == k))
      retval = end;
    return retval;
  }
};

/**
 *  @brief  Set equality comparison.
 *  @param  lhs A %SetInterface.
 *  @param  rhs A %SetInterface of the same type as @a x.
 *  @return  True iff the size and elements of the SetInterfaces are equal.
 *
 *  This is an equivalence relation. It is linear in the size of
 *  the set. Sets are considered equivalent if their sizes are
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
    const SetInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const SetInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
{
  typename SetInterface<D1,I1,V,R1,CR1,P1,CP1>::const_iterator iter = lhs.begin();
  typename SetInterface<D1,I1,V,R1,CR1,P1,CP1>::const_iterator iend = lhs.end();
  typename SetInterface<D2,I2,V,R2,CR2,P2,CP2>::const_iterator jter = rhs.begin();
  typename SetInterface<D2,I2,V,R2,CR2,P2,CP2>::const_iterator jend = rhs.end();
  while (iter != iend && jter != jend && *iter == *jter) {
    ++iter; ++jter;
  }
  return iter == iend && jter == jend;
}

/**
 *  @brief  Set ordering relation.
 *  @param  x  A %SetInterface.
 *  @param  y  A %SetInterface of the same type as @a x.
 *  @return  True iff @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  set. The elements must be comparable with @c <.
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
    const SetInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const SetInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
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
    const SetInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const SetInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
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
    const SetInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const SetInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
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
    const SetInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const SetInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
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
    const SetInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const SetInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
  { return !(lhs < rhs); }

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_SETINTERFACE_HPP
