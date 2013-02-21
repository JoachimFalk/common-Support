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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VALUEFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VALUEFACADE_HPP

#include "ValueVirtual.hpp"
#include "Facade.hpp"

#include "../SmartPtr/intrusive_refcount_ptr.hpp"

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

/// This class is a facade for a std::set look alike containing values of type T.
/// \example test_set.cpp
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
public:
  ValueFacade(T const &value = T())
    : base1_type(new Detail::ValueFacadeImpl<T,CR>(value)) {}
  ValueFacade(this_type const &value)
    : base1_type(new Detail::ValueFacadeImpl<T,CR>(value)) {}
  template <class DD, typename TT, typename CRCR>
  ValueFacade(ValueInterface<DD,TT,CRCR> const &value)
    : base1_type(new Detail::ValueFacadeImpl<T,CR>(value)) {}

  ValueFacade(typename base1_type::SmartPtr const &p)
    : base1_type(p) {}
  ValueFacade(::boost::intrusive_ptr<MaybeValueFacadeInterface<T,CR> > const &p)
    : base1_type(p->getValueFacadeInterface()) {}

  using base2_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VALUEFACADE_HPP
