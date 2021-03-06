// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_SETFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_SETFACADE_HPP

#include <CoSupport/SmartPtr/intrusive_refcount_ptr.hpp>

#include "SetVirtual.hpp"
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
class SetFacadeInterface
: public SetVirtualInterface<T,R,CR,P,CP>,
  public SmartPtr::RefCount
{};

template <class T, class R, class CR, class P, class CP>
void intrusive_ptr_release(SetFacadeInterface<T,R,CR,P,CP> *p) {
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
  class SetFacadeImpl: public SetFacadeInterface<T,R,CR,P,CP> {
    typedef SetFacadeImpl<T,R,CR,P,CP>      this_type;
    typedef SetFacadeInterface<T,R,CR,P,CP> base_type;
  public:
    SetFacadeImpl() {}
    template <class I>
    SetFacadeImpl(I const &begin, I const &end): set(begin, end) {}

    std::set<T> set;

    typedef Iter::Detail::BidirectionalTraversalVFImpl<
      CR,
      typename std::set<T>::iterator,
      ::boost::intrusive_ptr<this_type> >   VIterImpl;

    typename base_type::VIter *implPBegin() const
      { return new VIterImpl(const_cast<this_type *>(this),set.begin()); }
    typename base_type::VIter *implPEnd() const
      { return new VIterImpl(const_cast<this_type *>(this),set.end()); }
    std::pair<typename base_type::VIter *, bool> implPInsert(T const &v) {
      std::pair<typename std::set<T>::iterator, bool> retval(set.insert(v));
      return std::pair<typename base_type::VIter *, bool>(
          new VIterImpl(const_cast<this_type *>(this),retval.first), retval.second);
    }
    void         implErase(typename base_type::VIter const &iter)
      { set.erase(static_cast<VIterImpl const &>(iter).iter); }
    size_t       implSize() const
      { return set.size(); }
    void         implErase(typename base_type::VIter const &iter1,
                           typename base_type::VIter const &iter2) {
      set.erase(static_cast<VIterImpl const &>(iter1).iter,
                static_cast<VIterImpl const &>(iter2).iter);
    }
    typename base_type::VIter *implPLowerBound(T const &k) const
      { return new VIterImpl(const_cast<this_type *>(this),set.lower_bound(k)); }
    typename base_type::VIter *implPUpperBound(T const &k) const
      { return new VIterImpl(const_cast<this_type *>(this),set.upper_bound(k)); }
    typename base_type::VIter *implPFind(T const &k) const
      { return new VIterImpl(const_cast<this_type *>(this),set.find(k)); }
  };

} // namespace Detail

/// This class is a facade for a std::set look alike containing values of type T.
/// \example test_set.cpp
template <
  typename T,
  typename R  = typename ::boost::add_reference<T>::type,
  typename CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type,
  typename P  = typename ::boost::add_pointer<T>::type,
  typename CP = typename ::boost::add_pointer<typename ::boost::add_const<T>::type>::type
>
class SetFacade
: public FacadeFoundation<
    SetFacade<T,R,CR,P,CP>,
    SetFacadeInterface<T,R,CR,P,CP> >,
  public Detail::SetVirtualUser<
    SetFacade<T,R,CR,P,CP>,T,R,CR,P,CP>
{
  typedef SetFacade<T,R,CR,P,CP>                                        this_type;
  typedef FacadeFoundation<this_type,SetFacadeInterface<T,R,CR,P,CP> >  base1_type;
  typedef Detail::SetVirtualUser<this_type,T,R,CR,P,CP>                 base2_type;

  friend class SetInterface<this_type,Detail::SetVirtualIter,T,R,CR,P,CP>;
  friend class Detail::SetVirtualUser<this_type,T,R,CR,P,CP>;
  template <class CONTAINER> friend class Detail::SetVirtualIter;
private:
  /// This is required by Detail::SetVirtualUser.
  typename base1_type::ImplType *getImpl() const
    { return FacadeCoreAccess::getImpl(*this); }
protected:
  /// This is required by FacadeRef.
  explicit SetFacade(typename base1_type::_StorageType const &x)
    : base1_type(x) {}
public:
  SetFacade(typename base1_type::SmartPtr const &p)
    : base1_type(typename base1_type::_StorageType(p)) {}
  SetFacade(typename base1_type::ImplType *p)
    : base1_type(typename base1_type::_StorageType(p)) {}

  SetFacade()
    : base1_type(new Detail::SetFacadeImpl<T,R,CR,P,CP>()) {}
  template <class DD>
  SetFacade(std::set<DD> const &val)
    : base1_type(new Detail::SetFacadeImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}
  SetFacade(this_type const &val)
    : base1_type(new Detail::SetFacadeImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  SetFacade(SetInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : base1_type(new Detail::SetFacadeImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}

  using base2_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_SETFACADE_HPP
