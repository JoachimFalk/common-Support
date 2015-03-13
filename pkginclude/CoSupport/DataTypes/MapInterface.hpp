/*
 * Copyright (c) 2015-2015 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAPINTERFACE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAPINTERFACE_HPP

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <boost/utility/enable_if.hpp>

#include "Iter/Detail/BidirectionalTraversalBase.hpp"

#include <map>

namespace CoSupport { namespace DataTypes {

// WARNING: If you change MapInterface also adapt output operator in
// stl_output_for_map.hpp

/*
constructor/destructor
  map::map
  map::~map

member functions:

  map::at               // (this is C++11)
  map::begin
  map::cbegin           // (this is C++11)
  map::cend             // (this is C++11)
  map::clear
  map::count
  map::crbegin          // Ignore this for now (this is C++11)
  map::crend            // Ignore this for now (this is C++11)
  map::emplace          // Ignore this for now (this is C++11)
  map::emplace_hint     // Ignore this for now (this is C++11)
  map::empty
  map::end
  map::equal_range
  map::erase
  map::find
  map::get_allocator    // Ignore this for now
  map::insert
  map::key_comp         // Ignore this for now
  map::lower_bound
  map::max_size
  map::operator=
  map::operator[]
  map::rbegin           // Ignore this for now
  map::rend             // Ignore this for now
  map::size
  map::swap
  map::upper_bound
  map::value_comp       // Ignore this for now

non-member overloads:

  relational operators (map)

    template <class Key, class T, class Compare, class Alloc>
    bool operator ==(const map<Key,T,Compare,Alloc> &lhs,
                     const map<Key,T,Compare,Alloc> &rhs);

    template <class Key, class T, class Compare, class Alloc>
    bool operator !=(const map<Key,T,Compare,Alloc> &lhs,
                     const map<Key,T,Compare,Alloc> &rhs);

    template <class Key, class T, class Compare, class Alloc>
    bool operator < (const map<Key,T,Compare,Alloc> &lhs,
                     const map<Key,T,Compare,Alloc> &rhs);

    template <class Key, class T, class Compare, class Alloc>
    bool operator <=(const map<Key,T,Compare,Alloc> &lhs,
                     const map<Key,T,Compare,Alloc> &rhs);

    template <class Key, class T, class Compare, class Alloc>
    bool operator > (const map<Key,T,Compare,Alloc> &lhs,
                     const map<Key,T,Compare,Alloc> &rhs);

    template <class Key, class T, class Compare, class Alloc>
    bool operator >=(const map<Key,T,Compare,Alloc> &lhs,
                     const map<Key,T,Compare,Alloc> &rhs);

  swap (map)

    template <class Key, class T, class Compare, class Alloc>
    void swap(map<Key,T,Compare,Alloc> &x, map<Key,T,Compare,Alloc> &y);

*/

/// MapInterface is used as a public base class for defining new
/// standard-conforming, e.g., std::map<KEY>, map containers.  The derived class
/// DERIVED must implement the following protected methods which are required by
/// the MapInterface to implement the std::map interface:
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
  class DERIVED,                          // The derived map container being constructed
  template <class CONTAINER> class ITER,  // The iterator used by the derived map container
  class KEY,
  class MAPPED,
  class COMPARE = typename std::less<KEY>,
  class MAPPED_REFERENCE = typename boost::add_reference<MAPPED>::type,
  class MAPPED_CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<MAPPED>::type>::type,
  class VALUE_REFERENCE = typename boost::add_reference<std::pair<KEY const, MAPPED> >::type,
  class VALUE_CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<std::pair<KEY const, MAPPED> >::type>::type,
  class VALUE_PTR = typename boost::add_pointer<std::pair<KEY const, MAPPED> >::type,
  class VALUE_CONSTPTR = typename boost::add_pointer<typename boost::add_const<std::pair<KEY const, MAPPED> >::type>::type
>
class MapInterface {
  typedef MapInterface                                     this_type;
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

  typedef MAPPED_REFERENCE              mapped_reference;
  typedef MAPPED_CONSTREFERENCE         mapped_const_reference;
public:

  typedef KEY                           key_type;
  typedef MAPPED                        mapped_type;
  typedef std::pair<KEY const, MAPPED>  value_type;
  typedef COMPARE                       key_compare;
//typedef _Alloc                        allocator_type; // Ignore this

  typedef VALUE_REFERENCE               reference;
  typedef VALUE_CONSTREFERENCE          const_reference;
  typedef VALUE_PTR                     pointer;
  typedef VALUE_CONSTPTR                const_pointer;

