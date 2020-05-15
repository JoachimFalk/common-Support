// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2014 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_TEST_FACADE_LIB_HPP
#define _INCLUDED_TEST_FACADE_LIB_HPP

#include <CoSupport/DataTypes/Facade.hpp>
#include <CoSupport/SmartPtr/intrusive_refcount_ptr.hpp>

namespace Something {

namespace Detail {

  class AImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(AImpl, PAImpl)

  class BImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(BImpl, PBImpl)

  class CImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(CImpl, PCImpl)

} // namespace Detail

class AFacade
: public CoSupport::DataTypes::FacadeFoundation<
    AFacade,
    Detail::AImpl>
{
  typedef AFacade this_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType>;
protected:
  explicit AFacade(const _StorageType &x) : FFType(x) {}
public:
  AFacade();
  AFacade(const this_type &);
  AFacade(const SmartPtr &);

  void aNonConst();
  void aConst() const;
};

class BFacade
: public CoSupport::DataTypes::FacadeFoundation<
    BFacade,
    Detail::BImpl,
    AFacade>
{
  typedef BFacade this_type;
  typedef AFacade base_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType, base_type>;
protected:
  explicit BFacade(const _StorageType &x) : FFType(x) {}
public:
  BFacade();
  BFacade(const this_type &);
  BFacade(const SmartPtr &);

  static Ptr upcast(const AFacade &);
};

class CFacade
: public CoSupport::DataTypes::FacadeFoundation<
    CFacade,
    Detail::CImpl,
    AFacade>
{
  typedef CFacade this_type;
  typedef AFacade base_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType, base_type>;
protected:
  explicit CFacade(const _StorageType &x) : FFType(x) {}
public:
  CFacade();
  CFacade(const this_type &);
  CFacade(const SmartPtr &);

  static Ptr upcast(const AFacade &);
};

} // namespace Something

#endif // _INCLUDED_TEST_FACADE_LIB_HPP
