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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEFACADE_HPP

#include "../compatibility-glue/nullptr.h"
#include <CoSupport/SmartPtr/intrusive_refcount_ptr.hpp>

#include "MaybeValueVirtual.hpp"
#include "Facade.hpp"

namespace CoSupport { namespace DataTypes {

template <
  class T,
  class CR
>
class ValueFacadeInterface;

template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class MaybeValueFacadeInterface
: public SmartPtr::RefCount,//< this must be first otherwise the reinterpret cast to ValueFacadeInterface will not work!
  public MaybeValueVirtualInterface<T,CR>
{
public:
  ValueFacadeInterface<T,CR> const *getValueFacadeInterface() const
    { return reinterpret_cast<ValueFacadeInterface<T,CR> const *>(this); }
  ValueFacadeInterface<T,CR>       *getValueFacadeInterface()
    { return reinterpret_cast<ValueFacadeInterface<T,CR>       *>(this); }
};

template <class T, class CR>
void intrusive_ptr_release(MaybeValueFacadeInterface<T,CR> *p) {
  if (p->del_ref())
    // RefCountObject has virtual destructor
    delete p;
}

namespace Detail {

  template <
    class T,
    class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
  >
  class MaybeValueFacadeImpl: public MaybeValueFacadeInterface<T,CR> {
    typedef MaybeValueFacadeImpl<T,CR>       this_type;
    typedef MaybeValueFacadeInterface<T,CR>  base_type;
  public:
    MaybeValueFacadeImpl(boost::blank value = boost::blank()): value(value) {}
    MaybeValueFacadeImpl(T const &value): value(value) {}
    template <class DD, class TT, class CRCR>
    MaybeValueFacadeImpl(const MaybeValueInterface<DD,TT,CRCR> &v)
      : value(v.isDefined()
          ? boost::variant<boost::blank, T>(v.get())
          : boost::variant<boost::blank, T>(boost::blank())) {}

    boost::variant<boost::blank, T> value;

    void  implSet(const T &v) { value = v; }
    CR    implGet() const { return boost::get<T>(value); }
    void  implUndef() { value = boost::blank(); }
    bool  implIsDefined() const { return boost::get<T>(&value) != nullptr; }
  };

} // namespace Detail

/// This class is a facade for a possible value of type T.
/// \example test_maybevalue.cpp
template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class MaybeValueFacade
: public FacadeFoundation<
    MaybeValueFacade<T,CR>,
    MaybeValueFacadeInterface<T,CR> >,
  public Detail::MaybeValueVirtualUser<
    MaybeValueFacade<T,CR>,T,CR>
{
  typedef MaybeValueFacade<T,CR>                                        this_type;
  typedef FacadeFoundation<this_type,MaybeValueFacadeInterface<T,CR> >  base1_type;
  typedef Detail::MaybeValueVirtualUser<this_type,T,CR>                 base2_type;

  friend class MaybeValueInterface<this_type,T,CR>;
  friend class Detail::MaybeValueVirtualUser<this_type,T,CR>;
private:
  /// This is required by Detail::MaybeValueVirtualUser.
  typename base1_type::ImplType *getImpl() const
    { return FacadeCoreAccess::getImpl(*this); }
protected:
  /// This is required by FacadeRef.
  explicit MaybeValueFacade(typename base1_type::_StorageType const &x)
    : base1_type(x) {}
public:
  MaybeValueFacade(typename base1_type::SmartPtr const &p)
    : base1_type(typename base1_type::_StorageType(p)) {}
  MaybeValueFacade(typename base1_type::ImplType *p)
    : base1_type(typename base1_type::_StorageType(p)) {}

  MaybeValueFacade(boost::blank value = boost::blank())
    : base1_type(new Detail::MaybeValueFacadeImpl<T,CR>(value)) {}
  MaybeValueFacade(T const &value)
    : base1_type(new Detail::MaybeValueFacadeImpl<T,CR>(value)) {}
  MaybeValueFacade(this_type const &value)
    : base1_type(new Detail::MaybeValueFacadeImpl<T,CR>(value)) {}
  template <class DD, typename TT, typename CRCR>
  MaybeValueFacade(MaybeValueInterface<DD,TT,CRCR> const &value)
    : base1_type(new Detail::MaybeValueFacadeImpl<T,CR>(value)) {}

  using base2_type::operator =;
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

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEFACADE_HPP
