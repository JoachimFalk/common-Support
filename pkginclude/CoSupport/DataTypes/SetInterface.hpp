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

#include "Iter/Detail/BidirectionalTraversalBase.hpp"

#include <set>

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
  class DERIVED,                          // The derived set container being constructed
  template <class CONTAINER> class ITER,  // The iterator used by the derived set container
  class KEY,
  class REFERENCE = typename boost::add_reference<KEY>::type,
  class CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<KEY>::type>::type,
  class PTR = typename boost::add_pointer<KEY>::type,
  class CONSTPTR = typename boost::add_pointer<typename boost::add_const<KEY>::type>::type
>
class SetInterface {
  typedef SetInterface                                     this_type;
private:
  // Curiously Recurring Template interface.
  DERIVED &derived()
    { return *static_cast<DERIVED *>(this); }

  DERIVED const &derived() const
    { return *static_cast<DERIVED const *>(this); }
protected:
  /// Base class for the iterator template given by ITER
  template <class CONTAINER>
  struct IterBase {
    typedef Iter::Detail::BidirectionalTraversalBase<CONTAINER> type;
  };
public:

  typedef KEY             value_type;
  typedef KEY             key_type;
  typedef REFERENCE       reference;
  typedef CONSTREFERENCE  const_reference;
  typedef PTR             pointer;
  typedef CONSTPTR        const_pointer;

  typedef ITER<const DERIVED> iterator;
  typedef ITER<const DERIVED> const_iterator;

  typedef std::ptrdiff_t                                        difference_type;
  typedef typename boost::make_unsigned<difference_type>::type  size_type;

  iterator begin()
    { return derived().implBegin(); }
  const_iterator begin() const
    { return derived().implBegin(); }

  iterator end()
    { return derived().implEnd(); }
  const_iterator end() const
    { return derived().implEnd(); }

  bool empty() const
    { return begin() == end(); }

  void erase(const iterator &iter)
    { derived().implErase(iter); }
  void erase(const iterator &iter1, const iterator &iter2)
    { derived().implErase(iter1, iter2); }

  // const key_type &k is correct here this is also used by std::set
  size_type erase(const key_type &k) {
    iterator iter(implFind(k));
    iterator end(derived().implEnd());
    if (iter == end) {
      return 0;
    } else {
      derived().implErase(iter);
      return 1;
    }
  }

  void clear() { erase(begin(), end()); }

  // const value_type &v is correct here this is also used by std::set
  std::pair<iterator, bool>
  insert(const value_type &v)
    { return derived().implInsert(v); }
  // const value_type &v is correct here this is also used by std::set
  iterator
  insert(iterator iter, const value_type &v)
    { return derived().implInsert(v).first; }
  template <class IITER>
  typename boost::disable_if<boost::is_integral<IITER>, void>::type
  insert(IITER iter1, IITER const &iter2) {
    for (; iter1 != iter2; ++iter1)
      derived().implInsert(*iter1);
  }

  // const key_type &k is correct here this is also used by std::set
  iterator       lower_bound(const key_type &k)
    { return derived().implLowerBound(k); }
  const_iterator lower_bound(const key_type &k) const
    { return derived().implLowerBound(k); }

  // const key_type &k is correct here this is also used by std::set
  iterator       upper_bound(const key_type &k)
    { return derived().implUpperBound(k); }
  // const key_type &k is correct here this is also used by std::set
  const_iterator upper_bound(const key_type &k) const
    { return derived().implUpperBound(k); }

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
    { return derived().implFind(k); }
  // const key_type &k is correct here this is also used by std::set
  const_iterator find(const key_type &k) const
    { return derived().implFind(k); }

  // const key_type &k is correct here this is also used by std::set
  size_type count(const key_type &k) const
    { return find(k) != end() ? 1 : 0; }

  size_type size() const
    { return implSize(); }

  template <typename X>
  operator std::set<X>() const
    { return std::set<X>(begin(), end()); }
protected:
  // Default implementation.
  size_t implSize() const {
    size_t   retval = 0;
    iterator iter(derived().implBegin());
    iterator end(derived().implEnd());
    for (; !(iter == end); ++iter)
      ++retval;
    return retval;
  }
  // Default implementation.
  void implErase(iterator iter1, const iterator &iter2) {
    while (!(iter1 == iter2)) {
      iterator iter1l = iter1; ++iter1;
      derived().implErase(iter1l);
    }
  }
  // Default implementation.
  iterator implLowerBound(const key_type &k) const {
    iterator iter(derived().implBegin());
    iterator end(derived().implEnd());
    for (; !(iter == end); ++iter)
      if (!(*iter < k))
        break;
    return iter;
  }
  // Default implementation.
  iterator implUpperBound(const key_type &k) const {
    iterator retval = implLowerBound(k);
    if (*retval == k)
      ++retval;
    return retval;
  }
  // Default implementation.
  iterator implFind(const key_type &k) const {
    iterator retval(implLowerBound(k));
    iterator end(derived().implEnd());
    if (!(retval == end) && !(*retval == k))
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
  template<class> class I1, template<class> class I2,
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
  template<class> class I1, template<class> class I2,
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
  template<class> class I1, template<class> class I2,
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
  template<class> class I1, template<class> class I2,
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
  template<class> class I1, template<class> class I2,
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
  template<class> class I1, template<class> class I2,
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