  typedef ITER<const DERIVED>           iterator;
  typedef ITER<const DERIVED>           const_iterator;

  typedef std::ptrdiff_t                                        difference_type;
  typedef typename boost::make_unsigned<difference_type>::type  size_type;

  class value_compare
  : public std::binary_function<value_type, value_type, bool>
  {
    friend class MapInterface<DERIVED, ITER, KEY, MAPPED, COMPARE, VALUE_REFERENCE, VALUE_CONSTREFERENCE, VALUE_PTR, VALUE_CONSTPTR>;
  protected:
    key_compare c;

    value_compare(key_compare const &c): c(c) {}
  public:
    // bounce comparison to key_compare
    bool operator()(const value_type &lhs, const value_type &rhs) const
      { return c(lhs.first, rhs.first); }
  };



  iterator begin()
    { return derived().implBegin(); }
  const_iterator begin() const
    { return derived().implBegin(); }
  const_iterator cbegin() const
    { return derived().implBegin(); }

  iterator end()
    { return derived().implEnd(); }
  const_iterator end() const
    { return derived().implEnd(); }
  const_iterator cend() const
    { return derived().implEnd(); }

  bool empty() const
    { return begin() == end(); }

  void erase(const iterator &iter)
    { derived().implErase(iter); }
  void erase(const iterator &iter1, const iterator &iter2)
    { derived().implErase(iter1, iter2); }

  // const key_type &k is correct here this is also used by std::map
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

  // const value_type &v is correct here this is also used by std::map
  std::pair<iterator, bool>
  insert(const value_type &v)
    { return derived().implInsert(v); }
  // const value_type &v is correct here this is also used by std::map
  iterator
  insert(iterator iter, const value_type &v)
    { return derived().implInsert(v).first; }
  template <class IITER>
  typename boost::disable_if<boost::is_integral<IITER>, void>::type
  insert(IITER iter1, IITER const &iter2) {
    for (; iter1 != iter2; ++iter1)
      derived().implInsert(*iter1);
  }

  // const key_type &k is correct here this is also used by std::map
  iterator       lower_bound(const key_type &k)
    { return derived().implLowerBound(k); }
  const_iterator lower_bound(const key_type &k) const
    { return derived().implLowerBound(k); }

  // const key_type &k is correct here this is also used by std::map
  iterator       upper_bound(const key_type &k)
    { return derived().implUpperBound(k); }
  // const key_type &k is correct here this is also used by std::map
  const_iterator upper_bound(const key_type &k) const
    { return derived().implUpperBound(k); }

  // const key_type &k is correct here this is also used by std::map
  std::pair<iterator, iterator>
  equal_range(const key_type &k) {
    std::pair<iterator, iterator> retval(lower_bound(k), iterator());
    retval.second = retval.first;
    if (*retval.first == k)
      ++retval.second;
    return retval;
  }
  // const key_type &k is correct here this is also used by std::map
  std::pair<const_iterator, const_iterator>
  equal_range(const key_type &k) const {
    std::pair<const_iterator, const_iterator> retval(lower_bound(k), const_iterator());
    retval.second = retval.first;
    if (*retval.first == k)
      ++retval.second;
    return retval;
  }

  // const key_type &k is correct here this is also used by std::map
  iterator       find(const key_type &k)
    { return derived().implFind(k); }
  // const key_type &k is correct here this is also used by std::map
  const_iterator find(const key_type &k) const
    { return derived().implFind(k); }

  // const key_type &k is correct here this is also used by std::map
  size_type count(const key_type &k) const
    { return find(k) != end() ? 1 : 0; }

  size_type size() const
    { return implSize(); }

