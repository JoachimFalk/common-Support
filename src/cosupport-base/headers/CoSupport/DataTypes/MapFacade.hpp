// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2014 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAPFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAPFACADE_HPP

#include <CoSupport/SmartPtr/intrusive_refcount_ptr.hpp>

#include "MapVirtual.hpp"
#include "Facade.hpp"

#include "Iter/Detail/BidirectionalTraversalVFImpl.hpp"

namespace CoSupport { namespace DataTypes {

template <
  class T,
  class R = typename boost::add_reference<T>::type,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  class P = typename boost::add_pointer<T>::type,
  class CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class MapFacadeInterface
: public MapVirtualInterface<T,R,CR,P,CP>,
  public SmartPtr::RefCount
{};

template <class T, class R, class CR, class P, class CP>
void intrusive_ptr_release(MapFacadeInterface<T,R,CR,P,CP> *p) {
  if (p->del_ref())
    // RefCountObject has virtual destructor
    delete p;
}

namespace Detail {

  template <
    class SET,
    class T,
    class R = typename boost::add_reference<T>::type,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
    class P = typename boost::add_pointer<T>::type,
    class CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
  >
  class MapFacadeImpl: public MapFacadeInterface<T,R,CR,P,CP> {
    typedef MapFacadeImpl<SET,T,R,CR,P,CP>  this_type;
    typedef MapFacadeInterface<T,R,CR,P,CP> base_type;
  public:
    MapFacadeImpl() {}
    MapFacadeImpl(SET map): map(map) {}
    MapFacadeImpl(const SET &map): map(map) {}

    SET map;

    typedef Iter::Detail::BidirectionalTraversalVFImpl<
      CR,
      typename SET::iterator,
      boost::intrusive_ptr<this_type> >     VIterImpl;

    typename base_type::VIter *implPBegin() const
      { return new VIterImpl(const_cast<this_type *>(this),map.begin()); }
    typename base_type::VIter *implPEnd() const
      { return new VIterImpl(const_cast<this_type *>(this),map.end()); }
    std::pair<typename base_type::VIter *, bool> implPInsert(T const &v) {
      std::pair<typename SET::iterator, bool> retval(map.insert(v));
      return std::pair<typename base_type::VIter *, bool>(
          new VIterImpl(const_cast<this_type *>(this),retval.first), retval.second);
    }
    void         implErase(typename base_type::VIter const &iter)
      { map.erase(static_cast<VIterImpl const &>(iter).iter); }
    size_t       implSize() const
      { return map.size(); }
    void         implErase(typename base_type::VIter const &iter1,
                           typename base_type::VIter const &iter2) {
      map.erase(static_cast<VIterImpl const &>(iter1).iter,
                static_cast<VIterImpl const &>(iter2).iter);
    }
    typename base_type::VIter *implPLowerBound(T const &k) const
      { return new VIterImpl(const_cast<this_type *>(this),map.lower_bound(k)); }
    typename base_type::VIter *implPUpperBound(T const &k) const
      { return new VIterImpl(const_cast<this_type *>(this),map.upper_bound(k)); }
    typename base_type::VIter *implPFind(T const &k) const
      { return new VIterImpl(const_cast<this_type *>(this),map.find(k)); }
  };

} // namespace Detail

/// This class is a facade for a std::map look alike containing values of type T.
/// \example test_map.cpp
template <
  typename T,
  typename R  = typename boost::add_reference<T>::type,
  typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  typename P  = typename boost::add_pointer<T>::type,
  typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class MapFacade
: public FacadeFoundation<
    MapFacade<T,R,CR,P,CP>,
    MapFacadeInterface<T,R,CR,P,CP> >,
  public Detail::MapVirtualUser<
    MapFacade<T,R,CR,P,CP>,T,R,CR,P,CP>
{
  typedef MapFacade<T,R,CR,P,CP>                                        this_type;
  typedef FacadeFoundation<this_type,MapFacadeInterface<T,R,CR,P,CP> >  base1_type;
  typedef Detail::MapVirtualUser<this_type,T,R,CR,P,CP>                 base2_type;

  friend class MapInterface<this_type,Detail::MapVirtualIter,T,R,CR,P,CP>;
  friend class Detail::MapVirtualUser<this_type,T,R,CR,P,CP>;
  template <class CONTAINER, bool REVERSE> friend class Detail::MapVirtualIterBaseAccessor;
  template <class CONTAINER, bool REVERSE> friend class Detail::MapVirtualIter;
private:
  /// This is required by Detail::MapVirtualUser.
  typename base1_type::ImplType *getImpl() const
    { return FacadeCoreAccess::getImpl(*this); }
protected:
  /// This is required by FacadeRef.
  explicit MapFacade(typename base1_type::_StorageType const &x)
    : base1_type(x) {}
public:
  MapFacade(typename base1_type::SmartPtr const &p)
    : base1_type(typename base1_type::_StorageType(p)) {}
  MapFacade(typename base1_type::ImplType *p)
    : base1_type(typename base1_type::_StorageType(p)) {}

  MapFacade()
    : base1_type(new Detail::MapFacadeImpl<std::map<T>,T,R,CR,P,CP>()) {}
  MapFacade(this_type const &val)
    : base1_type(new Detail::MapFacadeImpl<std::map<T>,T,R,CR,P,CP>())
    { static_cast<Detail::MapFacadeImpl<std::map<T>,T,R,CR,P,CP> *>(this->getImpl())->map.insert(val.begin(), val.end()); }
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  MapFacade(MapInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : base1_type(new Detail::MapFacadeImpl<std::map<T>,T,R,CR,P,CP>())
    { static_cast<Detail::MapFacadeImpl<std::map<T>,T,R,CR,P,CP> *>(this->getImpl())->map.insert(val.begin(), val.end()); }

  using base2_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAPFACADE_HPP
