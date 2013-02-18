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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VALUEVIRTUAL_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VALUEVIRTUAL_HPP

#include "Value.hpp"

namespace CoSupport { namespace DataTypes {

/// This class represents a virtual interface for a storage which contains a value of type T.
/// \example test_value.cpp
template <class D, typename T, typename R = T const &>
class ValueVirtualInterface
: public ValueInterface<D,T,R> {
  typedef ValueVirtualInterface<D,T,R> this_type;
  typedef ValueInterface<D,T,R>        base_type;
public:
  virtual ~ValueVirtualInterface() {}

  using base_type::operator =;
protected:
  virtual void  implSet(const T &) = 0;
  virtual R     implGet() const = 0;
};

/// This class is a wrapper to access a value virtual interface.
/// \example test_value.cpp
template <typename T, typename R = T const &>
class ValueVirtual
: public ValueInterface<ValueVirtual<T,R>,T,R> {
  typedef ValueVirtual<T,R>             this_type;
  typedef ValueInterface<this_type,T,R> base_type;

  friend class ValueInterface<this_type,T,R>;
private:
  struct Impl: public ValueVirtualInterface<Impl, T, R> {
    friend class ValueVirtual<T,R>;
  } *impl;
protected:
  void implSet(const T &val) { impl->implSet(val); }
  R    implGet() const { return impl->implGet(); }
public:
  ValueVirtual()
    : impl(reinterpret_cast<Impl *>(new Value<T, ValueVirtualInterface, R>())) {}
  ValueVirtual(T const &val)
    : impl(reinterpret_cast<Impl *>(new Value<T, ValueVirtualInterface, R>(val))) {}
  template <class DD, typename TT, typename RR>
  ValueVirtual(ValueInterface<DD,TT,RR> const &val)
    : impl(reinterpret_cast<Impl *>(new Value<T, ValueVirtualInterface, R>(val.get()))) {}
  template <class DD, typename TT, typename RR>
  ValueVirtual(ValueVirtualInterface<DD,TT,RR> *impl)
    : impl(reinterpret_cast<Impl *>(impl)) {}

  ~ValueVirtual() { delete impl; }

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VALUEVIRTUAL_HPP
