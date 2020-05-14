// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VALUEVIRTUAL_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VALUEVIRTUAL_HPP

#include "ValueInterface.hpp"

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace CoSupport { namespace DataTypes {

/// This class represents a virtual interface for a storage which contains a value of type T.
/// \example test_value.cpp
template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class ValueVirtualInterface
: private boost::noncopyable {
  typedef ValueVirtualInterface<T,CR> this_type;
public:
  typedef T value_type;

  virtual ~ValueVirtualInterface() {}

  virtual void  implSet(const T &) = 0;
  virtual CR    implGet() const = 0;
};

namespace Detail {

  template <
    class T,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
  >
  class ValueVirtualImpl: public ValueVirtualInterface<T,CR> {
    typedef ValueVirtualImpl<T,CR>      this_type;
    typedef ValueVirtualInterface<T,CR> base_type;
  public:
    ValueVirtualImpl(T const &value = T()): value(value) {}

    T value;

    void  implSet(const T &v) { value = v; }
    CR    implGet() const { return value; }
  };

  template <
    class D,
    class T,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
  >
  class ValueVirtualUser
  : public ValueInterface<D,T,CR>
  {
    typedef ValueVirtualUser<D,T,CR>  this_type;
    typedef ValueInterface<D,T,CR>    base_type;
  public:
    using base_type::operator =;
  protected:
    typedef ValueVirtualInterface<T,CR> Impl;
 
    Impl *_impl() const
      { return static_cast<const D *>(this)->getImpl(); }

    void  implSet(const T &v) { _impl()->implSet(v); }
    CR    implGet() const { return _impl()->implGet(); }
  };

} // namespace Detail

/// This class is a wrapper to access a value virtual interface.
/// \example test_value.cpp
template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class ValueVirtual
: public Detail::ValueVirtualUser<ValueVirtual<T,CR>,T,CR> {
  typedef ValueVirtual<T,CR>                        this_type;
  typedef Detail::ValueVirtualUser<this_type,T,CR>  base_type;

  friend class ValueInterface<this_type,T,CR>;
  friend class Detail::ValueVirtualUser<this_type,T,CR>;
protected:
  boost::scoped_ptr<typename base_type::Impl> impl;

  typename base_type::Impl *getImpl() const { return impl.get(); }
public:
  template <class PImpl>
  ValueVirtual(PImpl *p, typename boost::enable_if<boost::is_base_of<typename base_type::Impl, PImpl>, int>::type dummy = 0)
    : impl(p) {}

  ValueVirtual(T const &value = T())
    : impl(new Detail::ValueVirtualImpl<T,CR>(value)) {}
  ValueVirtual(this_type const &value)
    : impl(new Detail::ValueVirtualImpl<T,CR>(value)) {}
  template <class DD, typename TT, typename CRCR>
  ValueVirtual(ValueInterface<DD,TT,CRCR> const &value)
    : impl(new Detail::ValueVirtualImpl<T,CR>(value)) {}

  using base_type::operator =;
#ifdef _MSC_VER
  // Visual Studio can't pull the following template assignment operator from base_type.
  // Hence, we replicate it here.
  template <class DD, typename TT, typename CRCR>
  this_type &operator = (const ValueInterface<DD, TT, CRCR> &val)
    { this->set(val); return *this; }
#endif //_MSC_VER

  this_type &operator =(const this_type &value)
    { this->set(value); return *this; }
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VALUEVIRTUAL_HPP