  template <typename X>
  operator std::map<X>() const
    { return std::map<X>(begin(), end()); }
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
 *  @brief  Map equality comparison.
 *  @param  lhs A %map.
 *  @param  rhs A %map of the same type as @a x.
 *  @return  True iff the size and elements of the maps are equal.
 *
 *  This is an equivalence relation.  It is linear in the size of the
 *  maps.  Maps are considered equivalent if their sizes are equal,
 *  and if corresponding elements compare equal.
 */
template <
  class D1, class D2,
  template<class> class I1, template<class> class I2,
  class K, class M, class C,
  class MR1, class MR2,
  class MCR1, class MCR2,
  class VR1, class VR2,
  class VCR1, class VCR2,
  class VP1, class VP2,
  class VCP1, class VCP2
>
inline
bool operator == (
    const MapInterface<D1,I1,K,M,C,MR1,MCR1,VR1,VCR1,VP1,VCP1> &lhs,
    const MapInterface<D2,I2,K,M,C,MR2,MCR2,VR2,VCR2,VP2,VCP2> &rhs)
{
  typename MapInterface<D1,I1,K,M,C,MR1,MCR1,VR1,VCR1,VP1,VCP1>::const_iterator iter = lhs.begin();
  typename MapInterface<D1,I1,K,M,C,MR1,MCR1,VR1,VCR1,VP1,VCP1>::const_iterator iend = lhs.end();
  typename MapInterface<D2,I2,K,M,C,MR2,MCR2,VR2,VCR2,VP2,VCP2>::const_iterator jter = rhs.begin();
  typename MapInterface<D2,I2,K,M,C,MR2,MCR2,VR2,VCR2,VP2,VCP2>::const_iterator jend = rhs.end();
  while (iter != iend && jter != jend && *iter == *jter) {
    ++iter; ++jter;
  }
  return iter == iend && jter == jend;
}

/**
 *  @brief  Map ordering relation.
 *  @param  lhs A %map.
 *  @param  rhs A %map of the same type as @a x.
 *  @return  True iff @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  maps.  The elements must be comparable with @c <.
 *
 *  See std::lexicographical_compare() for how the determination is made.
 */
template <
  class D1, class D2,
  template<class> class I1, template<class> class I2,
  class K, class M, class C,
  class MR1, class MR2,
  class MCR1, class MCR2,
  class VR1, class VR2,
  class VCR1, class VCR2,
  class VP1, class VP2,
  class VCP1, class VCP2
>
inline
bool operator < (
    const MapInterface<D1,I1,K,M,C,MR1,MCR1,VR1,VCR1,VP1,VCP1> &lhs,
    const MapInterface<D2,I2,K,M,C,MR2,MCR2,VR2,VCR2,VP2,VCP2> &rhs)
  { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

/// Based on operator==
template <
  class D1, class D2,
  template<class> class I1, template<class> class I2,
  class V,
  class MR1, class MR2,
  class MCR1, class MCR2,
  class VR1, class VR2,
  class VCR1, class VCR2,
  class VP1, class VP2,
  class VCP1, class VCP2
>
inline
bool operator != (
    const MapInterface<D1,I1,K,M,C,MR1,MCR1,VR1,VCR1,VP1,VCP1> &lhs,
    const MapInterface<D2,I2,K,M,C,MR2,MCR2,VR2,VCR2,VP2,VCP2> &rhs)
  { return !(lhs == rhs); }

/// Based on operator<
template <
  class D1, class D2,
  template<class> class I1, template<class> class I2,
  class V,
  class MR1, class MR2,
  class MCR1, class MCR2,
  class VR1, class VR2,
  class VCR1, class VCR2,
  class VP1, class VP2,
  class VCP1, class VCP2
>
inline
bool operator > (
    const MapInterface<D1,I1,K,M,C,MR1,MCR1,VR1,VCR1,VP1,VCP1> &lhs,
    const MapInterface<D2,I2,K,M,C,MR2,MCR2,VR2,VCR2,VP2,VCP2> &rhs)
  { return rhs < lhs; }

/// Based on operator<
template <
  class D1, class D2,
  template<class> class I1, template<class> class I2,
  class V,
  class MR1, class MR2,
  class MCR1, class MCR2,
  class VR1, class VR2,
  class VCR1, class VCR2,
  class VP1, class VP2,
  class VCP1, class VCP2
>
inline
bool operator <= (
    const MapInterface<D1,I1,K,M,C,MR1,MCR1,VR1,VCR1,VP1,VCP1> &lhs,
    const MapInterface<D2,I2,K,M,C,MR2,MCR2,VR2,VCR2,VP2,VCP2> &rhs)
  { return !(rhs < lhs); }

/// Based on operator<
template <
  class D1, class D2,
  template<class> class I1, template<class> class I2,
  class V,
  class MR1, class MR2,
  class MCR1, class MCR2,
  class VR1, class VR2,
  class VCR1, class VCR2,
  class VP1, class VP2,
  class VCP1, class VCP2
>
inline
bool operator >= (
    const MapInterface<D1,I1,K,M,C,MR1,MCR1,VR1,VCR1,VP1,VCP1> &lhs,
    const MapInterface<D2,I2,K,M,C,MR2,MCR2,VR2,VCR2,VP2,VCP2> &rhs)
  { return !(lhs < rhs); }

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAPINTERFACE_HPP
