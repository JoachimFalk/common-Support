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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VALUEFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VALUEFACADE_HPP

#include <CoSupport/SmartPtr/intrusive_refcount_ptr.hpp>

#include "ValueVirtual.hpp"
#include "Facade.hpp"

namespace CoSupport { namespace DataTypes {

template <
  class T,
  class CR
>
class MaybeValueFacadeInterface;

template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class ValueFacadeInterface
: public SmartPtr::RefCount,//< this must be first otherwise the reinterpret cast from MaybeValueFacadeInterface will not work!
  public ValueVirtualInterface<T,CR>
{};

template <class T, class CR>
void intrusive_ptr_release(ValueFacadeInterface<T,CR> *p) {
  if (p->del_ref())
    // RefCountObject has virtual destructor
    delete p;
}

namespace Detail {

  template <
    class T,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
  >
  class ValueFacadeImpl: public ValueFacadeInterface<T,CR> {
    typedef ValueFacadeImpl<T,CR>       this_type;
    typedef ValueFacadeInterface<T,CR>  base_type;
  public:
    ValueFacadeImpl(T const &value = T()): value(value) {}

    T value;

    void  implSet(const T &v) { value = v; }
    CR    implGet() const { return value; }
  };

} // namespace Detail

/// This class is a facade for a value of type T.
/// \example test_value.cpp
template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class ValueFacade
: public FacadeFoundation<
    ValueFacade<T,CR>,
    ValueFacadeInterface<T,CR> >,
  public Detail::ValueVirtualUser<
    ValueFacade<T,CR>,T,CR>
{
  typedef ValueFacade<T,CR>                                        this_type;
  typedef FacadeFoundation<this_type,ValueFacadeInterface<T,CR> >  base1_type;
  typedef Detail::ValueVirtualUser<this_type,T,CR>                 base2_type;

  friend class ValueInterface<this_type,T,CR>;
  friend class Detail::ValueVirtualUser<this_type,T,CR>;
private:
  /// This is required by Detail::ValueVirtualUser.
  typename base1_type::ImplType *getImpl() const
    { return FacadeCoreAccess::getImpl(*this); }
protected:
  /// This is required by FacadeRef.
  explicit ValueFacade(typename base1_type::_StorageType const &x)
    : base1_type(x) {}
public:
  template <class PImpl>
  ValueFacade(PImpl *p, typename boost::enable_if<boost::is_base_of<typename base1_type::ImplType, PImpl>, int>::type dummy = 0)
    : base1_type(typename base1_type::_StorageType(p)) {}
  template <class PImpl>
  ValueFacade(PImpl const &p, typename boost::enable_if<boost::is_same<PImpl, typename base1_type::SmartPtr>, int>::type dummy = 0)
    : base1_type(typename base1_type::_StorageType(p)) {}
//ValueFacade(::boost::intrusive_ptr<MaybeValueFacadeInterface<T,CR> > const &p)
//  : base1_type(typename base1_type::_StorageType(p->getValueFacadeInterface())) {}
//ValueFacade(MaybeValueFacadeInterface<T,CR> *p)
//  : base1_type(typename base1_type::_StorageType(p->getValueFacadeInterface())) {}

  ValueFacade(T const &value = T())
    : base1_type(new Detail::ValueFacadeImpl<T,CR>(value)) {}
  ValueFacade(this_type const &value)
    : base1_type(new Detail::ValueFacadeImpl<T,CR>(value)) {}
  template <class DD, typename TT, typename CRCR>
  ValueFacade(ValueInterface<DD,TT,CRCR> const &value)
    : base1_type(new Detail::ValueFacadeImpl<T,CR>(value)) {}

  using base2_type::operator =;
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

#endif // _INCLUDED_COSUPPORT_DATATYPES_VALUEFACADE_HPP
