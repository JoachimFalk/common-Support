//  -*- tab-width:8; intent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 expandtab:
/*
 * Copyright (c) 2004-2006 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP

#include <cassert>
#include <new>
#include <ostream>

#include <boost/blank.hpp>
#include <boost/variant.hpp>

namespace CoSupport { namespace DataTypes {

template <typename T>
class MaybeValue {
  typedef MaybeValue<T> this_type;

  template <typename TT> friend class MaybeValue;
protected:
  boost::variant<boost::blank, T> value;
public:
  MaybeValue()
    : value(boost::blank()) {}
  template <typename TT>
  MaybeValue(const MaybeValue<TT> &val)
    : value(val.value) {}
  MaybeValue(const T &val)
    : value(val) {}

  this_type &operator = (const boost::blank &)
    { this->undef(); return *this; }
  template <typename TT>
  this_type &operator = (const MaybeValue<TT> &val)
    { this->set(val); return *this; }
  this_type &operator = (const T &val)
    { this->set(val); return *this; }

  void undef()
    { value = boost::blank(); }
  bool isDefined() const
    { return boost::get<boost::blank>(&value) == NULL; }

  template <typename TT>
  void set(const MaybeValue<TT> &val)
    { value = val.value; }
  void set(const T &val)
    { value = val; }
  const T &get() const // this may throw
    { return boost::get<T>(value); }

  void dump(std::ostream &out) const {
    if (isDefined())
      out << get();
    else
      out << "undef";
  }
};

template <typename T>
std::ostream &operator <<(std::ostream &out, const MaybeValue<T> &val)
  { val.dump(out); return out; }

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP
