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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP

#include "ListVirtual.hpp"
#include "Facade.hpp"

#include "../SmartPtr/intrusive_refcount_ptr.hpp"

#include "Iter/Detail/BidirectionalTraversalVFImpl.hpp"

namespace CoSupport { namespace DataTypes {

template <
  class T,
  class R = typename boost::add_reference<T>::type,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  class P = typename boost::add_pointer<T>::type,
  class CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class ListFacadeInterface
: public ListVirtualInterface<T,R,CR,P,CP>,
  public SmartPtr::RefCount
{};

template <class T, class R, class CR, class P, class CP>
void intrusive_ptr_release(ListFacadeInterface<T,R,CR,P,CP> *p) {
  if (p->del_ref())
    // RefCountObject has virtual destructor
    delete p;
}

namespace Detail {

  template <
    class LIST,
    class T,
    class R = typename boost::add_reference<T>::type,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
    class P = typename boost::add_pointer<T>::type,
    class CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
  >
  class ListFacadeImpl: public ListFacadeInterface<T,R,CR,P,CP> {
    typedef ListFacadeImpl<LIST,T,R,CR,P,CP>  this_type;
    typedef ListFacadeInterface<T,R,CR,P,CP>  base_type;
  public:
    ListFacadeImpl() {}
    ListFacadeImpl(LIST list): list(list) {}
    ListFacadeImpl(const LIST &list): list(list) {}

    LIST list;

    typedef Iter::Detail::BidirectionalTraversalVFImpl<
      R,
      typename LIST::iterator,
      boost::intrusive_ptr<this_type> >     VIterImpl;

    typename base_type::VIter *implPBegin()
      { return new VIterImpl(this, list.begin()); }
    typename base_type::VIter *implPEnd()
      { return new VIterImpl(this, list.end()); }
    typename base_type::VIter *implPInsert(
        typename base_type::VIter const &iter, T const &v) {
      return new VIterImpl(this, list.insert(
        static_cast<VIterImpl const &>(iter).iter, v));
    }
    typename base_type::VIter *implPErase(
        typename base_type::VIter const &iter) {
      return new VIterImpl(this, list.erase(
        static_cast<VIterImpl const &>(iter).iter));
    }
    typename base_type::VIter *implPErase(
          typename base_type::VIter const &iter1,
          typename base_type::VIter const &iter2)
    {
      return new VIterImpl(this, list.erase(
        static_cast<VIterImpl const &>(iter1).iter,
        static_cast<VIterImpl const &>(iter2).iter));
    }
  };

} // namespace Detail

/// This class is a facade for a std::list look alike containing values of type T.
/// \example test_set.cpp
template <
  typename T,
  typename R  = typename boost::add_reference<T>::type,
  typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  typename P  = typename boost::add_pointer<T>::type,
  typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class ListFacade
: public FacadeFoundation<
    ListFacade<T,R,CR,P,CP>,
    ListFacadeInterface<T,R,CR,P,CP> >,
  public Detail::ListVirtualUser<
    ListFacade<T,R,CR,P,CP>,T,R,CR,P,CP>
{
  typedef ListFacade<T,R,CR,P,CP>                                        this_type;
  typedef FacadeFoundation<this_type,ListFacadeInterface<T,R,CR,P,CP> >  base1_type;
  typedef Detail::ListVirtualUser<this_type,T,R,CR,P,CP>                 base2_type;

  friend class ListInterface<this_type,Detail::ListVirtualIter,T,R,CR,P,CP>;
  friend class Detail::ListVirtualUser<this_type,T,R,CR,P,CP>;
  template <class CONTAINER, bool REVERSE> friend class Detail::ListVirtualIterBaseAccessor;
  template <class CONTAINER, bool REVERSE> friend class Detail::ListVirtualIter;
public:
  ListFacade()
    : base1_type(new Detail::ListFacadeImpl<std::list<T>,T,R,CR,P,CP>()) {}
  ListFacade(this_type const &val)
    : base1_type(new Detail::ListFacadeImpl<std::list<T>,T,R,CR,P,CP>()) {
    Detail::ListFacadeImpl<std::list<T>,T,R,CR,P,CP> *_impl =
      static_cast<Detail::ListFacadeImpl<std::list<T>,T,R,CR,P,CP> *>(this->getImpl());
    _impl->list.insert(_impl->list.begin(), val.begin(), val.end());
  }
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  ListFacade(ListInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : base1_type(new Detail::ListFacadeImpl<std::list<T>,T,R,CR,P,CP>()) {
    Detail::ListFacadeImpl<std::list<T>,T,R,CR,P,CP> *_impl =
      static_cast<Detail::ListFacadeImpl<std::list<T>,T,R,CR,P,CP> *>(this->getImpl());
    _impl->list.insert(_impl->list.begin(), val.begin(), val.end());
  }

  ListFacade(typename base1_type::SmartPtr const &p)
    : base1_type(p) {}

  using base2_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
