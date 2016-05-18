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
 *   FOR AFacade PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
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
 * OF MERCHANTABILITY AND FITNESS FOR AFacade PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifndef _INCLUDED_TEST_FACADE_LIB_HPP
#define _INCLUDED_TEST_FACADE_LIB_HPP

#include <CoSupport/DataTypes/Facade.hpp>
#include <CoSupport/SmartPtr/intrusive_refcount_ptr.hpp>

namespace Something {

namespace Detail {

  class AImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(AImpl, PAImpl);

  class BImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(BImpl, PBImpl);

  class CImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(CImpl, PCImpl);

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
  explicit AFacade(const SmartPtr &p);
public:
  AFacade();
  AFacade(const this_type &);

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
  ImplType *getImpl() const;

  explicit BFacade(const _StorageType &x) : FFType(x) {}
  explicit BFacade(const SmartPtr &p);
public:
  BFacade();
  BFacade(const this_type &);

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
  ImplType *getImpl() const;

  explicit CFacade(const _StorageType &x) : FFType(x) {}
  explicit CFacade(const SmartPtr &p);
public:
  CFacade();
  CFacade(const this_type &);

  static Ptr upcast(const AFacade &);
};

} // namespace Something

#endif // _INCLUDED_TEST_FACADE_LIB_HPP
