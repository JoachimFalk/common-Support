// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Tobias Schwarzer <tobias.schwarzer@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_LISTVIRTUAL_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_LISTVIRTUAL_HPP

#include "../compatibility-glue/nullptr.h"
#include "ListInterface.hpp"

#include <list>

#include <boost/scoped_ptr.hpp>
#include <memory> // for std::unique_ptr

#include "Iter/BidirectionalTraversalVIf.hpp"
#include "Iter/Detail/BidirectionalTraversalVImpl.hpp"

namespace CoSupport { namespace DataTypes {

template <
  class T,
  class R = typename ::boost::add_reference<T>::type,
  class CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type,
  class P = typename ::boost::add_pointer<T>::type,
  class CP = typename ::boost::add_pointer<typename ::boost::add_const<T>::type>::type
>
class ListVirtualInterface {
  typedef ListVirtualInterface<T,R,CR,P,CP> this_type;
public:
  virtual ~ListVirtualInterface() {}

  typedef Iter::BidirectionalTraversalVIf<R> VIter;

  virtual VIter *implPBegin() = 0;
  virtual VIter *implPEnd() = 0;
  virtual VIter *implPInsert(VIter const &iter, T const &v) = 0;
  virtual VIter *implPErase(VIter const &iter) = 0;

  // Default implementation.
  virtual VIter *implPErase(VIter const &iter1, const VIter &iter2) {
    std::unique_ptr<VIter > iter(iter1.duplicate());
    while (!iter->equal(iter2)) {
      iter.reset(implPErase(*iter));
    }
    return iter.release();
  }
};

namespace Detail {

  template <
    class T,
    class R = typename ::boost::add_reference<T>::type,
    class CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type,
    class P = typename ::boost::add_pointer<T>::type,
    class CP = typename ::boost::add_pointer<typename ::boost::add_const<T>::type>::type
  >
  class ListVirtualImpl: public ListVirtualInterface<T,R,CR,P,CP> {
    typedef ListVirtualImpl<T,R,CR,P,CP>      this_type;
    typedef ListVirtualInterface<T,R,CR,P,CP> base_type;
  public:
    ListVirtualImpl() {}
    template <class I>
    ListVirtualImpl(I const &begin, I const &end): list(begin, end) {}

    std::list<T> list;

    typedef Iter::Detail::BidirectionalTraversalVImpl<
      R,
      typename std::list<T>::iterator>        VIterImpl;

    typename base_type::VIter *implPBegin()
      { return new VIterImpl(list.begin()); }
    typename base_type::VIter *implPEnd()
      { return new VIterImpl(list.end()); }
    typename base_type::VIter *implPInsert(
        typename base_type::VIter const &iter, T const &v) {
      return new VIterImpl(list.insert(
        static_cast<VIterImpl const &>(iter).iter, v));
    }
    typename base_type::VIter *implPErase(
        typename base_type::VIter const &iter) {
      return new VIterImpl(list.erase(
        static_cast<VIterImpl const &>(iter).iter));
    }
    typename base_type::VIter *implPErase(
          typename base_type::VIter const &iter1,
          typename base_type::VIter const &iter2)
    {
      return new VIterImpl(list.erase(
        static_cast<VIterImpl const &>(iter1).iter,
        static_cast<VIterImpl const &>(iter2).iter));
    }
  };

  template <class D, typename T, typename R, typename CR, typename P, typename CP>
  class ListVirtualUser;

  template <class CONTAINER>
  class ListVirtualIter: public Iter::Detail::BidirectionalTraversalBase<CONTAINER> {
    typedef ListVirtualIter<CONTAINER> this_type;

    template <
      class    D,
      typename T,
      typename R,
      typename CR,
      typename P,
      typename CP
    >
    friend class ListVirtualUser;
    friend class boost::iterator_core_access;
    friend class Iter::Detail::BidirectionalTraversalBase<CONTAINER>;
    friend class ListVirtualIter<typename ::boost::add_const<CONTAINER>::type>;
  public:
    ListVirtualIter(): impl(nullptr) {}
    ListVirtualIter(this_type const &rhs)
      : impl(rhs.impl->duplicate()) {}
    template <class CC>
    ListVirtualIter(ListVirtualIter<CC> const &rhs,typename ::boost::enable_if<
      ::boost::is_same<typename ::boost::remove_const<CONTAINER>::type, CC>, void *>::type _dummy = nullptr)
      : impl(rhs.impl->duplicate()) {}

