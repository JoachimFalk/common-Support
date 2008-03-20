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
    typedef Storage<Impl>                     this_type;
  public:
    typedef Impl                              ImplType;
    typedef ::boost::intrusive_ptr<ImplType>  SmartPtr;
  protected:
    SmartPtr pImpl; //< this is the storage smart ptr, its name must be pImpl
  protected:
    explicit Storage(const SmartPtr &p)
      : pImpl(p) {}

    void assign(const this_type &x)
      { pImpl = x.pImpl; }
  };
} // namespace Detail

template <class T, template <class> class C>
class FacadeRef: public T {
  typedef FacadeRef<T, C> this_type;

  template <class TT, template <class> class CC> friend class FacadeRef;
public:
  typedef typename T::_H::ImplType  ImplType;
  typedef typename T::_H::SmartPtr  SmartPtr;
public:
  FacadeRef(const SmartPtr &p)
    : T(p) {}
  FacadeRef(const FacadeRef<T,C> &t)
    : T(SmartPtr()) { this->assign(t); }

  // bounce assign to base class
  template <typename TT>
  this_type &operator =(const TT &obj)
    { return static_cast<this_type &>(T::operator =(obj)); }
};

template <class T, template <class> class C>
class FacadePtr {
  typedef FacadePtr<T,C> this_type;

  template <class TT, template <class> class CC> friend class FacadePtr;
private:
  typedef typename C<T>::type       value_type;
public:
  typedef typename T::_H::ImplType  ImplType;
  typedef typename T::_H::SmartPtr  SmartPtr;

  typedef typename T::_H::ConstRef  ConstRef;
  typedef typename T::_H::Ref       Ref;
  typedef typename T::_H::ConstPtr  ConstPtr;
  typedef typename T::_H::Ptr       Ptr;

  typedef value_type &(this_type::*unspecified_bool_type)() const;
private:
  FacadeRef<T, C> ref;
//FIXME: protected:
public:
  ImplType *getImpl() const
    { return ref._impl(); }
public:
  FacadePtr(const SmartPtr &p)
    : ref(p) {}
  FacadePtr(const FacadePtr<T, CoSupport::Type::Mutable> &t)
    : ref(t.ref) {}
  FacadePtr(typename C<T>::type *t)
    : ref(SmartPtr()) { if (t) ref.assign(*t); }

  value_type &operator *() const {
    return const_cast<value_type &>
      (static_cast<const T &>(ref));
  }
  value_type *operator ->() const {
    return const_cast<value_type *>
      (static_cast<const T *>(&ref));
  }

  this_type &operator =(const this_type &x)
    { ref.assign(x.ref); return *this; }

  // "operator bool()" would be utilized in comparisons if the
  // comp. operator is not specified (e.g. if "operator!="
  // is not defined, (a != b) results always [assuming pImpl
  // is valid] in (true != true), which is WRONG!!!)
  // -> define all comparison operators
    operator unspecified_bool_type() const {
    return ref.pImpl != NULL
      ? static_cast<unspecified_bool_type>(&this_type::operator *)
      : NULL;
  }
  bool operator ==(const FacadePtr<T,Type::Const> &x) const
    { return ref.pImpl.get() == x.ref.pImpl.get(); }
  bool operator ==(const T *x) const
    { return ref.pImpl.get() == (x ? x->pImpl.get() : NULL); }

  bool operator !=(const FacadePtr<T,Type::Const> &x) const
    { return ref.pImpl.get() != x.ref.pImpl.get(); }
  bool operator !=(const T *x) const
    { return ref.pImpl.get() != (x ? x->pImpl.get() : NULL); }

  bool operator <(const FacadePtr<T,Type::Const> &x) const
    { return ref.pImpl.get() < x.ref.pImpl.get(); }
  bool operator <(const T *x) const
    { return ref.pImpl.get() < (x ? x->pImpl.get() : NULL); }

  bool operator <=(const FacadePtr<T,Type::Const> &x) const
    { return ref.pImpl.get() <= x.ref.pImpl.get(); }
  bool operator <=(const T *x) const
    { return ref.pImpl.get() <= (x ? x->pImpl.get() : NULL); }

