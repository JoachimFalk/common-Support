// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAPVIRTUAL_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAPVIRTUAL_HPP

#include "../compatibility-glue/nullptr.h"
#include "MapInterface.hpp"

#include <map>

#include <boost/scoped_ptr.hpp>
#include <memory> // for std::unique_ptr

#include "Iter/BidirectionalTraversalVIf.hpp"
#include "Iter/Detail/BidirectionalTraversalVImpl.hpp"

namespace CoSupport { namespace DataTypes {

template <
  class T,
  class R = typename boost::add_reference<T>::type,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  class P = typename boost::add_pointer<T>::type,
  class CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class MapVirtualInterface {
  typedef MapVirtualInterface<T,R,CR,P,CP> this_type;
public:
  virtual ~MapVirtualInterface() {}

  typedef Iter::BidirectionalTraversalVIf<CR> VIter;

  virtual VIter *implPBegin() const = 0;
  virtual VIter *implPEnd() const = 0;
  virtual std::pair<VIter *, bool>  implPInsert(T const &v) = 0;
  virtual void         implErase(VIter const &iter) = 0;

  // Default implementation.
  virtual size_t implSize() const {
    size_t                          retval = 0;
    boost::scoped_ptr<VIter > iter(implPBegin());
    boost::scoped_ptr<VIter > end(implPEnd());
    for (; !iter->equal(*end); iter->increment())
      ++retval;
    return retval;
  }
  // Default implementation.
  virtual void implErase(VIter const &iter1, const VIter &iter2) {
    boost::scoped_ptr<VIter > iter(iter1.duplicate());
    while (!iter->equal(iter2)) {
      boost::scoped_ptr<VIter > iterl(iter->duplicate()); iter->increment();
      implErase(*iterl);
    }
  }
  // Default implementation.
  virtual VIter *implPLowerBound(T const &k) const {
    std::unique_ptr<VIter >   iter(implPBegin());
    boost::scoped_ptr<VIter > end(implPEnd());
    for (; !iter->equal(*end); iter->increment())
      if (!(iter->dereference() < k)) //FIXME: comparision semantics may differ from interally implemented one
        break;
    return iter.release();
  }
  // Default implementation.
  virtual VIter *implPUpperBound(T const &k) const {
    std::unique_ptr<VIter > retval(implPLowerBound(k));
    if (retval->dereference() == k)
      retval->increment();
    return retval.release();
  }
  // Default implementation.
  virtual VIter *implPFind(T const &k) const {
    std::unique_ptr<VIter > retval(implPLowerBound(k));
    std::unique_ptr<VIter > end(implPEnd());
    if (!retval->equal(*end) && !(retval->dereference() == k))
      retval = end;
    return retval.release();
  }
};

namespace Detail {

  template <
    class T,
    class R = typename boost::add_reference<T>::type,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
    class P = typename boost::add_pointer<T>::type,
    class CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
  >
  class MapVirtualImpl: public MapVirtualInterface<T,R,CR,P,CP> {
    typedef MapVirtualImpl<T,R,CR,P,CP>       this_type;
    typedef MapVirtualInterface<T,R,CR,P,CP>  base_type;
  public:
    MapVirtualImpl() {}

    std::map<T> map;

    typedef Iter::Detail::BidirectionalTraversalVImpl<CR, typename std::map<T>::iterator> VIterImpl;

    typename base_type::VIter *implPBegin() const
      { return new VIterImpl(map.begin()); }
    typename base_type::VIter *implPEnd() const
      { return new VIterImpl(map.end()); }
    std::pair<typename base_type::VIter *, bool> implPInsert(T const &v) {
      std::pair<typename std::map<T>::iterator, bool> retval(map.insert(v));
      return std::pair<typename base_type::VIter *, bool>(
          new VIterImpl(retval.first), retval.second);
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
      { return new VIterImpl(map.lower_bound(k)); }
    typename base_type::VIter *implPUpperBound(T const &k) const
      { return new VIterImpl(map.upper_bound(k)); }
    typename base_type::VIter *implPFind(T const &k) const
      { return new VIterImpl(map.find(k)); }
  };

  template <class D, typename T, typename R, typename CR, typename P, typename CP>
  class MapVirtualUser;

  template <class CONTAINER>
  struct MapVirtualIterBaseAccessor {
    typedef typename CONTAINER::template IterBase<CONTAINER>::type type;
  };

  template <class CONTAINER>
  class MapVirtualIter: public MapVirtualIterBaseAccessor<CONTAINER>::type {
    typedef MapVirtualIter<CONTAINER> this_type;

