// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_ALLOCATORS_IDALLOCREQUEST_HPP
#define _INCLUDED_COSUPPORT_ALLOCATORS_IDALLOCREQUEST_HPP

#include <string>
#include <cassert>

#include <CoSupport/limits.h>
#include <CoSupport/Math/string_hash.hpp>

namespace CoSupport { namespace Allocators {

namespace Detail {

  template <typename T>
  struct IdRange {
    static const T   min  = MIN_TYPE(T);
    static const T   max  = MAX_TYPE(T);
    static const int bits = BITS_TYPE(T);
  };

} // namespace Detail

template <typename ID> 
class IdAllocRequest {
  typedef IdAllocRequest<ID> this_type;
public:
  typedef enum { Force, Suggest, Auto, Anon } AllocType;
protected:
  ID         _id;
  AllocType  _allocType;
public:
  IdAllocRequest(AllocType a = Auto)
    : _id(0), _allocType(a) {
    assert(a == Auto || a == Anon);
  }
  IdAllocRequest(const ID id, AllocType a = Force)
    : _id(id), _allocType(a) {
    assert(a == Force || a == Suggest);
  }

  ID        id() const { return _id; }
  AllocType allocType() const { return _allocType; }

  // Derive id from name if allocType == Auto
  template <class IDRANGE>
  this_type &idFromName(const std::string &name) {
    if (_allocType == Auto) {
      CoSupport::Math::FNV<IDRANGE::bits> hf;
      _id = IDRANGE::min + hf(name.c_str());
      assert(_id <= IDRANGE::max);
      _allocType = Suggest;
    }
    return *this;
  }
  this_type &idFromName(const std::string &name)
    { return idFromName<Detail::IdRange<ID> >(name); }
};

} } // namespace CoSupport::Allocators

#endif // _INCLUDED_COSUPPORT_ALLOCATORS_IDALLOCREQUEST_HPP
