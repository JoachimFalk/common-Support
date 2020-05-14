// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Tobias Schwarzer <tobias.schwarzer@fau.de>
 *   2014 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2019 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#ifndef _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP

#include <CoSupport/SmartPtr/intrusive_refcount_ptr.hpp>

#include "ListVirtual.hpp"
#include "Facade.hpp"

#include "Iter/Detail/BidirectionalTraversalVFImpl.hpp"

namespace CoSupport { namespace DataTypes {

template <
  class T,
  class R = typename ::boost::add_reference<T>::type,
  class CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type,
  class P = typename ::boost::add_pointer<T>::type,
  class CP = typename ::boost::add_pointer<typename ::boost::add_const<T>::type>::type
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
    class T,
    class R = typename ::boost::add_reference<T>::type,
    class CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type,
    class P = typename ::boost::add_pointer<T>::type,
    class CP = typename ::boost::add_pointer<typename ::boost::add_const<T>::type>::type
  >
  class ListFacadeImpl: public ListFacadeInterface<T,R,CR,P,CP> {
    typedef ListFacadeImpl<T,R,CR,P,CP>       this_type;
    typedef ListFacadeInterface<T,R,CR,P,CP>  base_type;
  public:
    ListFacadeImpl() {}
    template <class I>
    ListFacadeImpl(I const &begin, I const &end): list(begin, end) {}

    std::list<T> list;

    typedef Iter::Detail::BidirectionalTraversalVFImpl<
      R,
      typename std::list<T>::iterator,
      ::boost::intrusive_ptr<this_type> >   VIterImpl;

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
  typename R  = typename ::boost::add_reference<T>::type,
  typename CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type,
  typename P  = typename ::boost::add_pointer<T>::type,
  typename CP = typename ::boost::add_pointer<typename ::boost::add_const<T>::type>::type
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
  template <class CONTAINER> friend class Detail::ListVirtualIter;
private:
  /// This is required by Detail::ListVirtualUser.
  typename base1_type::ImplType *getImpl() const
    { return FacadeCoreAccess::getImpl(*this); }
protected:
  /// This is required by FacadeRef.
  explicit ListFacade(typename base1_type::_StorageType const &x)
    : base1_type(x) {}
public:
  ListFacade(typename base1_type::SmartPtr const &p)
    : base1_type(typename base1_type::_StorageType(p)) {}
  ListFacade(typename base1_type::ImplType *p)
    : base1_type(typename base1_type::_StorageType(p)) {}

  ListFacade()
    : base1_type(new Detail::ListFacadeImpl<T,R,CR,P,CP>()) {}
  template <class DD>
  ListFacade(std::list<DD> const &val)
    : base1_type(new Detail::ListFacadeImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}
  ListFacade(this_type const &val)
    : base1_type(new Detail::ListFacadeImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  ListFacade(ListInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : base1_type(new Detail::ListFacadeImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}

  using base2_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
