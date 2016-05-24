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
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <boost/utility/enable_if.hpp>

#include "Detail/RandomAccessTraversalIterTemplate.hpp"

#include <vector>

namespace CoSupport { namespace DataTypes {

// WARNING: If you change VectorInterface also adapt output operator in
// stl_output_for_vector.hpp

/// VectorInterface is used as a public base class for defining new
/// standard-conforming, e.g., std::vector<VALUE>, vector containers.
/// The derived class DERIVED must implement first, last, add, del,
/// and possibly delRange. These will be used by this template to
/// implement the std::vector interface.
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
protected:
  // This is not a standard container type definition => hide it!
  typedef ITER_  IterImpl;      // for usage by Detail::BidirectionalTraversalIterTemplate
private:
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
  const_iterator cbegin() const
    { return base_type::construct<const_iterator>(derived().first()); }

  iterator end()
    { return base_type::construct<iterator>(derived().last()); }
  const_iterator end() const
    { return base_type::construct<const_iterator>(derived().last()); }
  const_iterator cend() const
    { return base_type::construct<const_iterator>(derived().last()); }

  reference front()
    { return *begin(); }
  const_reference front() const
    { return *begin(); }

  reference back()
    { return *--end(); }
  const_reference back() const
    { return *--end(); }

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

  // const value_type &v is correct here this is also used by std::vector
  iterator insert(const iterator &iter, const value_type &v) {
    return base_type::construct<iterator>
      (derived().add(base_type::retrieve(iter), v));
  }
  // const value_type &v is correct here this is also used by std::vector
  void insert(iterator iter, size_type n, const value_type &v) {
    for (;n > 0; --n)
      iter = base_type::construct<iterator>
        (derived().add(base_type::retrieve(iter), v));
  }
  template <class IITER>
  typename boost::disable_if<boost::is_integral<IITER>, void>::type
  insert(iterator iter, IITER first, IITER const &last) {
    for (; first != last; ++first)
      iter = base_type::construct<iterator>
        (derived().add(base_type::retrieve(iter), *first));
  }

  void clear() { erase(begin(), end()); }

  // const value_type &v is correct here this is also used by std::vector
  void push_back(const value_type &v) { insert(end(), v); }
  void pop_back()                     { erase(--end()); }

/*operator std::vector<VALUE>()
    { return std::vector<VALUE>(begin(), end()); }
  operator std::vector<VALUE>() const
    { return std::vector<VALUE>(begin(), end()); }*/

  template <typename X>
  operator std::vector<X>() const
    { return std::vector<X>(begin(), end()); }

protected:
  // Default implementation. This should be overwritten for
  // efficiency reasons in DERIVED.
  IterImpl delRange(const IterImpl iter1, IterImpl iter2) {
    for (bool finished = iter1.equal(iter2); !finished; ) {
      iter2.advance(-1); finished = iter1.equal(iter2);
      iter2 = derived().del(iter2);
    }
    return iter2;
  }
};

/**
 *  @brief  Vector equality comparison.
 *  @param  lhs A %VectorInterface.
 *  @param  rhs A %VectorInterface of the same type as @a x.
 *  @return  True iff the size and elements of the VectorInterfaces are equal.
 *
 *  This is an equivalence relation. It is linear in the size of
 *  the vecto. Vectors are considered equivalent if their sizes are
 *  equal, and if corresponding elements compare equal.
 */
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class TR1, class TR2,
  class TCR1, class TCR2,
  class TP1, class TP2,
  class TCP1, class TCP2
>
inline
bool operator == (
    const VectorInterface<D1,I1,V,TR1,TCR1,TP1,TCP1> &lhs,
    const VectorInterface<D2,I2,V,TR2,TCR2,TP2,TCP2> &rhs)
{
  typename VectorInterface<D1,I1,V,TR1,TCR1,TP1,TCP1>::const_iterator iter = lhs.begin();
  typename VectorInterface<D1,I1,V,TR1,TCR1,TP1,TCP1>::const_iterator iend = lhs.end();
  typename VectorInterface<D2,I2,V,TR2,TCR2,TP2,TCP2>::const_iterator jter = rhs.begin();
  typename VectorInterface<D2,I2,V,TR2,TCR2,TP2,TCP2>::const_iterator jend = rhs.end();
  while (iter != iend && jter != jend && *iter == *jter) {
    ++iter; ++jter;
  }
  return iter == iend && jter == jend;
}

/**
 *  @brief  Vector ordering relation.
 *  @param  x  A %VectorInterface.
 *  @param  y  A %VectorInterface of the same type as @a x.
 *  @return  True iff @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  vector. The elements must be comparable with @c <.
 *
 *  See std::lexicographical_compare() for how the determination is made.
 */
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class TR1, class TR2,
  class TCR1, class TCR2,
  class TP1, class TP2,
  class TCP1, class TCP2
>
inline
bool operator < (
    const VectorInterface<D1,I1,V,TR1,TCR1,TP1,TCP1> &lhs,
    const VectorInterface<D2,I2,V,TR2,TCR2,TP2,TCP2> &rhs)
  { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

/// Based on operator==
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class TR1, class TR2,
  class TCR1, class TCR2,
  class TP1, class TP2,
  class TCP1, class TCP2
>
inline
bool operator != (
    const VectorInterface<D1,I1,V,TR1,TCR1,TP1,TCP1> &lhs,
    const VectorInterface<D2,I2,V,TR2,TCR2,TP2,TCP2> &rhs)
  { return !(lhs == rhs); }

/// Based on operator<
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class TR1, class TR2,
  class TCR1, class TCR2,
  class TP1, class TP2,
  class TCP1, class TCP2
>
inline
bool operator > (
    const VectorInterface<D1,I1,V,TR1,TCR1,TP1,TCP1> &lhs,
    const VectorInterface<D2,I2,V,TR2,TCR2,TP2,TCP2> &rhs)
  { return rhs < lhs; }

/// Based on operator<
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class TR1, class TR2,
  class TCR1, class TCR2,
  class TP1, class TP2,
  class TCP1, class TCP2
>
inline
bool operator <= (
    const VectorInterface<D1,I1,V,TR1,TCR1,TP1,TCP1> &lhs,
    const VectorInterface<D2,I2,V,TR2,TCR2,TP2,TCP2> &rhs)
  { return !(rhs < lhs); }

/// Based on operator<
template <
  class D1, class D2,
  class I1, class I2,
  class V,
  class TR1, class TR2,
  class TCR1, class TCR2,
  class TP1, class TP2,
  class TCP1, class TCP2
>
inline
bool operator >= (
    const VectorInterface<D1,I1,V,TR1,TCR1,TP1,TCP1> &lhs,
    const VectorInterface<D2,I2,V,TR2,TCR2,TP2,TCP2> &rhs)
  { return !(lhs < rhs); }

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VECTORINTERFACE_HPP
