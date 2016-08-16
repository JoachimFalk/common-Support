/*
 * Copyright (c) 2004-2014 Hardware-Software-CoDesign, University of
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

#include "../compatibility-glue/nullptr.h"
#include "../commondefs.h"

#include <boost/intrusive_ptr.hpp>

#include "ValueInterface.hpp"

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/utility/enable_if.hpp>

namespace CoSupport { namespace DataTypes {

template <class T, template <class> class C>     class FacadeRef;
template <class T, template <class> class C>     class FacadePtr;
template <class Derived, class Impl, class Base> class FacadeFoundation;

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

namespace Detail {

  template <class Impl> class Storage;

} // namespace Detail

class FacadeCoreAccess {
public:
  template <class Impl>
  static
  Impl                               *getImplBase(Detail::Storage<Impl> const &storage);

  template <class T, template <class> class C>
  static
  typename T::_StorageType::ImplType *getImplBase(FacadePtr<T,C> const &ptr);

  template <class T>
  static
  typename T::FFType::ImplType *getImpl(T const &facade)
    { return static_cast<typename T::FFType::ImplType *>(getImplBase(facade)); }

  template <class T, template <class> class C>
  static
  typename T::FFType::ImplType *getImpl(FacadePtr<T,C> const &ptr)
    { return static_cast<typename T::FFType::ImplType *>(getImplBase(ptr)); }
};

namespace Detail {

  template <class Impl>
  class Storage {
    typedef Storage<Impl>                     this_type;

//  template <class T, template <class> class C> friend class FacadeRef;
    template <class T, template <class> class C> friend class FacadePtr;
    template <class IImpl>                       friend IImpl *FacadeCoreAccess::getImplBase(Storage<IImpl> const &);
  public:
    template <class T>
    struct _StoragePtrKind { typedef ::boost::intrusive_ptr<T> type; };

    typedef Impl                              ImplType;
  public:
    typedef this_type                                   _StorageType;
    typedef typename _StoragePtrKind<ImplType>::type    _StoragePtr;
  private:
    _StoragePtr pImpl; //< this is the storage smart ptr, its name must be pImpl
  protected:
    Storage() {}
    Storage(const this_type &v)
      : pImpl(v.pImpl) {}
  public:
    Storage(const _StoragePtr &p)
      : pImpl(p) {}
    Storage(ImplType *p)
      : pImpl(p) {}

//  void assign(const this_type &x)
//    { pImpl = x.pImpl; }
//  explicit Storage(const _StoragePtr &p)
//    : pImpl(p) {}
//protected:
//  ImplType *getImpl() const
//    { return pImpl.get(); }
  };

} // namespace Detail

template <class Impl>
Impl *FacadeCoreAccess::getImplBase(Detail::Storage<Impl> const &storage)
  { return storage.pImpl.get(); }

template <class T, template <class> class C>
class FacadePtr {
  typedef FacadePtr<T, C>      this_type;
  typedef typename C<T>::type  value_type;
  typedef value_type          &deref_type;

  template <class TT, template <class> class CC> friend class FacadePtr;
  template <class TT, template <class> class CC> friend typename TT::_StorageType::ImplType *FacadeCoreAccess::getImplBase(FacadePtr<TT,CC> const &);

  typedef deref_type (this_type::*unspecified_bool_type)() const;
private:
  typename T::_StorageType storage;
public:
  FacadePtr(typename T::SmartPtr const &p)
    : storage(typename FacadeTraits<T>::Ref(p)) {}

  FacadePtr(typename C<typename T::FFType>::type *ptr = nullptr)
    : storage(ptr == nullptr
        ? typename T::_StorageType()
        : static_cast<typename T::_StorageType const &>(*ptr)) {}

  FacadePtr(this_type const &ptr)
    : storage(ptr.storage) {}
  template <typename TT, template <class> class CC>
  FacadePtr(FacadePtr<TT, CC> const &ptr,
        // This is a conversion check from CC<TT>::type * to C<T>::type *
        typename C<T>::type *dummy = static_cast<typename CC<TT>::type *>(nullptr))
    : storage(ptr.storage) {}
  template <class DD, typename TT, typename CRCR>
  FacadePtr(ValueInterface<DD,TT,CRCR> const &value,
      typename boost::enable_if<boost::is_convertible<
        TT,
        this_type>, void
      >::type *dummy = nullptr)
    : storage(this_type(value.get()).storage) {}

  deref_type  operator *() const {
    return static_cast<value_type &>
      (const_cast<this_type *>(this)->storage);
  }
  value_type *operator ->() const {
    return &static_cast<value_type &>
      (const_cast<this_type *>(this)->storage);
  }

  // "operator bool()" would be utilized in comparisons if the
  // comp. operator is not specified (e.g. if "operator!="
  // is not defined, (a != b) results always [assuming pImpl
  // is valid] in (true != true), which is WRONG!!!)
  // -> define all comparison operators
  operator unspecified_bool_type() const {
    return storage.pImpl
      ? static_cast<unspecified_bool_type>(&this_type::operator *)
      : nullptr;
  }

  this_type &operator =(const this_type &rhs)
    { storage.pImpl = rhs.storage.pImpl; return *this; }
};

template <class T, template <class> class C>
typename T::_StorageType::ImplType *FacadeCoreAccess::getImplBase(FacadePtr<T,C> const &ptr)
  { return FacadeCoreAccess::getImplBase(ptr.storage); }

template <class T1, template <class> class C1, class T2, template <class> class C2>
bool operator ==(
    FacadePtr<T1,C1> const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) == FacadeCoreAccess::getImplBase(rhs); }
//template <class T2, template <class> class C2>
//bool operator ==(
//    typename T2::_StorageType const *lhs,
//    FacadePtr<T2,C2> const &rhs)
//  { assert(lhs); return lhs->pImpl.get() == FacadeCoreAccess::getImplBase(rhs); }
//template <class T1, template <class> class C1>
//bool operator ==(
//    FacadePtr<T1,C1> const &lhs,
//    typename T1::_StorageType const *rhs)
//  { assert(rhs); return FacadeCoreAccess::getImplBase(lhs) == rhs->pImpl.get(); }
template <class T2, template <class> class C2>
bool operator ==(
    typename T2::SmartPtr const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return lhs.get() == FacadeCoreAccess::getImplBase(rhs); }
template <class T1, template <class> class C1>
bool operator ==(
    FacadePtr<T1,C1> const &lhs,
    typename T1::SmartPtr const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) == rhs.get(); }
template <class T2, template <class> class C2>
bool operator ==(
    std::nullptr_t null,
    FacadePtr<T2,C2> const &rhs)
  { assert(null == nullptr); return !rhs; }
template <class T1, template <class> class C1>
bool operator ==(
    FacadePtr<T1,C1> const &lhs,
    std::nullptr_t null)
  { assert(null == nullptr); return !lhs; }

template <class T1, template <class> class C1, class T2, template <class> class C2>
bool operator !=(
    FacadePtr<T1,C1> const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) != FacadeCoreAccess::getImplBase(rhs); }
//template <class T2, template <class> class C2>
//bool operator !=(
//    typename T2::_StorageType const *lhs,
//    FacadePtr<T2,C2> const &rhs)
//  { assert(lhs); return lhs->pImpl.get() != FacadeCoreAccess::getImplBase(rhs); }
//template <class T1, template <class> class C1>
//bool operator !=(
//    FacadePtr<T1,C1> const &lhs,
//    typename T1::_StorageType const *rhs)
//  { assert(rhs); return FacadeCoreAccess::getImplBase(lhs) == rhs->pImpl.get(); }
template <class T2, template <class> class C2>
bool operator !=(
    typename T2::SmartPtr const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return lhs.get() != FacadeCoreAccess::getImplBase(rhs); }
template <class T1, template <class> class C1>
bool operator !=(
    FacadePtr<T1,C1> const &lhs,
    typename T1::SmartPtr const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) != rhs.get(); }
template <class T2, template <class> class C2>
bool operator !=(
    std::nullptr_t null,
    FacadePtr<T2,C2> const &rhs)
  { assert(null == nullptr); return rhs; }
template <class T1, template <class> class C1>
bool operator !=(
    FacadePtr<T1,C1> const &lhs,
    std::nullptr_t null)
  { assert(null == nullptr); return lhs; }

template <class T1, template <class> class C1, class T2, template <class> class C2>
bool operator <=(
    FacadePtr<T1,C1> const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) <= FacadeCoreAccess::getImplBase(rhs); }
//template <class T2, template <class> class C2>
//bool operator <=(
//    typename T2::_StorageType const *lhs,
//    FacadePtr<T2,C2> const &rhs)
//  { assert(lhs); return lhs->pImpl.get() <= FacadeCoreAccess::getImplBase(rhs); }
//template <class T1, template <class> class C1>
//bool operator <=(
//    FacadePtr<T1,C1> const &lhs,
//    typename T1::_StorageType const *rhs)
//  { assert(rhs); return FacadeCoreAccess::getImplBase(lhs) <= rhs->pImpl.get(); }
template <class T2, template <class> class C2>
bool operator <=(
    typename T2::SmartPtr const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return lhs.get() <= FacadeCoreAccess::getImplBase(rhs); }
template <class T1, template <class> class C1>
bool operator <=(
    FacadePtr<T1,C1> const &lhs,
    typename T1::SmartPtr const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) <= rhs.get(); }

template <class T1, template <class> class C1, class T2, template <class> class C2>
bool operator >=(
    FacadePtr<T1,C1> const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) >= FacadeCoreAccess::getImplBase(rhs); }
//template <class T2, template <class> class C2>
//bool operator >=(
//    typename T2::_StorageType const *lhs,
//    FacadePtr<T2,C2> const &rhs)
//  { assert(lhs); return lhs->pImpl.get() >= FacadeCoreAccess::getImplBase(rhs); }
//template <class T1, template <class> class C1>
//bool operator >=(
//    FacadePtr<T1,C1> const &lhs,
//    typename T1::_StorageType const *rhs)
//  { assert(rhs); return FacadeCoreAccess::getImplBase(lhs) >= rhs->pImpl.get(); }
template <class T2, template <class> class C2>
bool operator >=(
    typename T2::SmartPtr const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return lhs.get() >= FacadeCoreAccess::getImplBase(rhs); }
template <class T1, template <class> class C1>
bool operator >=(
    FacadePtr<T1,C1> const &lhs,
    typename T1::SmartPtr const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) >= rhs.get(); }

template <class T1, template <class> class C1, class T2, template <class> class C2>
bool operator < (
    FacadePtr<T1,C1> const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) <  FacadeCoreAccess::getImplBase(rhs); }
//template <class T2, template <class> class C2>
//bool operator < (
//    typename T2::_StorageType const *lhs,
//    FacadePtr<T2,C2> const &rhs)
//  { assert(lhs); return lhs->pImpl.get() <  FacadeCoreAccess::getImplBase(rhs); }
//template <class T1, template <class> class C1>
//bool operator < (
//    FacadePtr<T1,C1> const &lhs,
//    typename T1::_StorageType const *rhs)
//  { assert(rhs); return FacadeCoreAccess::getImplBase(lhs) <  rhs->pImpl.get(); }
template <class T2, template <class> class C2>
bool operator < (
    typename T2::SmartPtr const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return lhs.get() <  FacadeCoreAccess::getImplBase(rhs); }
template <class T1, template <class> class C1>
bool operator < (
    FacadePtr<T1,C1> const &lhs,
    typename T1::SmartPtr const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) <  rhs.get(); }

template <class T1, template <class> class C1, class T2, template <class> class C2>
bool operator > (
    FacadePtr<T1,C1> const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) >  FacadeCoreAccess::getImplBase(rhs); }
//template <class T2, template <class> class C2>
//bool operator > (
//    typename T2::_StorageType const *lhs,
//    FacadePtr<T2,C2> const &rhs)
//  { assert(lhs); return lhs->pImpl.get() >  FacadeCoreAccess::getImplBase(rhs); }
//template <class T1, template <class> class C1>
//bool operator > (
//    FacadePtr<T1,C1> const &lhs,
//    typename T1::_StorageType const *rhs)
//  { assert(rhs); return FacadeCoreAccess::getImplBase(lhs) >  rhs->pImpl.get(); }
template <class T2, template <class> class C2>
bool operator > (
    typename T2::SmartPtr const &lhs,
    FacadePtr<T2,C2> const &rhs)
  { return lhs.get() >  FacadeCoreAccess::getImplBase(rhs); }
template <class T1, template <class> class C1>
bool operator > (
    FacadePtr<T1,C1> const &lhs,
    typename T1::SmartPtr const &rhs)
  { return FacadeCoreAccess::getImplBase(lhs) >  rhs.get(); }

namespace Detail {
  
  struct value_type_facade_pointer_t
    : public value_type_pointer_tag_t {};

  template <class T, template <class> class C>
  struct ValueTypeClassifier<FacadePtr<T, C> >
    { typedef value_type_facade_pointer_t tag; };

  template <class D, class T, template <class> class C>
  class ValueTypeDecorator<value_type_facade_pointer_t, D, FacadePtr<T, C>, FacadePtr<T, C> const> {
    typedef ValueTypeDecorator<value_type_facade_pointer_t, D, FacadePtr<T, C>, FacadePtr<T, C> const> this_type;
  private:
    typedef FacadePtr<T, C> value_type;
    typedef FacadeRef<T, C> deref_type;
  protected:
    typedef D *(this_type::*unspecified_bool_type)();
  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
    value_type operator ->() const
      { return getDerived()->get(); }
    deref_type operator *() const
      { return *getDerived()->get(); }

    operator unspecified_bool_type() const {
      return getDerived()->get()
        ? static_cast<unspecified_bool_type>(&this_type::getDerived)
        : static_cast<unspecified_bool_type>(0);
    }
  };

  template <class T, template <class> class C, class F>
  class FacadeProxyPtr
  : public ValueInterface<
      FacadeProxyPtr<T,C,F>,
      FacadePtr<T,C>,
      FacadePtr<T,C> const>
  {
    typedef FacadeProxyPtr<T,C,F> this_type;
    typedef FacadePtr<T,C>        value_type;

    friend class ValueInterface<this_type, value_type, value_type const>;
  private:
    F *ptr;

    // Don't implement this! FacadeProxyPtr is not an lvalue!
    //void implSet(const value_type &val) {...}

    value_type const implGet() const
      { return ptr->toPtr(); }
  public:
    FacadeProxyPtr(F *ptr): ptr(ptr) {}

    // We need this operator due to ``operator F *() const''
    operator bool() const
      { return ptr && ptr->toPtr(); }

    operator F *() const
      { return ptr; }
  };

} // namespace Detail

template <class T, template <class> class C>
class FacadeRef: public T {
  typedef FacadeRef<T, C> this_type;
  typedef T               base_type;

  template <class TT, template <class> class CC> friend class FacadeRef;
  template <class TT, template <class> class CC> friend class FacadePtr;
public:
  FacadeRef(typename this_type::SmartPtr const &p)
    : base_type(p) {}
  FacadeRef(this_type const &t)
    : base_type(static_cast<typename this_type::_StorageType const &>(t)) {}

  // bounce assign to base class
  using T::operator =;

  Detail::FacadeProxyPtr<T, boost::add_const, const this_type> operator &() const
    { return Detail::FacadeProxyPtr<T, boost::add_const, const this_type>(this); }
  Detail::FacadeProxyPtr<T, boost::remove_const, this_type> operator &()
    { return Detail::FacadeProxyPtr<T, boost::remove_const, this_type>(this); }
};

template <class Derived, class Impl, class Base = Detail::Storage<Impl> >
class FacadeFoundation: public Base {
  typedef FacadeFoundation<Derived,Impl,Base> this_type;
  typedef Base                                base_type;

  template <class TT, template <class> class CC> friend class FacadeRef;
  template <class TT, template <class> class CC> friend class FacadePtr;
  friend class FacadeCoreAccess;
protected:
  typedef this_type FFType;
public:
  // FIXME: Make this protected again
  typedef Impl                                                ImplType;
  typedef typename Base::template _StoragePtrKind<Impl>::type SmartPtr;

  // CoSupport::DataTypes:: is here to ameliorate doxygen C++ parsing results
  typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstRef  ConstRef;
  typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ref       Ref;
  typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstPtr  ConstPtr;
  typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ptr       Ptr;
//private:
////
//// Curiously Recurring Template interface.
////
//ImplType *_impl() const
//  { return static_cast<const Derived*>(this)->getImpl(); }
protected:
//// may be overridden in derived class
//ImplType *getImpl() const
//  { return static_cast<ImplType *>(this->pImpl.get()); }

  FacadeFoundation(typename this_type::_StorageType const &x)
    : base_type(x) {}
//// Constructor converting the SmartPtr of the derived implementation into the StoragePtr.
//// Note that this constructor may not be implementable if the type hierarchy of the
//// implementation classes is not available. In this case, the derived class must implement
//// a constructor in a .cpp file where this information is available and use constructor
//// given above for its base class.
//explicit FacadeFoundation(typename this_type::SmartPtr const &p)
//  : base_type(typename this_type::_StorageType(p)) {}
public:
  operator ConstRef &() const // do dirty magic
    { return static_cast<ConstRef &>(*this); }
  operator Ref &() // do dirty magic
    { return static_cast<Ref &>(*this); }

  ConstPtr toPtr() const
    { return ConstPtr(this); }
  Ptr toPtr()
    { return Ptr(this); }

  Detail::FacadeProxyPtr<Derived, boost::add_const, const Derived> operator &() const
    { return Detail::FacadeProxyPtr<Derived, boost::add_const, const Derived>(static_cast<const Derived *>(this)); }
  Detail::FacadeProxyPtr<Derived, boost::remove_const, Derived> operator &()
    { return Detail::FacadeProxyPtr<Derived, boost::remove_const, Derived>(static_cast<Derived *>(this)); }
private:
  // default no copy no assign
  FacadeFoundation(const this_type &);
  FacadeFoundation &operator =(const this_type &);
};

} } // namespace CoSupport::DataTypes

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>

namespace boost {

//typedef const FacadeRef<T, boost::add_const> ConstRef;
//typedef FacadeRef<T, boost::remove_const>    Ref;
//typedef FacadePtr<T, boost::add_const>       ConstPtr;
//typedef FacadePtr<T, boost::remove_const>    Ptr;

  template <class Derived, template <class> class C>
  struct add_const<CoSupport::DataTypes::FacadeRef<Derived, C> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstRef type; };

  template <class Derived, template <class> class C>
  struct add_const<CoSupport::DataTypes::FacadePtr<Derived, C> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstPtr type; };

  template <class Derived, template <class> class C>
  struct remove_const<const CoSupport::DataTypes::FacadeRef<Derived, C> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ref type; };

  template <class Derived, template <class> class C>
  struct remove_const<CoSupport::DataTypes::FacadePtr<Derived, C> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ptr type; };

  // FIXME: This still does not work for add_reference<Derived>::type
  template <class Derived, class Impl, class Base>
  struct add_reference<CoSupport::DataTypes::FacadeFoundation<Derived, Impl, Base> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ref type; };

  // FIXME: This still does not work for add_reference<Derived const>::type
  template <class Derived, class Impl, class Base>
  struct add_reference<const CoSupport::DataTypes::FacadeFoundation<Derived, Impl, Base> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::ConstRef type; };

  template <class Derived, template <class> class C>
  struct remove_reference<CoSupport::DataTypes::FacadeRef<Derived, C> >
    { typedef typename C<Derived>::type type; };

  template <class Derived, template <class> class C>
  struct remove_reference<const CoSupport::DataTypes::FacadeRef<Derived, C> >
    { typedef typename C<Derived>::type type; };

  // FIXME: This still does not work for add_pointer<Derived>::type
  template <class Derived, class Impl, class Base>
  struct add_pointer<CoSupport::DataTypes::FacadeFoundation<Derived, Impl, Base> >
    { typedef typename CoSupport::DataTypes::FacadeTraits<Derived>::Ptr type; };

  // FIXME: This still does not work for add_pointer<Derived const>::type
  template <class Derived, class Impl, class Base>
  struct add_pointer<const CoSupport::DataTypes::FacadeFoundation<Derived, Impl, Base> >
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

//template <class TT, class T, template <class> class C, class F>
//const CoSupport::DataTypes::FacadePtr<TT,C> dynamic_pointer_cast(
//    const CoSupport::DataTypes::Detail::FacadeProxyPtr<T,C,F> &ptr)
//  { return TT::upcast(*ptr); }

//template <class TT, class T, template <class> class C, class F>
//const CoSupport::DataTypes::FacadePtr<TT,C> static_pointer_cast(
//    const CoSupport::DataTypes::Detail::FacadeProxyPtr<T,C,F> &ptr)
//  { return &static_cast<typename C<TT>::type &>(*ptr); }

  template <typename TT, typename D, typename T, template <class> class C>
  const CoSupport::DataTypes::FacadePtr<TT, C> dynamic_pointer_cast(
      CoSupport::DataTypes::ValueInterface<D,
        CoSupport::DataTypes::FacadePtr<T,C>,
        CoSupport::DataTypes::FacadePtr<T,C> const> const &ptr)
    { return dynamic_pointer_cast<TT>(static_cast<CoSupport::DataTypes::FacadePtr<T, C> >(ptr)); }

  template <typename TT, typename D, typename T, template <class> class C>
  const CoSupport::DataTypes::FacadePtr<TT, C> static_pointer_cast(
      CoSupport::DataTypes::ValueInterface<D,
        CoSupport::DataTypes::FacadePtr<T,C>,
        CoSupport::DataTypes::FacadePtr<T,C> const> const &ptr)
    { return static_pointer_cast<TT>(static_cast<CoSupport::DataTypes::FacadePtr<T, C> >(ptr)); }

} // namespace boost

#endif // _INCLUDED_COSUPPORT_DATATYPES_FACADE_HPP
