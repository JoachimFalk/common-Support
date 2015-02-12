//  -*- tab-width:8; intent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 expandtab:
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VALUEINTERFACE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VALUEINTERFACE_HPP

#include <ostream>

#include "../compatibility-glue/nullptr.h"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/typeof/typeof.hpp>

#include <boost/function_types/result_type.hpp>

#include <boost/static_assert.hpp>

namespace boost {

  template<class T> class intrusive_ptr;
  template<class T> class shared_ptr;

} // namespace boost

namespace CoSupport { namespace DataTypes {

namespace Detail {

  class ValueInterfaceTag {};

  struct value_type_unknown_tag_t {};

  template <typename T>
  struct ValueTypeClassifier { typedef value_type_unknown_tag_t tag; };

  template <typename tag, class D, typename T, typename CR>
  class ValueTypeDecorator;

  template <class D, typename T, typename CR>
  class ValueTypeDecorator<value_type_unknown_tag_t, D, T, CR> {};

#define COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(TYPE,TAG) \
  template <> struct ValueTypeClassifier<TYPE> { typedef TAG tag; }

  struct value_type_numeric_tag_t {};

  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(int,value_type_numeric_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(long,value_type_numeric_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(long long,value_type_numeric_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(unsigned int,value_type_numeric_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(unsigned long,value_type_numeric_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(unsigned long long,value_type_numeric_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(float,value_type_numeric_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(double,value_type_numeric_tag_t);

  template <class D, typename T, typename CR>
  class ValueTypeDecorator<value_type_numeric_tag_t, D, T, CR> {
    typedef ValueTypeDecorator<value_type_numeric_tag_t, D, T, CR> this_type;
  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
    D &operator +=(T const &x)
      { getDerived()->set(getDerived()->get() + x); return *getDerived(); }
    D &operator -=(T const &x)
      { getDerived()->set(getDerived()->get() - x); return *getDerived(); }
    D &operator *=(T const &x)
      { getDerived()->set(getDerived()->get() * x); return *getDerived(); }
    D &operator /=(T const &x)
      { getDerived()->set(getDerived()->get() / x); return *getDerived(); }
    D &operator %=(T const &x)
      { getDerived()->set(getDerived()->get() % x); return *getDerived(); }
    D &operator ++()
      { return *this += 1; }
    D &operator --()
      { return *this -= 1; }
    T operator ++(int) {
      T retval(getDerived()->get());
      getDerived()->set(retval + 1);
      return retval;
    }
    T operator --(int) {
      T retval(getDerived()->get());
      getDerived()->set(retval - 1);
      return retval;
    }
  };

  struct value_type_pointer_tag_t {};
/*
  struct value_type_charptr_tag_t
    : public value_type_pointer_tag_t {};

  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(char *,value_type_charptr_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(const char *,value_type_charptr_tag_t);

  template <class D, typename T, typename CR>
  class ValueTypeDecorator<value_type_charptr_tag_t, D, T, CR> {
    typedef ValueTypeDecorator<value_type_charptr_tag_t, D, T, CR> this_type;

  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
  };

  struct value_type_std_string_tag_t {};

  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(std::string,value_type_std_string_tag_t);

  template <class D, typename T, typename CR>
  class ValueTypeDecorator<value_type_std_string_tag_t, D, T, CR> {
    typedef ValueTypeDecorator<value_type_std_string_tag_t, D, T, CR> this_type;
  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
  };
 */

  struct value_type_plain_pointer_t
    : public value_type_pointer_tag_t {};

  template <typename T>
  struct ValueTypeClassifier<T *> { typedef value_type_plain_pointer_t tag; };

  template <class D, typename T, typename CR>
  class ValueTypeDecorator<value_type_plain_pointer_t, D, T, CR> {
    typedef ValueTypeDecorator<value_type_plain_pointer_t, D, T, CR> this_type;
  private:
    typedef T                                                 value_type;
    typedef typename boost::add_reference<
      typename boost::remove_pointer<value_type>::type>::type deref_type;
  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
    D &operator +=(ptrdiff_t x)
      { getDerived()->set(getDerived()->get() + x); return *getDerived(); }
    D &operator -=(ptrdiff_t x)
      { getDerived()->set(getDerived()->get() - x); return *getDerived(); }
    D &operator ++()
      { return *this += 1; }
    D &operator --()
      { return *this -= 1; }
    T operator ++(int) {
      T retval(getDerived()->get());
      getDerived()->set(retval + 1);
      return retval;
    }
    T operator --(int) {
      T retval(getDerived()->get());
      getDerived()->set(retval - 1);
      return retval;
    }

    deref_type operator [](ptrdiff_t x) const
      { return getDerived()->get()[x]; }

    value_type operator ->() const
      { return getDerived()->get(); }
    deref_type operator *() const
      { return *getDerived()->get(); }
  };

  struct value_type_smart_pointer_t
    : public value_type_pointer_tag_t {};

  template <typename T>
  struct ValueTypeClassifier<boost::shared_ptr<T> >
    { typedef value_type_smart_pointer_t tag; };
  template <typename T>
  struct ValueTypeClassifier<boost::intrusive_ptr<T> >
    { typedef value_type_smart_pointer_t tag; };

  template <class D, typename T, typename CR>
  class ValueTypeDecorator<value_type_smart_pointer_t, D, T, CR> {
    typedef ValueTypeDecorator<value_type_smart_pointer_t, D, T, CR> this_type;
  private:
    typedef T                                                 value_type;
    typedef typename boost::add_reference<
      typename boost::remove_pointer<value_type>::type>::type deref_type;
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

#undef COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION

} // namespace Detail

/// This class represents the interface for a storage which contains a value of type T.
/// \example test_value.cpp
template <
  class D,
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class ValueInterface
: public Detail::ValueTypeDecorator<
    typename Detail::ValueTypeClassifier<T>::tag, D, T, CR
  >,
  public Detail::ValueInterfaceTag {
  typedef ValueInterface<D,T,CR> this_type;
protected:
  D       *getDerived()
    { return static_cast<D *>(this); }

  D const *getDerived() const
    { return static_cast<D const *>(this); }
public:
  template <class DD, typename TT, typename CRCR>
  D &operator = (const ValueInterface<DD, TT, CRCR> &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const this_type &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const T &val)
    { this->set(val); return *getDerived(); }

  operator CR() const
    { return this->get(); }

  template <class DD, typename TT, typename CRCR>
  void set(const ValueInterface<DD,TT,CRCR> &val)
    { this->set(val.get()); }
  // implSet is an interface method which must be implemented in D!
  void set(const T &val) {
    BOOST_STATIC_ASSERT((
      boost::is_same<
          typename boost::function_types::result_type<BOOST_TYPEOF(&D::implSet)>::type,
          void>
        ::value));
    getDerived()->implSet(val);
  }
  // implGet is an interface method which must be implemented in D!
  CR get() const { // this may throw
    BOOST_STATIC_ASSERT((
      boost::is_reference<CR>
        ::value ==
      boost::is_reference<
          typename boost::function_types::result_type<BOOST_TYPEOF(&D::implGet)>::type>
        ::value));
    return getDerived()->implGet();
  }

};

template <class DD, typename TT, typename CRCR>
std::ostream &operator << (std::ostream &out, ValueInterface<DD,TT,CRCR> const &x)
  { return out << x.get(); }

template <class D1, typename T1, typename CR1, class D2, typename T2, typename CR2>
bool
operator ==(
    const ValueInterface<D1, T1, CR1> &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs.get() == rhs.get(); }
template <typename T1, class D2, typename T2, typename CR2>
typename boost::disable_if<boost::mpl::or_<
    boost::is_base_of<
      Detail::ValueInterfaceTag,
      T1
    >,
    boost::mpl::and_<
      boost::is_base_of<
        Detail::value_type_pointer_tag_t,
        typename Detail::ValueTypeClassifier<T2>::tag
      >,
      boost::is_same<T1, std::nullptr_t> 
    >
  >, bool>::type
operator ==(
    T1 const &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs == rhs.get(); }
template <class D2, typename T2, typename CR2>
typename boost::enable_if<boost::is_base_of<
    Detail::value_type_pointer_tag_t,
    typename Detail::ValueTypeClassifier<T2>::tag
  >, bool>::type
operator ==(
    std::nullptr_t null,
    const ValueInterface<D2, T2, CR2> &rhs)
  { assert(!null); return !rhs.get(); }
template <class D1, typename T1, typename CR1, typename T2>
typename boost::disable_if<boost::mpl::or_<
    boost::is_base_of<
      Detail::ValueInterfaceTag,
      T2
    >,
    boost::mpl::and_<
      boost::is_base_of<
        Detail::value_type_pointer_tag_t,
        typename Detail::ValueTypeClassifier<T1>::tag
      >,
      boost::is_same<T2, std::nullptr_t> 
    >
  >, bool>::type
operator ==(
    const ValueInterface<D1, T1, CR1> &lhs,
    T2 const &rhs)
  { return lhs.get() == rhs; }
template <class D1, typename T1, typename CR1>
typename boost::enable_if<boost::is_base_of<
    Detail::value_type_pointer_tag_t,
    typename Detail::ValueTypeClassifier<T1>::tag
  >, bool>::type
operator ==(
    const ValueInterface<D1, T1, CR1> &lhs,
    std::nullptr_t null)
  { assert(!null); return !lhs.get(); }

template <class D1, typename T1, typename CR1, class D2, typename T2, typename CR2>
bool
operator !=(
    const ValueInterface<D1, T1, CR1> &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs.get() != rhs.get(); }
template <typename T1, class D2, typename T2, typename CR2>
typename boost::disable_if<boost::mpl::or_<
    boost::is_base_of<
      Detail::ValueInterfaceTag,
      T1
    >,
    boost::mpl::and_<
      boost::is_base_of<
        Detail::value_type_pointer_tag_t,
        typename Detail::ValueTypeClassifier<T2>::tag
      >,
      boost::is_same<T1, std::nullptr_t> 
    >
  >, bool>::type
operator !=(
    T1 const &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs != rhs.get(); }
template <class D2, typename T2, typename CR2>
typename boost::enable_if<boost::is_base_of<
    Detail::value_type_pointer_tag_t,
    typename Detail::ValueTypeClassifier<T2>::tag
  >, bool>::type
operator !=(
    std::nullptr_t null,
    const ValueInterface<D2, T2, CR2> &rhs)
  { assert(!null); return rhs.get(); }
template <class D1, typename T1, typename CR1, typename T2>
typename boost::disable_if<boost::mpl::or_<
    boost::is_base_of<
      Detail::ValueInterfaceTag,
      T2
    >,
    boost::mpl::and_<
      boost::is_base_of<
        Detail::value_type_pointer_tag_t,
        typename Detail::ValueTypeClassifier<T1>::tag
      >,
      boost::is_same<T2, std::nullptr_t> 
    >
  >, bool>::type
operator !=(
    const ValueInterface<D1, T1, CR1> &lhs,
    T2 const &rhs)
  { return lhs.get() != rhs; }
template <class D1, typename T1, typename CR1>
typename boost::enable_if<boost::is_base_of<
    Detail::value_type_pointer_tag_t,
    typename Detail::ValueTypeClassifier<T1>::tag
  >, bool>::type
operator !=(
    const ValueInterface<D1, T1, CR1> &lhs,
    std::nullptr_t null)
  { assert(!null); return lhs.get(); }


template <class D1, typename T1, typename CR1, class D2, typename T2, typename CR2>
bool
operator < (
    const ValueInterface<D1, T1, CR1> &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs.get() <  rhs.get(); }
template <typename T1, class D2, typename T2, typename CR2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator < (
    T1 const &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs <  rhs.get(); }
template <class D1, typename T1, typename CR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator < (
    const ValueInterface<D1, T1, CR1> &lhs,
    T2 const &rhs)
  { return lhs.get() <  rhs; }

template <class D1, typename T1, typename CR1, class D2, typename T2, typename CR2>
bool
operator <=(
    const ValueInterface<D1, T1, CR1> &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs.get() <= rhs.get(); }
template <typename T1, class D2, typename T2, typename CR2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator <=(
    T1 const &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs <= rhs.get(); }
template <class D1, typename T1, typename CR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator <=(
    const ValueInterface<D1, T1, CR1> &lhs,
    T2 const &rhs)
  { return lhs.get() <= rhs; }

template <class D1, typename T1, typename CR1, class D2, typename T2, typename CR2>
bool
operator > (
    const ValueInterface<D1, T1, CR1> &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs.get() >  rhs.get(); }
template <typename T1, class D2, typename T2, typename CR2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator > (
    T1 const &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs >  rhs.get(); }
template <class D1, typename T1, typename CR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator > (
    const ValueInterface<D1, T1, CR1> &lhs,
    T2 const &rhs)
  { return lhs.get() >  rhs; }

template <class D1, typename T1, typename CR1, class D2, typename T2, typename CR2>
bool
operator >=(
    const ValueInterface<D1, T1, CR1> &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs.get() >= rhs.get(); }
template <typename T1, class D2, typename T2, typename CR2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator >=(
    T1 const &lhs,
    const ValueInterface<D2, T2, CR2> &rhs)
  { return lhs >= rhs.get(); }
template <class D1, typename T1, typename CR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator >=(
    const ValueInterface<D1, T1, CR1> &lhs,
    T2 const &rhs)
  { return lhs.get() >= rhs; }

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VALUEINTERFACE_HPP
