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

#include "test_facade_lib.hpp"

#include <CoSupport/SmartPtr/RefCount.hpp>

namespace Something {

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
  IMPL_INTRUSIVE_REFCOUNT_PTR(AImpl);

  class BImpl: public AImpl {
    typedef BImpl this_type;
  public:
    virtual this_type *dup() const
      { return new this_type(*this); }
  };
  IMPL_INTRUSIVE_REFCOUNT_PTR(BImpl);

  class CImpl: public AImpl {
    typedef CImpl this_type;
  public:
    virtual this_type *dup() const
      { return new this_type(*this); }
  };
  IMPL_INTRUSIVE_REFCOUNT_PTR(CImpl);

} // namespace Detail

A::A()
 : FFType(new ImplType()) {}

A::A(const SmartPtr &p)
 : FFType(p) {}

A::A(const this_type &x)
 : FFType(x.getImpl()->dup()) {}

void A::aNonConst()
  { return getImpl()->aNonConst(); }

void A::aConst() const
  { return getImpl()->aConst(); }

B::B()
 : FFType(new ImplType()) {}

B::B(const SmartPtr &p)
 : FFType(p) {}

B::B(const this_type &x)
 : FFType(x.getImpl()->dup()) {}

B::ImplType *B::getImpl() const
  { return static_cast<ImplType *>(this->pImpl.get()); }

B::Ptr B::upcast(const A &x)
//{ return ::boost::dynamic_pointer_cast<ImplType>(x.pImpl); }
  { return ::boost::dynamic_pointer_cast<ImplType>(static_cast<const this_type &>(x).pImpl); }

C::C()
 : FFType(new ImplType()) {}

C::C(const SmartPtr &p)
 : FFType(p) {}

C::C(const this_type &x)
 : FFType(x.getImpl()->dup()) {}

C::ImplType *C::getImpl() const
  { return static_cast<ImplType *>(this->pImpl.get()); }

C::Ptr C::upcast(const A &x)
//{ return ::boost::dynamic_pointer_cast<ImplType>(x.pImpl); }
  { return ::boost::dynamic_pointer_cast<ImplType>(static_cast<const this_type &>(x).pImpl); }

} // namespace Something