    this_type &operator =(this_type const &rhs) { impl.reset(rhs.impl->duplicate()); return *this; }
  private:
    typedef typename CONTAINER::Impl::VIter Impl;
    ::boost::scoped_ptr<Impl> impl;

    ListVirtualIter(Impl *impl): impl(impl) {}

    void increment() { impl->increment(); }
    void decrement() { impl->decrement(); }
    bool equal(const this_type &rhs) const { return impl->equal(*rhs.impl); }

    typename this_type::reference dereference() const { return impl->dereference(); }
  };

  template <
    class    D,
    typename T,
    typename R  = typename ::boost::add_reference<T>::type,
    typename CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type,
    typename P  = typename ::boost::add_pointer<T>::type,
    typename CP = typename ::boost::add_pointer<typename ::boost::add_const<T>::type>::type
  >
  class ListVirtualUser
  : public ListInterface<D,ListVirtualIter,T,R,CR,P,CP>
  {
    typedef ListVirtualUser<D,T,R,CR,P,CP>               this_type;
    typedef ListInterface<D,ListVirtualIter,T,R,CR,P,CP>  base_type;
  public:
    using base_type::operator =;
  protected:
    typedef ListVirtualInterface<T,R,CR,P,CP> Impl;
 
    Impl *_impl() const
      { return static_cast<const D *>(this)->getImpl(); }

    typename this_type::iterator  implBegin()
      { return _impl()->implPBegin(); }
    typename this_type::iterator  implEnd()
      { return _impl()->implPEnd(); }
    typename this_type::iterator  implInsert(const typename this_type::iterator &iter, const typename this_type::value_type &v)
      { return _impl()->implPInsert(*iter.impl, v); }
    typename this_type::iterator  implErase(const typename this_type::iterator &iter)
      { return _impl()->implPErase(*iter.impl); }
    typename this_type::iterator  implErase(const typename this_type::iterator &iter1, const typename this_type::iterator &iter2)
      { return _impl()->implPErase(*iter1.impl, *iter2.impl); }
  };

} // namespace Detail

/// This class implements the interface for a std::list containing values of type T.
template <
  typename T,
  typename R  = typename ::boost::add_reference<T>::type,
  typename CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type,
  typename P  = typename ::boost::add_pointer<T>::type,
  typename CP = typename ::boost::add_pointer<typename ::boost::add_const<T>::type>::type
>
class ListVirtual
: public Detail::ListVirtualUser<ListVirtual<T,R,CR,P,CP>,T,R,CR,P,CP>
{
  typedef ListVirtual<T,R,CR,P,CP>                                     this_type;
  typedef Detail::ListVirtualUser<ListVirtual<T,R,CR,P,CP>,T,R,CR,P,CP> base_type;

  friend class ListInterface<this_type,Detail::ListVirtualIter,T,R,CR,P,CP>;
  friend class Detail::ListVirtualUser<this_type,T,R,CR,P,CP>;
  template <class CONTAINER> friend class Detail::ListVirtualIter;
protected:
  ::boost::scoped_ptr<typename base_type::Impl> impl;

  typename base_type::Impl *getImpl() const { return impl.get(); }
public:
  template <class DD>
  ListVirtual(std::list<DD> const &val)
    : impl(new Detail::ListVirtualImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}

  ListVirtual()
    : impl(new Detail::ListVirtualImpl<T,R,CR,P,CP>()) {}
  ListVirtual(this_type const &val)
    : impl(new Detail::ListVirtualImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}
//template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
//ListVirtual(ListInterface<DD,II,T,RR,CRCR,PP,CPCP> &val)
//  : impl(new Detail::ListVirtualImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  ListVirtual(ListInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : impl(new Detail::ListVirtualImpl<T,R,CR,P,CP>(val.begin(), val.end())) {}

  ListVirtual(typename base_type::Impl *impl)
    : impl(impl) {}

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_LISTVIRTUAL_HPP
