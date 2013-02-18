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

#include "MaybeValueVirtual.hpp"
#include "Facade.hpp"

#include "../SmartPtr/intrusive_refcount_ptr.hpp"

namespace CoSupport { namespace DataTypes {

/// This class represents a facade interface for a storage which contains a value of type T.
template <class D, typename T, typename R = T const &>
class MaybeValueFacadeInterface
: public MaybeValueVirtualInterface<D,T,R>,
  public SmartPtr::RefCount
{
public:
  using MaybeValueVirtualInterface<D,T,R>::operator =;
};

template <typename T, typename R = T const &>
class MaybeValueFacade;

namespace Detail {

  template <typename T, typename R = T const &>
  struct MaybeValueFacadeImpl: public MaybeValueFacadeInterface<MaybeValueFacadeImpl<T,R>, T, R> {
    friend class MaybeValueFacade<T,R>;
  };

  template <typename T, typename R>
  void intrusive_ptr_release(MaybeValueFacadeImpl<T,R> *p) {
    if (p->del_ref())
      // RefCountObject has virtual destructor
      delete p;
  }

} // namespace Detail

/// This class is a facade for a storage which contains a value of type T.
template <typename T, typename R>
class MaybeValueFacade
: public FacadeFoundation<
    MaybeValueFacade<T,R>,
    Detail::MaybeValueFacadeImpl<T,R> >,
  public MaybeValueInterface<MaybeValueFacade<T,R>,T,R>
{
  typedef MaybeValueFacade<T,R>                                             this_type;
  typedef FacadeFoundation<this_type, Detail::MaybeValueFacadeImpl<T, R> >  base1_type;
  typedef MaybeValueInterface<this_type,T,R>                                base2_type;

  friend class MaybeValueInterface<this_type,T,R>;
protected:
  void implSet(const T &val) { this->getImpl()->implSet(val); }
  R    implGet() const { return this->getImpl()->implGet(); }
  void implUndef() { return this->getImpl()->implUndef(); }
  bool implIsDefined() const { return this->getImpl()->implIsDefined(); }
public:
  MaybeValueFacade()
    : base1_type(reinterpret_cast<typename base1_type::ImplType *>(new MaybeValue<T, MaybeValueFacadeInterface, R>())) {}
  MaybeValueFacade(T const &val)
    : base1_type(reinterpret_cast<typename base1_type::ImplType *>(new MaybeValue<T, MaybeValueFacadeInterface, R>(val))) {}
  template <class DD, typename TT, typename RR>
  MaybeValueFacade(MaybeValueInterface<DD,TT,RR> const &val)
    : base1_type(reinterpret_cast<typename base1_type::ImplType *>(new MaybeValue<T, MaybeValueFacadeInterface, R>(val.get()))) {}
  template <class DD, typename TT, typename RR>
  MaybeValueFacade(MaybeValueFacadeInterface<DD,TT,RR> *impl)
    : base1_type(reinterpret_cast<typename base1_type::ImplType *>(impl)) {}
  MaybeValueFacade(typename this_type::SmartPtr const &impl)
    : base1_type(reinterpret_cast<typename base1_type::ImplType *>(impl.get())) {}

  using base2_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEFACADE_HPP
