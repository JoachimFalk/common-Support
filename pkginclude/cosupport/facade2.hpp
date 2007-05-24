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

#ifndef _INCLUDED_COSUPPORT_FACADE2_HPP
#define _INCLUDED_COSUPPORT_FACADE2_HPP

#include <boost/intrusive_ptr.hpp>

#include <cosupport/typetransform.hpp>

namespace CoSupport {

namespace Detail {
  template <class Impl>
  class Storage {
  protected:
    typedef Impl                              ImplType;
    typedef ::boost::intrusive_ptr<ImplType>  SmartPtr;

    SmartPtr pImpl;
  protected:
    explicit Storage(const SmartPtr &p)
      : pImpl(p) {}
  };
} // namespace Detail

template <class T, template <class> class C>
class FacadeRef: public T {
protected:
  typedef typename T::_H::ImplType  ImplType;
  typedef typename T::_H::SmartPtr  SmartPtr;

  template <class TT, template <class> class CC> friend class FacadeRef;
public:
  explicit FacadeRef(const SmartPtr &p)
    : T(p) {}
  FacadeRef(const FacadeRef<T,C> &t)
    : T(t._impl()) {}
};

template <class T, template <class> class C>
class FacadePtr {
  typedef FacadePtr<T,C> this_type;
protected:
  typedef typename T::_H::ImplType  ImplType;
  typedef typename T::_H::SmartPtr  SmartPtr;

  template <class TT, template <class> class CC> friend class FacadePtr;
public:
  typedef typename T::_H::RefConst  RefConst;
  typedef typename T::_H::Ref       Ref;
  typedef typename T::_H::PtrConst  PtrConst;
  typedef typename T::_H::Ptr       Ptr;

  typedef T &(this_type::*unspecified_bool_type)();
private:
  FacadeRef<T, C> ref;
public:
  explicit FacadePtr(const SmartPtr &p)
    : ref(p) {}
  FacadePtr(const FacadePtr<T, CoSupport::Type::Mutable> &t)
    : ref(t.ref) {}
  FacadePtr(typename C<T>::type *t)
    : ref(t != NULL ? t->_impl() : SmartPtr()) {}

  T &operator *()
    { return ref; }
  const T &operator *() const
    { return ref; }
  T *operator ->()
    { return &ref; }
  const T *operator ->() const
    { return &ref; }

  this_type &operator =(const this_type &x)
    { ref._impl(x.ref._impl()); return *this; }

  operator unspecified_bool_type() const {
    return ref._impl() != NULL
      ? static_cast<unspecified_bool_type>(&this_type::operator *)
      : NULL;
  }
  unspecified_bool_type operator ==(const this_type &x) const {
    return ref._impl() == x.ref._impl()
      ? static_cast<unspecified_bool_type>(&this_type::operator *)
      : NULL;
  }
};

template <class Derived, class Impl, class Base = Detail::Storage<Impl> >
class FacadeFoundation: public Base {
  template <class TT, template <class> class CC> friend class FacadeRef;
  template <class TT, template <class> class CC> friend class FacadePtr;
protected:
  typedef Impl                                              ImplType;
  typedef ::boost::intrusive_ptr<ImplType>                  SmartPtr;
  typedef FacadeFoundation<Derived,Impl,Base>               FFType;
public:
  typedef const FacadeRef<Derived, CoSupport::Type::Const>  RefConst;
  typedef FacadeRef<Derived, CoSupport::Type::Mutable>      Ref;
  typedef FacadePtr<Derived, CoSupport::Type::Const>        PtrConst;
  typedef FacadePtr<Derived, CoSupport::Type::Mutable>      Ptr;
private:
  // hack for type downward traversal
  typedef FFType                                            _H;
  //
  // Curiously Recurring Template interface.
  //
  const SmartPtr &_impl() const
    { return static_cast<const Derived*>(this)->getImpl(); }
  void _impl(const SmartPtr &p)
    { return static_cast<Derived*>(this)->setImpl(p); }
protected:
  // may be overridden in derived class
  const SmartPtr &getImpl() const
    { return this->pImpl; }
  // may be overridden in derived class
  void setImpl(const SmartPtr &p)
    { this->pImpl = p; }

  explicit FacadeFoundation(const typename Base::SmartPtr &p)
    : Base(p) {}
public:
  operator RefConst &() const // do dirty magic
    { return reinterpret_cast<RefConst &>(*this); }
  operator Ref &() // do dirty magic
    { return reinterpret_cast<Ref &>(*this); }

  PtrConst toPtr() const
    { return PtrConst(static_cast<const Derived *>(this)); }
  Ptr toPtr()
    { return Ptr(static_cast<Derived *>(this)); }
private:
  // default no copy no assign
  FacadeFoundation(const FacadeFoundation &);
  FacadeFoundation &operator =(const FacadeFoundation &);
};

namespace Type {

  template <typename T>
  struct Const<FacadeRef<T, Mutable> >
    { typedef const FacadeRef<T, Type::Const> type; };
  template <typename T>
  struct Const<FacadePtr<T, Mutable> >
    { typedef const FacadePtr<T, Type::Const> type; };

  template <typename T>
  struct Mutable<const FacadeRef<T, Const> >
    { typedef FacadeRef<T, Type::Mutable> type; };
  template <typename T>
  struct Mutable<const FacadePtr<T, Const> >
    { typedef FacadePtr<T, Type::Mutable> type; };

  template <typename T>
  struct ToggleConst<const FacadeRef<T, Const> >
    { typedef FacadeRef<T, Type::Mutable> type; };
  template <typename T>
  struct ToggleConst<FacadeRef<T, Mutable> >
    { typedef const FacadeRef<T, Type::Const> type; };
  template <typename T>
  struct ToggleConst<const FacadePtr<T, Const> >
    { typedef FacadePtr<T, Type::Mutable> type; };
  template <typename T>
  struct ToggleConst<FacadePtr<T, Mutable> >
    { typedef const FacadePtr<T, Type::Const> type; };

} // namespace Type
} // namespace CoSupport

#endif // _INCLUDED_COSUPPORT_FACADE2_HPP