    template <
      class    D,
      typename T,
      typename R,
      typename CR,
      typename P,
      typename CP
    >
    friend class MapVirtualUser;
    friend class boost::iterator_core_access;
    friend class MapVirtualIterBaseAccessor<CONTAINER>::type;
  public:
    MapVirtualIter(): impl(nullptr) {}
    MapVirtualIter(this_type const &rhs): impl(rhs.impl->duplicate()) {}

    this_type &operator =(this_type const &rhs) { impl.reset(rhs.impl->duplicate()); return *this; }
  private:
    typedef typename CONTAINER::Impl::VIter Impl;
    boost::scoped_ptr<Impl> impl;

    MapVirtualIter(Impl *impl): impl(impl) {}

    void increment() { impl->increment(); }
    void decrement() { impl->decrement(); }
    bool equal(const this_type &rhs) const { return impl->equal(*rhs.impl); }

    typename this_type::reference dereference() const { return impl->dereference(); }
  };

  template <
    class    D,
    typename T,
    typename R  = typename boost::add_reference<T>::type,
    typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
    typename P  = typename boost::add_pointer<T>::type,
    typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
  >
  class MapVirtualUser
  : public MapInterface<D,MapVirtualIter,T,R,CR,P,CP>
  {
    typedef MapVirtualUser<D,T,R,CR,P,CP>               this_type;
    typedef MapInterface<D,MapVirtualIter,T,R,CR,P,CP>  base_type;
  public:
    using base_type::operator =;
  protected:
    typedef MapVirtualInterface<T,R,CR,P,CP> Impl;
 
    Impl *_impl() const
      { return static_cast<const D *>(this)->getImpl(); }

    typename this_type::iterator  implBegin() const
      { return _impl()->implPBegin(); }
    typename this_type::iterator  implEnd() const
      { return _impl()->implPEnd(); }
    std::pair<typename this_type::iterator, bool>  implInsert(const typename this_type::value_type &v) {
      std::pair<typename Impl::VIter *, bool> retval(_impl()->implPInsert(v));
      return std::pair<typename this_type::iterator, bool>(
          typename this_type::iterator(retval.first),
          retval.second);
    }
    void                          implErase(const typename this_type::iterator &iter)
      { return _impl()->implErase(*iter.impl); }
    size_t                        implSize() const
      { return _impl()->implSize(); }
    void                          implErase(typename this_type::iterator iter1, const typename this_type::iterator &iter2)
      { return _impl()->implErase(*iter1.impl, *iter2.impl); }
    typename this_type::iterator  implLowerBound(const typename this_type::key_type &k) const
      { return _impl()->implPLowerBound(k); }
    typename this_type::iterator  implUpperBound(const typename this_type::key_type &k) const
      { return _impl()->implPUpperBound(k); }
    typename this_type::iterator  implFind(const typename this_type::key_type &k) const
      { return _impl()->implPFind(k); }
  };

} // namespace Detail

/// This class implements the interface for a std::map containing values of type T.
template <
  typename T,
  typename R  = typename boost::add_reference<T>::type,
  typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  typename P  = typename boost::add_pointer<T>::type,
  typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class MapVirtual
: public Detail::MapVirtualUser<MapVirtual<T,R,CR,P,CP>,T,R,CR,P,CP>
{
  typedef MapVirtual<T,R,CR,P,CP>                                     this_type;
  typedef Detail::MapVirtualUser<MapVirtual<T,R,CR,P,CP>,T,R,CR,P,CP> base_type;

  friend class MapInterface<this_type,Detail::MapVirtualIter,T,R,CR,P,CP>;
  friend class Detail::MapVirtualUser<this_type,T,R,CR,P,CP>;
  template <class CONTAINER> friend class Detail::MapVirtualIterBaseAccessor;
  template <class CONTAINER> friend class Detail::MapVirtualIter;
protected:
  boost::scoped_ptr<typename base_type::Impl> impl;

  typename base_type::Impl *getImpl() const { return impl.get(); }
public:
  MapVirtual()
    : impl(new Detail::MapVirtualImpl<T,R,CR,P,CP>()) {}
  MapVirtual(this_type const &val)
    : impl(new Detail::MapVirtualImpl<T,R,CR,P,CP>())
    { static_cast<Detail::MapVirtualImpl<T,R,CR,P,CP> *>(impl.get())->map.insert(val.begin(), val.end()); }
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  MapVirtual(MapInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : impl(new Detail::MapVirtualImpl<T,R,CR,P,CP>())
    { static_cast<Detail::MapVirtualImpl<T,R,CR,P,CP> *>(impl.get())->map.insert(val.begin(), val.end()); }
  MapVirtual(typename base_type::Impl *impl)
    : impl(impl) {}

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAPVIRTUAL_HPP
