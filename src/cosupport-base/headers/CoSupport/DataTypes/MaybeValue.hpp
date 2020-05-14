// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP

#include "../compatibility-glue/nullptr.h"
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
    { return boost::get<boost::blank>(&value) == nullptr; }
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
#ifdef _MSC_VER
  // Visual Studio can't pull the following template assignment operator from base_type.
  // Hence, we replicate it here.
  template <class DD, typename TT, typename CRCR>
  this_type &operator = (const MaybeValueInterface<DD, TT, CRCR> &val)
    { this->set(val); return *this; }
#endif //_MSC_VER
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP
