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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_FACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_FACADE_HPP

#include <boost/intrusive_ptr.hpp>

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>

#include "../commondefs.h"

namespace CoSupport { namespace DataTypes {

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

  class FacadePtrNullClass {};
} // namespace Detail

template <class T, template <class> class C> class FacadeRef;
template <class T, template <class> class C> class FacadePtr;

template <class T>
struct FacadeTraits {
  typedef const FacadeRef<T, boost::add_const> ConstRef;
  typedef FacadeRef<T, boost::remove_const>    Ref;
  typedef FacadePtr<T, boost::add_const>       ConstPtr;
  typedef FacadePtr<T, boost::remove_const>    Ptr;
};

template <class T>
struct FacadeTraits<const T> {
  typedef typename FacadeTraits<T>::ConstRef   ConstRef;
  typedef typename FacadeTraits<T>::ConstRef   Ref;
  typedef typename FacadeTraits<T>::ConstPtr   ConstPtr;
  typedef typename FacadeTraits<T>::ConstPtr   Ptr;
};

template <class T, template <class> class C>
class FacadeRef: public T {
  typedef FacadeRef<T, C> this_type;

  template <class TT, template <class> class CC> friend class FacadeRef;
  template <class TT, template <class> class CC> friend class FacadePtr;
public:
  typedef typename T::_H::ImplType  ImplType;
  typedef typename T::_H::SmartPtr  SmartPtr;
public:
  FacadeRef(const SmartPtr &p)
    : T(p) {}
  FacadeRef(const this_type &t)
    : T(SmartPtr()) { this->assign(t); }

  // bounce assign to base class
  using T::operator =;
//template <typename TT>
//this_type &operator =(const TT &obj)
//  { return static_cast<this_type &>(T::operator =(obj)); }
};

template <class T, template <class> class C>
class FacadePtr: public FacadePtr<typename T::_B, C> {
  typedef FacadePtr<T,C>                this_type;
  typedef FacadePtr<typename T::_B, C>  base_type;

  template <class TT, template <class> class CC> friend class FacadePtr;
private:
  typedef typename C<T>::type                 value_type;
  typedef typename FacadeTraits<T>::ConstRef  ConstRef;
  typedef typename FacadeTraits<T>::Ref       Ref;
  typedef typename FacadeTraits<T>::ConstPtr  ConstPtr;
  typedef typename FacadeTraits<T>::Ptr       Ptr;
  typedef typename T::_H::ImplType            ImplType;
  typedef typename T::_H::SmartPtr            SmartPtr;
protected:
  typedef typename base_type::FPN             FPN;
public:
//FIXME: protected:
  // FIXME: This is an ugly hack !!!
  ImplType *getImpl() const {
    return reinterpret_cast<ConstRef &>(*this).getImpl();
  }
protected:
  // FIXME: This is an ugly hack !!!
  void assign(const value_type &p) {
    reinterpret_cast<Ref &>(*this).assign(p);
  }
public:
//using base_type::operator <=;

  FacadePtr(const SmartPtr &p)
    : base_type(NULL) { this->assign(Ref(p)); }
  FacadePtr(const Ptr &t)
    : base_type(NULL) { this->assign(*t); }
  FacadePtr(FPN *null = NULL)
    : base_type(NULL) { assert(null == NULL); }

  value_type &operator *() const {
    return const_cast<value_type &>
      (reinterpret_cast<const T &>(*this));
  }
  value_type *operator ->() const {
    return const_cast<value_type *>
      (reinterpret_cast<const T *>(this));
  }

  this_type &operator =(const this_type &x)
    { this->assign(*x); return *this; }

};

template <typename Impl, template <class> class C>
class FacadePtr<Detail::Storage<Impl>, C>: public Detail::Storage<Impl> {
  typedef FacadePtr<Detail::Storage<Impl>, C> this_type;
  typedef Detail::Storage<Impl>               base_type;

  template <typename II, template <class> class CC> friend class FacadePtr;
private:
  typedef typename FacadeTraits<Detail::Storage<Impl> >::ConstPtr ConstPtr;
  typedef typename FacadeTraits<Detail::Storage<Impl> >::Ptr      Ptr;
protected:
  typedef Detail::FacadePtrNullClass FPN;
protected:
  typename this_type::ImplType *getImpl() const
    { return this->pImpl.get(); }
public:
  typedef void (base_type::*unspecified_bool_type)(const base_type &);
protected:
  FacadePtr(const typename this_type::SmartPtr &p)
    : base_type(p) {}
public:

  // "operator bool()" would be utilized in comparisons if the
  // comp. operator is not specified (e.g. if "operator!="
  // is not defined, (a != b) results always [assuming pImpl
  // is valid] in (true != true), which is WRONG!!!)
  // -> define all comparison operators
  operator unspecified_bool_type() const {
    return this->pImpl.get() != NULL
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }

