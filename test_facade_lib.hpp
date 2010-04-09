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

class A
: public CoSupport::DataTypes::FacadeFoundation<
    A,
    Detail::AImpl>
{
  typedef A this_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType>;
protected:
  A(const SmartPtr &p);
public:
  A();
  A(const this_type &);
};

class B
: public CoSupport::DataTypes::FacadeFoundation<
    B,
    Detail::BImpl,
    A,
    Detail::PBImpl>
{
  typedef B this_type;
  typedef A base_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType, base_type, SmartPtr>;
protected:
  ImplType *getImpl() const;

  B(const SmartPtr &p);
public:
  B();
  B(const this_type &);

  static Ptr upcast(const A &);
};

class C
: public CoSupport::DataTypes::FacadeFoundation<
    C,
    Detail::CImpl,
    A,
    Detail::PCImpl>
{
  typedef C this_type;
  typedef A base_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType, base_type, SmartPtr>;
protected:
  ImplType *getImpl() const;

  C(const SmartPtr &p);
public:
  C();
  C(const this_type &);

  static Ptr upcast(const A &);
};

} // namespace Something

#endif // _INCLUDED_TEST_FACADE_LIB_HPP


