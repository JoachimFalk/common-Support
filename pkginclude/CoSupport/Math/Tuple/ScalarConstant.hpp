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

#ifndef _INCLUDED_COSUPPORT_MATH_TUPLE_SCALARCONSTANT_HPP
#define _INCLUDED_COSUPPORT_MATH_TUPLE_SCALARCONSTANT_HPP

#include "../../DataTypes/VectorInterface.hpp"

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/add_const.hpp>

namespace CoSupport { namespace Math { namespace Tuple {

namespace Detail {

  template <typename T>
  class ScalarConstantRandomAccessTraversalIter {
    typedef ScalarConstantRandomAccessTraversalIter<T> this_type;
  public:
    T      t;
    size_t n;

    ScalarConstantRandomAccessTraversalIter(T t, size_t n): t(t), n(n) {}

    void advance(int m) { n += m; }
    int  distance_to(const this_type &rhs) const { return (int) rhs.n - n; }
    bool equal(const this_type &rhs) const { return n == rhs.n; }

    T const &deref() const { return t; }
  };

} // namespace Detail

template<class T>
class ScalarConstant: public DataTypes::VectorInterface<
    ScalarConstant<T>,
    Detail::ScalarConstantRandomAccessTraversalIter<
      typename boost::add_const<T>::type>,
    typename boost::add_const<T>::type,
    typename boost::add_reference<typename boost::add_const<T>::type>::type,
    typename boost::add_reference<typename boost::add_const<T>::type>::type,
    typename boost::add_pointer<typename boost::add_const<typename boost::remove_reference<T>::type>::type>::type,
    typename boost::add_pointer<typename boost::add_const<typename boost::remove_reference<T>::type>::type>::type
  >
{
  typedef ScalarConstant<T> this_type;

  friend class DataTypes::VectorInterface<
    ScalarConstant<T>,
    typename this_type::IterImpl,
    typename this_type::value_type,
    typename this_type::reference,
    typename this_type::const_reference,
    typename this_type::pointer,
    typename this_type::const_pointer
  >;
public:
  ScalarConstant(T t, size_t n = 1): t(t), n(n) {}
protected:
  T      t;
  size_t n;

  typename this_type::IterImpl first() const
    { return typename this_type::IterImpl(t, 0); }
  typename this_type::IterImpl last() const
    { return typename this_type::IterImpl(t, n); }

/*Detail::ScalarConstantRandomAccessTraversalIter<typename std::vector<T>::iterator> del(const Detail::ScalarConstantRandomAccessTraversalIter<typename std::vector<T>::iterator> &iter)
    { ... }
  Detail::ScalarConstantRandomAccessTraversalIter<typename std::vector<T>::iterator> add(const Detail::ScalarConstantRandomAccessTraversalIter<typename std::vector<T>::iterator> &iter, const typename this_type::value_type &value)
    { ... }
 */
};

} } } // namespace CoSupport::Math::Tuple

#endif // _INCLUDED_COSUPPORT_MATH_TUPLE_SCALARCONSTANT_HPP
