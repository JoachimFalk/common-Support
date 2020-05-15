// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include "test_facade_lib.hpp"

#include <CoSupport/SmartPtr/RefCount.hpp>

namespace Something {

using CoSupport::DataTypes::FacadeCoreAccess;

namespace Detail {

  class AImpl: public CoSupport::SmartPtr::RefCount {
    typedef AImpl this_type;
  public:
    virtual this_type *dup() const
      { return new this_type(*this); }

    void aNonConst() {}
    void aConst() const {}

    virtual ~AImpl() {}
  };
  IMPL_INTRUSIVE_REFCOUNT_PTR(AImpl)

  class BImpl: public AImpl {
    typedef BImpl this_type;
  public:
    virtual this_type *dup() const
      { return new this_type(*this); }
  };
  IMPL_INTRUSIVE_REFCOUNT_PTR(BImpl)

  class CImpl: public AImpl {
    typedef CImpl this_type;
  public:
    virtual this_type *dup() const
      { return new this_type(*this); }
  };
  IMPL_INTRUSIVE_REFCOUNT_PTR(CImpl)

} // namespace Detail

AFacade::AFacade()
 : FFType(new ImplType()) {}

AFacade::AFacade(const SmartPtr &p)
 : FFType(p) {}

AFacade::AFacade(const this_type &x)
 : FFType(FacadeCoreAccess::getImpl(x)->dup()) {}

void AFacade::aNonConst()
  { return FacadeCoreAccess::getImpl(*this)->aNonConst(); }

void AFacade::aConst() const
  { return FacadeCoreAccess::getImpl(*this)->aConst(); }

BFacade::BFacade()
 : FFType(new ImplType()) {}

BFacade::BFacade(const SmartPtr &p)
 : FFType(_StorageType(p)) {}

BFacade::BFacade(const this_type &x)
 : FFType(FacadeCoreAccess::getImpl(x)->dup()) {}

BFacade::Ptr BFacade::upcast(const AFacade &x)
  { return SmartPtr(dynamic_cast<ImplType *>(FacadeCoreAccess::getImplBase(x))); }

CFacade::CFacade()
 : FFType(new ImplType()) {}

CFacade::CFacade(const SmartPtr &p)
 : FFType(_StorageType(p)) {}

CFacade::CFacade(const this_type &x)
 : FFType(FacadeCoreAccess::getImpl(x)->dup()) {}

CFacade::Ptr CFacade::upcast(const AFacade &x)
  { return SmartPtr(dynamic_cast<ImplType *>(FacadeCoreAccess::getImplBase(x))); }

} // namespace Something
