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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEFACADE_HPP

#include "../compatibility-glue/nullptr.h"
#include "../SmartPtr/intrusive_refcount_ptr.hpp"

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
public:
  MaybeValueFacade(boost::blank value = boost::blank())
    : base1_type(new Detail::MaybeValueFacadeImpl<T,CR>(value)) {}
  MaybeValueFacade(T const &value)
    : base1_type(new Detail::MaybeValueFacadeImpl<T,CR>(value)) {}
  MaybeValueFacade(this_type const &value)
    : base1_type(new Detail::MaybeValueFacadeImpl<T,CR>(value)) {}
  template <class DD, typename TT, typename CRCR>
  MaybeValueFacade(MaybeValueInterface<DD,TT,CRCR> const &value)
    : base1_type(new Detail::MaybeValueFacadeImpl<T,CR>(value)) {}

  explicit MaybeValueFacade(typename base1_type::_StorageType const &x)
    : base1_type(x) {}
  MaybeValueFacade(typename base1_type::SmartPtr const &p)
    : base1_type(p) {}

  using base2_type::operator =;

  this_type &operator =(const this_type &value)
    { this->set(value); return *this; }
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEFACADE_HPP