  bool operator >(const FacadePtr<T,Type::Const> &x) const
    { return ref.pImpl.get() > x.ref.pImpl.get(); }
  bool operator >(const T *x) const
    { return ref.pImpl.get() > (x ? x->pImpl.get() : NULL); }

  bool operator >=(const FacadePtr<T,Type::Const> &x) const
    { return ref.pImpl.get() >= x.ref.pImpl.get(); }
  bool operator >=(const T *x) const
    { return ref.pImpl.get() >= (x ? x->pImpl.get() : NULL); }
  
  bool operator ==(const FacadePtr<T,Type::Mutable> &x) const
    { return ref.pImpl.get() == x.ref.pImpl.get(); }

  bool operator !=(const FacadePtr<T,Type::Mutable> &x) const
    { return ref.pImpl.get() != x.ref.pImpl.get(); }

  bool operator <(const FacadePtr<T,Type::Mutable> &x) const
    { return ref.pImpl.get() < x.ref.pImpl.get(); }

  bool operator <=(const FacadePtr<T,Type::Mutable> &x) const
    { return ref.pImpl.get() <= x.ref.pImpl.get(); }

  bool operator >(const FacadePtr<T,Type::Mutable> &x) const
    { return ref.pImpl.get() > x.ref.pImpl.get(); }

  bool operator >=(const FacadePtr<T,Type::Mutable> &x) const
    { return ref.pImpl.get() >= x.ref.pImpl.get(); }
};

template <class Derived, class Impl, class Base = Detail::Storage<Impl>, class SPtr = typename Base::SmartPtr>
class FacadeFoundation: public Base {
  typedef FacadeFoundation<Derived,Impl,Base,SPtr>          this_type;

  template <class TT, template <class> class CC> friend class FacadeRef;
  template <class TT, template <class> class CC> friend class FacadePtr;
protected:
  typedef this_type                                         FFType;
public:
  typedef Impl                                              ImplType;
  typedef SPtr                                              SmartPtr;

  typedef const FacadeRef<Derived, CoSupport::Type::Const>  ConstRef;
  typedef FacadeRef<Derived, CoSupport::Type::Mutable>      Ref;
  typedef FacadePtr<Derived, CoSupport::Type::Const>        ConstPtr;
  typedef FacadePtr<Derived, CoSupport::Type::Mutable>      Ptr;
private:
  // hack for type downward traversal
  typedef FFType                                            _H;
  //
  // Curiously Recurring Template interface.
  //
  ImplType *_impl() const
    { return static_cast<const Derived*>(this)->getImpl(); }
//FIXME: protected:
public:
  // may be overridden in derived class
  ImplType *getImpl() const
    { return this->pImpl.get(); }

  explicit FacadeFoundation(const typename Base::SmartPtr &p)
    : Base(p) {}
public:
  operator ConstRef &() const // do dirty magic
    { return static_cast<ConstRef &>(*this); }
  operator Ref &() // do dirty magic
    { return static_cast<Ref &>(*this); }

  ConstPtr toPtr() const
    { return ConstPtr(static_cast<const Derived *>(this)); }
  Ptr toPtr()
    { return Ptr(static_cast<Derived *>(this)); }
private:
  // default no copy no assign
  FacadeFoundation(const FacadeFoundation &);
  FacadeFoundation &operator =(const FacadeFoundation &);
};

template <class T>
struct FacadeTraits {
  typedef const FacadeRef<T, Type::Const> ConstRef;
  typedef FacadeRef<T, Type::Mutable>     Ref;
  typedef FacadePtr<T, Type::Const>       ConstPtr;
  typedef FacadePtr<T, Type::Mutable>     Ptr;
};

template <class TT, class T, template <class> class C>
const CoSupport::FacadePtr<TT,C> dynamic_pointer_cast(const CoSupport::FacadePtr<T,C> &ptr)
  { return TT::upcast(*ptr); }

template <class TT, class T, template <class> class C>
const CoSupport::FacadePtr<TT,C> static_pointer_cast(const CoSupport::FacadePtr<T,C> &ptr)
  { return &static_cast<typename C<TT>::type &>(*ptr); }

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
