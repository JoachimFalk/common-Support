//  -*- tab-width:8; intent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 expandtab:
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_SETVIRTUAL_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_SETVIRTUAL_HPP

#include "SetInterface.hpp"

#include <set>

#include <boost/scoped_ptr.hpp>
#include <memory> // for std::auto_ptr

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
class SetVirtualInterface {
  typedef SetVirtualInterface<T,R,CR,P,CP> this_type;
public:
  virtual ~SetVirtualInterface() {}

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
    std::auto_ptr<VIter >     iter(implPBegin());
    boost::scoped_ptr<VIter > end(implPEnd());
    for (; !iter->equal(*end); iter->increment())
      if (!(iter->dereference() < k)) //FIXME: comparision semantics may differ from interally implemented one
        break;
    return iter.release();
  }
  // Default implementation.
  virtual VIter *implPUpperBound(T const &k) const {
    std::auto_ptr<VIter > retval(implPLowerBound(k));
    if (retval->dereference() == k)
      retval->increment();
    return retval.release();
  }
  // Default implementation.
  virtual VIter *implPFind(T const &k) const {
    std::auto_ptr<VIter > retval(implPLowerBound(k));
    std::auto_ptr<VIter > end(implPEnd());
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
  class SetVirtualImpl: public SetVirtualInterface<T,R,CR,P,CP> {
    typedef SetVirtualImpl<T,R,CR,P,CP>       this_type;
    typedef SetVirtualInterface<T,R,CR,P,CP>  base_type;
  public:
    SetVirtualImpl() {}

    std::set<T> set;

    typedef Iter::Detail::BidirectionalTraversalVImpl<CR, typename std::set<T>::iterator> VIterImpl;

    typename base_type::VIter *implPBegin() const
      { return new VIterImpl(set.begin()); }
    typename base_type::VIter *implPEnd() const
      { return new VIterImpl(set.end()); }
    std::pair<typename base_type::VIter *, bool> implPInsert(T const &v) {
      std::pair<typename std::set<T>::iterator, bool> retval(set.insert(v));
      return std::pair<typename base_type::VIter *, bool>(
          new VIterImpl(retval.first), retval.second);
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
      { return new VIterImpl(set.lower_bound(k)); }
    typename base_type::VIter *implPUpperBound(T const &k) const
      { return new VIterImpl(set.upper_bound(k)); }
    typename base_type::VIter *implPFind(T const &k) const
      { return new VIterImpl(set.find(k)); }
  };

  template <class D, typename T, typename R, typename CR, typename P, typename CP>
  class SetVirtualUser;

  template <class CONTAINER>
  struct SetVirtualIterBaseAccessor {
    typedef typename CONTAINER::template IterBase<CONTAINER>::type type;
  };

  template <class CONTAINER>
  class SetVirtualIter: public SetVirtualIterBaseAccessor<CONTAINER>::type {
    typedef SetVirtualIter<CONTAINER> this_type;

    template <
      class    D,
      typename T,
      typename R,
      typename CR,
      typename P,
      typename CP
    >
    friend class SetVirtualUser;
    friend class boost::iterator_core_access;
    friend class SetVirtualIterBaseAccessor<CONTAINER>::type;
  public:
    SetVirtualIter(): impl(NULL) {}
    SetVirtualIter(this_type const &rhs): impl(rhs.impl->duplicate()) {}

    this_type &operator =(this_type const &rhs) { impl.reset(rhs.impl->duplicate()); return *this; }
  private:
    typedef typename CONTAINER::Impl::VIter Impl;
    boost::scoped_ptr<Impl> impl;

    SetVirtualIter(Impl *impl): impl(impl) {}

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
  class SetVirtualUser
  : public SetInterface<D,SetVirtualIter,T,R,CR,P,CP>
  {
    typedef SetVirtualUser<D,T,R,CR,P,CP>               this_type;
    typedef SetInterface<D,SetVirtualIter,T,R,CR,P,CP>  base_type;
  public:
    using base_type::operator =;
  protected:
    typedef SetVirtualInterface<T,R,CR,P,CP> Impl;
 
    Impl *_impl() const
      { return static_cast<const D *>(this)->getImpl(); }

    typename this_type::iterator  implBegin() const
      { return _impl()->implPBegin(); }
    typename this_type::iterator  implEnd() const
      { return _impl()->implPEnd(); }
    std::pair<typename this_type::iterator, bool>  implInsert(const typename this_type::value_type &v) {
      std::pair<typename Impl::VIter *, bool> retval(_impl()->implPInsert(v));
      return std::pair<typename this_type::iterator, bool>(retval.first, retval.second);
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

/// This class implements the interface for a std::set containing values of type T.
template <
  typename T,
  typename R  = typename boost::add_reference<T>::type,
  typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  typename P  = typename boost::add_pointer<T>::type,
  typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class SetVirtual
: public Detail::SetVirtualUser<SetVirtual<T,R,CR,P,CP>,T,R,CR,P,CP>
{
  typedef SetVirtual<T,R,CR,P,CP>                                     this_type;
  typedef Detail::SetVirtualUser<SetVirtual<T,R,CR,P,CP>,T,R,CR,P,CP> base_type;

  friend class SetInterface<this_type,Detail::SetVirtualIter,T,R,CR,P,CP>;
  friend class Detail::SetVirtualUser<this_type,T,R,CR,P,CP>;
  template <class CONTAINER> friend class Detail::SetVirtualIterBaseAccessor;
  template <class CONTAINER> friend class Detail::SetVirtualIter;
protected:
  boost::scoped_ptr<typename base_type::Impl> impl;

  typename base_type::Impl *getImpl() const { return impl.get(); }
public:
  SetVirtual()
    : impl(new Detail::SetVirtualImpl<T,R,CR,P,CP>()) {}
  SetVirtual(this_type const &val)
    : impl(new Detail::SetVirtualImpl<T,R,CR,P,CP>())
    { static_cast<Detail::SetVirtualImpl<T,R,CR,P,CP> *>(impl.get())->set.insert(val.begin(), val.end()); }
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  SetVirtual(SetInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : impl(new Detail::SetVirtualImpl<T,R,CR,P,CP>())
    { static_cast<Detail::SetVirtualImpl<T,R,CR,P,CP> *>(impl.get())->set.insert(val.begin(), val.end()); }
  SetVirtual(typename base_type::Impl *impl)
    : impl(impl) {}

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_SETVIRTUAL_HPP
