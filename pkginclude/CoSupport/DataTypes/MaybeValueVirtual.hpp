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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEVIRTUAL_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEVIRTUAL_HPP

#include "MaybeValue.hpp"

namespace CoSupport { namespace DataTypes {

/// This class represents a virtual interface for a storage which may contain a value of type T
/// \example test_maybevalue.cpp
template <class D, typename T, typename R = T const &>
class MaybeValueVirtualInterface
: public MaybeValueInterface<D,T,R> {
  typedef MaybeValueVirtualInterface<D,T,R> this_type;
  typedef MaybeValueInterface<D,T,R>        base_type;
public:
  virtual ~MaybeValueVirtualInterface() {}

  using base_type::operator =;
protected:
  virtual void  implSet(const T &) = 0;
  virtual R     implGet() const = 0;
  virtual void  implUndef() = 0;
  virtual bool  implIsDefined() const = 0;
};

/// This class is a wrapper to access a value virtual interface.
/// \example test_maybevalue.cpp
template <typename T, typename R = T const &>
class MaybeValueVirtual
: public MaybeValueInterface<MaybeValueVirtual<T,R>,T,R> {
  typedef MaybeValueVirtual<T,R>             this_type;
  typedef MaybeValueInterface<this_type,T,R> base_type;

  friend class MaybeValueInterface<this_type,T,R>;
private:
  struct Impl: public MaybeValueVirtualInterface<Impl, T, R> {
    friend class MaybeValueVirtual<T,R>;
  } *impl;
protected:
  void implSet(const T &val) { impl->implSet(val); }
  R    implGet() const { return impl->implGet(); }
  void implUndef() { impl->implUndef(); }
  bool implIsDefined() const { return impl->implIsDefined(); }
public:
  MaybeValueVirtual()
    : impl(reinterpret_cast<Impl *>(new MaybeValue<T, MaybeValueVirtualInterface, R>())) {}
  MaybeValueVirtual(T const &val)
    : impl(reinterpret_cast<Impl *>(new MaybeValue<T, MaybeValueVirtualInterface, R>(val))) {}
  template <class DD, typename TT, typename RR>
  MaybeValueVirtual(MaybeValueInterface<DD,TT,RR> const &val)
    : impl(reinterpret_cast<Impl *>(val.isDefined()
       ? new MaybeValue<T, MaybeValueVirtualInterface, R>(val.get())
       : new MaybeValue<T, MaybeValueVirtualInterface, R>())) {}
  template <class DD, typename TT, typename RR>
  MaybeValueVirtual(MaybeValueVirtualInterface<DD,TT,RR> *impl)
    : impl(reinterpret_cast<Impl *>(impl)) {}

  ~MaybeValueVirtual() { delete impl; }

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEVIRTUAL_HPP
