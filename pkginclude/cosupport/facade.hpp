/*
 * Copyright (c) 2004-2007 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_FACADE_HPP
#define _INCLUDED_COSUPPORT_FACADE_HPP

#include <cosupport/intrusive_refcount_ptr.hpp>

namespace CoSupport {

  template <typename T> class FacadeBase;
  template <typename T> class FacadeUser;
  template <typename T> class FacadePolish;
  template <typename T> class FacadeRefPolish;

  template <typename T>
  class FacadeBase {
    friend class FacadePolish<T>;
    friend class FacadePolish<const T>;
    friend class FacadeRefPolish<T>;
    friend class FacadeRefPolish<const T>;

    typedef FacadeBase<T>             this_type;
  protected:
    typedef T                         ImplType;
    typedef ::boost::intrusive_ptr<T> SmartPtr;
  private:
    SmartPtr pObj;
  protected:
    FacadeBase(const SmartPtr &obj)
      : pObj(obj) {}

    ImplType       &impl()       { return *pObj; }
    ImplType const &impl() const { return *pObj; }
  private:
    ImplType *dup() const;
    void      assign(const this_type &obj);

    FacadeBase(const this_type &);             // disable
  };

#define DECL_FACADECLASS(T)                       \
  template <>                                     \
  class FacadeUser<T >: public FacadeBase<T >

#define DECL_FACADEDECORATION                     \
  private:                                        \
    typedef FacadeUser<ImplType> this_type;       \
  protected:                                      \
    FacadeUser(const SmartPtr &obj)               \
      : FacadeBase<ImplType>(obj) {}

  template <typename T>
  class FacadePolish: public FacadeUser<T> {
    typedef FacadePolish<T>                   this_type;
  protected:
    typedef typename FacadeUser<T>::ImplType  ImplType;
    typedef typename FacadeUser<T>::SmartPtr  SmartPtr;
  public:
    FacadePolish()
      : FacadeUser<T>() {}
    FacadePolish(const SmartPtr &obj)
      : FacadeUser<T>(obj) {}
    FacadePolish(const this_type &obj)
      : FacadeUser<T>(obj.dup()) {}

    this_type &operator = (const FacadePolish<T> &obj) 
      { assign(obj); return *this; }

    // bounce other assignments to FacadeUser<T>
    template <typename X>
    this_type &operator = (const X &obj) {
      *static_cast<FacadeUser<T> *>(this) = obj;
      return *this;
    }
  };
  template <typename T>
  class FacadeRefPolish: public FacadePolish<T> {
    typedef FacadeRefPolish<T>                  this_type;
  protected:
    typedef typename FacadePolish<T>::ImplType  ImplType;
    typedef typename FacadePolish<T>::SmartPtr  SmartPtr;
  public:
    FacadeRefPolish(const SmartPtr &obj)
      : FacadePolish<T>(obj) {}
    FacadeRefPolish(const this_type &obj)
      : FacadePolish<T>(obj.pObj) {}
    FacadeRefPolish(const FacadePolish<T> &obj)
      : FacadePolish<T>(obj.pObj) {}

    this_type &operator = (const FacadePolish<T> &obj) 
      { assign(obj); return *this; }

    // bounce other assignments to FacadeUser<T>
    template <typename X>
    this_type &operator = (const X &obj) {
      *static_cast<FacadeUser<T> *>(this) = obj;
      return *this;
    }
  };
  template <typename T>
  class FacadeRefPolish<const T>: public FacadePolish<T> {
    typedef FacadeRefPolish<const T>            this_type;
  protected:
    typedef typename FacadePolish<T>::ImplType  ImplType;
    typedef typename FacadePolish<T>::SmartPtr  SmartPtr;
  public:
    FacadeRefPolish(const SmartPtr &obj)
      : FacadePolish<T>(obj) {}
    FacadeRefPolish(const this_type &obj)
      : FacadePolish<T>(obj.pObj) {}
    FacadeRefPolish(const FacadePolish<T> &obj)
      : FacadePolish<T>(obj.pObj) {}
  private:
    // no asignment should be possible
    template <typename X>
    this_type &operator = (const X &obj);
  };

} // namespace CoSupport

#endif // _INCLUDED_COSUPPORT_FACADE_HPP
