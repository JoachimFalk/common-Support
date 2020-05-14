// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Tobias Schwarzer <tobias.schwarzer@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEVIRTUAL_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEVIRTUAL_HPP

#include "../compatibility-glue/nullptr.h"
#include "MaybeValueInterface.hpp"
#include "ValueVirtual.hpp"

#include <boost/scoped_ptr.hpp>
#include <boost/blank.hpp>
#include <boost/variant.hpp>

namespace CoSupport { namespace DataTypes {

/// This class represents a virtual interface for a storage which contains a value of type T.
/// \example test_value.cpp
template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class MaybeValueVirtualInterface
: public ValueVirtualInterface<T,CR> {
  typedef MaybeValueVirtualInterface<T,CR> this_type;
public:
  virtual void  implUndef() = 0;
  virtual bool  implIsDefined() const = 0;
};

namespace Detail {

  template <
    class T,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
  >
  class MaybeValueVirtualImpl: public MaybeValueVirtualInterface<T,CR> {
    typedef MaybeValueVirtualImpl<T,CR>      this_type;
    typedef MaybeValueVirtualInterface<T,CR> base_type;
  public:
    MaybeValueVirtualImpl(boost::blank value = boost::blank()): value(value) {}
    MaybeValueVirtualImpl(T const &value): value(value) {}
    template <class DD, class TT, class CRCR>
    MaybeValueVirtualImpl(const MaybeValueInterface<DD,TT,CRCR> &v)
      : value(v.isDefined()
          ? boost::variant<boost::blank, T>(v.get())
          : boost::variant<boost::blank, T>(boost::blank())) {}

    boost::variant<boost::blank, T> value;

    void  implSet(const T &v) { value = v; }
    CR    implGet() const { return boost::get<T>(value); }
    void  implUndef() { value = boost::blank(); }
    bool  implIsDefined() const { return boost::get<T>(&value) != nullptr; }
  };

  template <
    class D,
    class T,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
  >
  class MaybeValueVirtualUser
  : public MaybeValueInterface<D,T,CR>
  {
    typedef MaybeValueVirtualUser<D,T,CR>  this_type;
    typedef MaybeValueInterface<D,T,CR>    base_type;
  public:
    using base_type::operator =;
  protected:
    typedef MaybeValueVirtualInterface<T,CR> Impl;
 
    Impl *_impl() const
      { return static_cast<const D *>(this)->getImpl(); }

    void  implSet(const T &v) { _impl()->implSet(v); }
    CR    implGet() const { return _impl()->implGet(); }
    void  implUndef() { _impl()->implUndef(); }
    bool  implIsDefined() const { return _impl()->implIsDefined(); }
  };

} // namespace Detail

/// This class is a wrapper to access a value virtual interface.
/// \example test_value.cpp
template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class MaybeValueVirtual
: public Detail::MaybeValueVirtualUser<MaybeValueVirtual<T,CR>,T,CR> {
  typedef MaybeValueVirtual<T,CR>                        this_type;
  typedef Detail::MaybeValueVirtualUser<this_type,T,CR>  base_type;

  friend class MaybeValueInterface<this_type,T,CR>;
  friend class Detail::MaybeValueVirtualUser<this_type,T,CR>;
protected:
  boost::scoped_ptr<typename base_type::Impl> impl;

  typename base_type::Impl *getImpl() const { return impl.get(); }
public:
  MaybeValueVirtual(boost::blank value = boost::blank())
    : impl(new Detail::MaybeValueVirtualImpl<T,CR>(value)) {}
  MaybeValueVirtual(T const &value)
    : impl(new Detail::MaybeValueVirtualImpl<T,CR>(value)) {}
  MaybeValueVirtual(this_type const &value)
    : impl(new Detail::MaybeValueVirtualImpl<T,CR>(value)) {}
  template <class DD, typename TT, typename CRCR>
  MaybeValueVirtual(MaybeValueInterface<DD,TT,CRCR> const &value)
    : impl(new Detail::MaybeValueVirtualImpl<T,CR>(value)) {}

  MaybeValueVirtual(typename base_type::Impl *impl)
    : impl(impl) {}

  using base_type::operator =;
#ifdef _MSC_VER
  // Visual Studio can't pull the following template assignment operator from base_type.
  // Hence, we replicate it here.
  template <class DD, typename TT, typename CRCR>
  this_type &operator = (const MaybeValueInterface<DD, TT, CRCR> &val)
    { this->set(val); return *this; }
#endif //_MSC_VER

  this_type &operator =(const this_type &value)
    { this->set(value); return *this; }
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEVIRTUAL_HPP