  unspecified_bool_type operator ==(const Ptr &x) const {
    return this->pImpl.get() == x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
  unspecified_bool_type operator ==(const ConstPtr &x) const {
    return this->pImpl.get() == x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
  unspecified_bool_type operator ==(FPN *null) const {
    assert(null == NULL); return !*this
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }

  unspecified_bool_type operator !=(const Ptr &x) const {
    return this->pImpl.get() != x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
  unspecified_bool_type operator !=(const ConstPtr &x) const {
    return this->pImpl.get() != x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
  unspecified_bool_type operator !=(FPN *null) const {
    assert(null == NULL); return *this
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }

  unspecified_bool_type operator > (const Ptr &x) const {
    return this->pImpl.get() >  x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
  unspecified_bool_type operator > (const ConstPtr &x) const {
    return this->pImpl.get() >  x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }

  unspecified_bool_type operator < (const Ptr &x) const {
    return this->pImpl.get() <  x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
  unspecified_bool_type operator < (const ConstPtr &x) const {
    return this->pImpl.get() <  x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }

  unspecified_bool_type operator >=(const Ptr &x) const {
    return this->pImpl.get() >= x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
  unspecified_bool_type operator >=(const ConstPtr &x) const {
    return this->pImpl.get() >= x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }

  unspecified_bool_type operator <=(const Ptr &x) const {
    return this->pImpl.get() <= x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
  unspecified_bool_type operator <=(const ConstPtr &x) const {
    return this->pImpl.get() <= x.getImpl()
      ? static_cast<unspecified_bool_type>(&this_type::assign)
      : NULL;
  }
};

template <class Derived, class Impl, class Base = Detail::Storage<Impl>, class SPtr = typename Base::SmartPtr>
class FacadeFoundation: public Base {
  typedef FacadeFoundation<Derived,Impl,Base,SPtr>  this_type;
  // Hack for type downward traversal as FFType may be redefined in Derived!
  typedef this_type _H;

  template <class TT, template <class> class CC> friend class FacadeRef;
  template <class TT, template <class> class CC> friend class FacadePtr;
protected:
  typedef this_type FFType;
public:
  typedef Base      _B; // Why does this need to be public, FacadePtr is a friend!

  typedef Impl      ImplType;
  typedef SPtr      SmartPtr;

  // CoSupport::DataTypes:: is here to ameliorate doxygen C++ parsing results
  typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstRef  ConstRef;
  typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ref       Ref;
  typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstPtr  ConstPtr;
  typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ptr       Ptr;
private:
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

  explicit FacadeFoundation(const typename _B::SmartPtr &p)
    : _B(p) {}
public:
  operator ConstRef &() const // do dirty magic
    { return static_cast<ConstRef &>(*this); }
  operator Ref &() // do dirty magic
    { return static_cast<Ref &>(*this); }

  ConstPtr toPtr() const
    { return ConstPtr(_impl()); }
  Ptr toPtr()
    { return Ptr(_impl()); }

  ConstPtr operator &() const
    { return ConstPtr(_impl()); }
  Ptr operator &()
    { return Ptr(_impl()); }
private:
  // default no copy no assign
  FacadeFoundation(const this_type &);
  FacadeFoundation &operator =(const this_type &);
};

template <class TT, class T, template <class> class C>
COSUPPORT_ATTRIBUTE_DEPRECATED
const FacadePtr<TT,C> dynamic_pointer_cast(const FacadePtr<T,C> &ptr)
  { return TT::upcast(*ptr); }

template <class TT, class T, template <class> class C>
COSUPPORT_ATTRIBUTE_DEPRECATED
const FacadePtr<TT,C> static_pointer_cast(const FacadePtr<T,C> &ptr)
  { return &static_cast<typename C<TT>::type &>(*ptr); }

} } // namespace CoSupport::DataTypes

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>

namespace boost {

  template <class Derived, class Impl, class Base, class SPtr>
  struct add_reference<CoSupport::DataTypes::FacadeFoundation<Derived, Impl, Base, SPtr> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ref type; };

  template <class Derived, class Impl, class Base, class SPtr>
  struct add_reference<const CoSupport::DataTypes::FacadeFoundation<Derived, Impl, Base, SPtr> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstRef type; };

  template <class Derived, template <class> class C>
  struct remove_reference<CoSupport::DataTypes::FacadeRef<Derived, C> >
    { typedef typename C<Derived>::type type; };

  template <class Derived, class Impl, class Base, class SPtr>
  struct add_pointer<CoSupport::DataTypes::FacadeFoundation<Derived, Impl, Base, SPtr> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ptr type; };

  template <class Derived, class Impl, class Base, class SPtr>
  struct add_pointer<const CoSupport::DataTypes::FacadeFoundation<Derived, Impl, Base, SPtr> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstPtr type; };

  template <class Derived, template <class> class C>
  struct remove_pointer<CoSupport::DataTypes::FacadePtr<Derived, C> >
    { typedef typename C<Derived>::type type; };

  template <class TT, class T, template <class> class C>
  const CoSupport::DataTypes::FacadePtr<TT,C> dynamic_pointer_cast(
      const CoSupport::DataTypes::FacadePtr<T,C> &ptr)
    { return TT::upcast(*ptr); }

  template <class TT, class T, template <class> class C>
  const CoSupport::DataTypes::FacadePtr<TT,C> static_pointer_cast(
      const CoSupport::DataTypes::FacadePtr<T,C> &ptr)
    { return &static_cast<typename C<TT>::type &>(*ptr); }

} // namespace boost

#endif // _INCLUDED_COSUPPORT_DATATYPES_FACADE_HPP
