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
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <boost/utility/enable_if.hpp>

#include "Iter/Detail/BidirectionalTraversalBase.hpp"

#include <list>

namespace CoSupport { namespace DataTypes {

// WARNING: If you change ListInterface also adapt output operator in
// stl_output_for_list.hpp

/// ListInterface is used as a public base class for defining new
/// standard-conforming, e.g., std::list<VALUE>, list containers.
/// The derived class DERIVED must implement the following protected
/// methods which are required by the ListInterface to implement the
/// std::list interface:
/// \li implBegin()
/// \li implEnd()
/// \li implInsert()
/// \li del()
template <
  class DERIVED,                          // The derived list container being constructed
  template <class CONTAINER> class ITER,  // The iterator used by the derived list container
  class VALUE,
  class REFERENCE = typename boost::add_reference<VALUE>::type,
  class CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<VALUE>::type>::type,
  class PTR_ = typename boost::add_pointer<VALUE>::type,
  class CONSTPTR_ = typename boost::add_pointer<typename boost::add_const<VALUE>::type>::type
>
class ListInterface {
  typedef ListInterface                                     this_type;
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

  typedef VALUE           value_type;
  typedef REFERENCE       reference;
  typedef CONSTREFERENCE  const_reference;
  typedef PTR_            pointer;
  typedef CONSTPTR_       const_pointer;

  typedef ITER<DERIVED>       iterator;
  typedef ITER<DERIVED const> const_iterator;

  typedef std::ptrdiff_t                                        difference_type;
  typedef typename boost::make_unsigned<difference_type>::type  size_type;

  iterator begin()
    { return derived().implBegin(); }
  const_iterator begin() const
    { return const_cast<this_type *>(this)->derived().implBegin(); }
  const_iterator cbegin() const
    { return const_cast<this_type *>(this)->derived().implBegin(); }

  iterator end()
    { return derived().implEnd(); }
  const_iterator end() const
    { return const_cast<this_type *>(this)->derived().implEnd(); }
  const_iterator cend() const
    { return const_cast<this_type *>(this)->derived().implEnd(); }

  bool empty() const
    { return begin() == end(); }

  reference front()
    { return *begin(); }
  const_reference front() const
    { return *begin(); }

  reference back()
    { return *--end(); }
  const_reference back() const
    { return *--end(); }

  iterator erase(const iterator &iter)
    { return derived().implErase(iter); }
  iterator erase(const iterator &iter1, const iterator &iter2)
    { return derived().implErase(iter1, iter2); }

  // const value_type &v is correct here this is also used by std::list
  iterator insert(const iterator &iter, const value_type &v) {
    return derived().implInsert(iter, v);
  }
  // const value_type &v is correct here this is also used by std::vector
  void insert(iterator iter, size_type n, const value_type &v) {
    for (;n > 0; --n)
      iter = insert(iter, v);
  }
  template <class IITER>
  typename boost::disable_if<boost::is_integral<IITER>, void>::type
  insert(iterator iter, IITER first, IITER const &last) {
    for (; first != last; ++first)
      iter = insert(iter, *first);
  }

  void clear() { erase(begin(), end()); }

  // const value_type &v is correct here this is also used by std::list
  void push_back(const value_type &v)
    { insert(end(), v); }
  // const value_type &v is correct here this is also used by std::list
  void push_front(const value_type &v) 
    { insert(begin(), v); }

  void pop_back()
    { erase(--end()); }
  void pop_front()
    { erase(begin()); }

  size_t size() const {
    size_t retval = 0;
    
    for (const_iterator iter = begin(); iter != end(); ++iter)
      ++retval;
    return retval;
  }

/*operator std::list<VALUE>()
    { return std::list<VALUE>(begin(), end()); }
  operator std::list<VALUE>() const
    { return std::list<VALUE>(begin(), end()); }*/

  template <typename X>
  operator std::list<X>() const
    { return std::list<X>(begin(), end()); }
protected:
  // Default implementation.
  iterator implErase(iterator iter1, const iterator &iter2) {
    while (!(iter1 == iter2)) {
      iter1 = derived().implErase(iter1);
    }
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
  template<class> class I1, template<class> class I2,
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
 *  list. The elements must be comparable with @c <.
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
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
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
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
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
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
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
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
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
    const ListInterface<D1,I1,V,R1,CR1,P1,CP1> &lhs,
    const ListInterface<D2,I2,V,R2,CR2,P2,CP2> &rhs)
  { return !(lhs < rhs); }

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_LISTINTERFACE_HPP
