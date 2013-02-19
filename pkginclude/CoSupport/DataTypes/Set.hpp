// vim: set sw=2 sts=2 ts=8 et syn=cpp:
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_SET_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_SET_HPP

#include "SetInterface.hpp"

#include <set>

namespace CoSupport { namespace DataTypes {

/// This class implements the interface for a std::set containing values of type T.
template <
  typename T,
  template<class DD, template <class, bool> class II, class TT, class RR, class CRCR, class PP, class CPCP> class BASE,
  typename R,
  typename CR,
  typename P,
  typename CP
>
class Set;

namespace Detail {

  template <class CONTAINER, bool REVERSE>
  class SetIter;

  template <class CONTAINER, bool REVERSE>
  struct SetIterBaseAccessor {
    typedef typename CONTAINER::template IterBase<CONTAINER, REVERSE> type;
  };

  template <class CONTAINER>
  class SetIter<CONTAINER, false>: public SetIterBaseAccessor<CONTAINER, false>::type {
    typedef SetIter<CONTAINER, false> this_type;

    template <
      typename T,
      template<class DD, template <class, bool> class II, class TT, class RR, class CRCR, class PP, class CPCP> class BASE,
      typename R,
      typename CR,
      typename P,
      typename CP
    >
    friend class Set;
    friend class boost::iterator_core_access;
  public:
    SetIter() {}
  private:
    typename std::set<typename CONTAINER::value_type>::iterator iter;

    SetIter(typename std::set<typename CONTAINER::value_type>::iterator const &iter): iter(iter) {}

    void increment() { ++iter; }
    void decrement() { --iter; }
    bool equal(const this_type &rhs) const { return iter == rhs.iter; }

    typename this_type::reference dereference() const { return *iter; }
  };

} // namespace Detail

/// This class implements the interface for a std::set containing values of type T.
template <
  typename T,
  template<class DD, template <class, bool> class II, class TT, class RR, class CRCR, class PP, class CPCP> class BASE = SetInterface,
  typename R  = typename boost::add_reference<T>::type,
  typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  typename P  = typename boost::add_pointer<T>::type,
  typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class Set
: public BASE<Set<T,BASE,R,CR,P,CP>,Detail::SetIter,T,R,CR,P,CP>
{
  typedef Set<T,BASE,R,CR,P,CP>                       this_type;
  typedef BASE<this_type,Detail::SetIter,T,R,CR,P,CP> base_type;

  friend class SetInterface<this_type,Detail::SetIter,T,R,CR,P,CP>;
  template <class CONTAINER, bool REVERSE> friend class Detail::SetIterBaseAccessor;
protected:
  std::set<T> set;

  typename this_type::iterator implBegin() const
    { return const_cast<this_type *>(this)->set.begin(); }
  typename this_type::iterator implEnd() const
    { return const_cast<this_type *>(this)->set.end(); }

  typename this_type::iterator *implPBegin() const
    { return new typename this_type::iterator(this->implBegin()); }
  typename this_type::iterator *implPEnd() const
    { return new typename this_type::iterator(this->implEnd()); }

  using base_type::implErase;

  void
  implErase(const typename this_type::iterator &iter)
    { set.erase(iter.iter); }
  std::pair<typename this_type::iterator, bool>
  implInsert(const typename this_type::value_type &value) {
    std::pair<typename std::set<T>::iterator, bool> retval(set.insert(value));
    return std::make_pair(
      typename this_type::iterator(retval.first),
      retval.second);
  }
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_SET_HPP
