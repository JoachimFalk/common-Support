//  -*- tab-width:8; intent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 expandtab:
/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
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

#include "MaybeValueInterface.hpp"

namespace CoSupport { namespace DataTypes {

/// This class implements the interface for a storage which may contain a value of type T.
/// \example test_maybevalue.cpp
template <
  class T,
  class CR = typename ::boost::add_reference<typename ::boost::add_const<T>::type>::type
>
class MaybeValue: public MaybeValueInterface<MaybeValue<T,CR>,T,CR> {
  typedef MaybeValue<T,CR>                    this_type;
  typedef MaybeValueInterface<this_type,T,CR> base_type;

  friend class MaybeValueInterface<this_type,T,CR>;
private:
  typedef boost::variant<boost::blank, T>    storage_type;

  storage_type value;
protected:
  void implSet(const T &val)
    { value = val; }
  CR   implGet() const
    { return boost::get<T>(value); }
  void implUndef()
    { value = boost::blank(); }
  bool implIsDefined() const
    { return boost::get<boost::blank>(&value) == NULL; }
public:
  MaybeValue(boost::blank value = boost::blank())
    : value(value) {}
  MaybeValue(T const &value)
    : value(value) {}
  template <class DD, typename TT, typename CRCR>
  MaybeValue(MaybeValueInterface<DD,TT,CRCR> const &value)
    : value(value.isDefined()
        ? storage_type(value.get())
        : storage_type(boost::blank())) {}

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP
