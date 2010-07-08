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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VALUE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VALUE_HPP

#include <ostream>

#include <boost/utility.hpp>
#include <boost/type_traits.hpp>

namespace CoSupport { namespace DataTypes {

template <class D, typename T, typename R = T const &>
class ValueInterface;

namespace Detail {

  class ValueInterfaceTag {};

  struct value_type_unknown_tag_t {};

  template <typename T>
  struct ValueTypeClassifier { typedef value_type_unknown_tag_t tag; };

  template <typename tag, class D, typename T, typename R>
  class ValueTypeDecorator;

  template <class D, typename T, typename R>
  class ValueTypeDecorator<value_type_unknown_tag_t, D, T, R> {};

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

  template <class D, typename T, typename R>
  class ValueTypeDecorator<value_type_numeric_tag_t, D, T, R> {
    typedef ValueTypeDecorator<value_type_numeric_tag_t, D, T, R> this_type;
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

  struct value_type_charptr_tag_t {};

  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(char *,value_type_charptr_tag_t);
  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(const char *,value_type_charptr_tag_t);

  template <class D, typename T, typename R>
  class ValueTypeDecorator<value_type_charptr_tag_t, D, T, R> {
    typedef ValueTypeDecorator<value_type_charptr_tag_t, D, T, R> this_type;

  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
  };

  struct value_type_std_string_tag_t {};

  COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION(std::string,value_type_std_string_tag_t);

  template <class D, typename T, typename R>
  class ValueTypeDecorator<value_type_std_string_tag_t, D, T, R> {
    typedef ValueTypeDecorator<value_type_std_string_tag_t, D, T, R> this_type;
  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
  };

#undef COSUPPORT_CREATE_VALUETYPE_TAG_ASSOCIATION

} // namespace Detail

template <class D, typename T, typename R>
class ValueInterface
: public Detail::ValueTypeDecorator<
    typename Detail::ValueTypeClassifier<T>::tag, D, T, R
  >,
  public Detail::ValueInterfaceTag {
  typedef ValueInterface<D,T,R> this_type;
protected:
  D       *getDerived()
    { return static_cast<D *>(this); }

  D const *getDerived() const
    { return static_cast<D const *>(this); }
public:
  template <class DD, typename TT, typename RR>
  D &operator = (const ValueInterface<DD, TT, RR> &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const this_type &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const T &val)
    { this->set(val); return *getDerived(); }

  operator R() const
    { return this->get(); }

  template <class DD, typename TT, typename RR>
  void set(const ValueInterface<DD,TT,RR> &val)
    { this->set(val.get()); }
  // setImpl is an interface method which must be implemented in D!
  void set(const T &val)
    { getDerived()->setImpl(val); }
  // getImpl is an interface method which must be implemented in D!
  R get() const // this may throw
    { return getDerived()->getImpl(); }
};

template <class DD, typename TT, typename RR>
std::ostream &operator << (std::ostream &out, ValueInterface<DD,TT,RR> const &x)
  { return out << x.get(); }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool
operator ==(
    const ValueInterface<D1, T1, R1> &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs.get() == rhs.get(); }
template <typename T1, class D2, typename T2, typename R2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator ==(
    T1 const &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs == rhs.get(); }
template <class D1, typename T1, typename R1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator ==(
    const ValueInterface<D1, T1, R1> &lhs,
    T2 const &rhs)
  { return lhs.get() == rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool
operator !=(
    const ValueInterface<D1, T1, R1> &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs.get() != rhs.get(); }
template <typename T1, class D2, typename T2, typename R2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator !=(
    T1 const &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs != rhs.get(); }
template <class D1, typename T1, typename R1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator !=(
    const ValueInterface<D1, T1, R1> &lhs,
    T2 const &rhs)
  { return lhs.get() != rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool
operator < (
    const ValueInterface<D1, T1, R1> &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs.get() <  rhs.get(); }
template <typename T1, class D2, typename T2, typename R2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator < (
    T1 const &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs <  rhs.get(); }
template <class D1, typename T1, typename R1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator < (
    const ValueInterface<D1, T1, R1> &lhs,
    T2 const &rhs)
  { return lhs.get() <  rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool
operator <=(
    const ValueInterface<D1, T1, R1> &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs.get() <= rhs.get(); }
template <typename T1, class D2, typename T2, typename R2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator <=(
    T1 const &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs <= rhs.get(); }
template <class D1, typename T1, typename R1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator <=(
    const ValueInterface<D1, T1, R1> &lhs,
    T2 const &rhs)
  { return lhs.get() <= rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool
operator > (
    const ValueInterface<D1, T1, R1> &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs.get() >  rhs.get(); }
template <typename T1, class D2, typename T2, typename R2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator > (
    T1 const &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs >  rhs.get(); }
template <class D1, typename T1, typename R1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator > (
    const ValueInterface<D1, T1, R1> &lhs,
    T2 const &rhs)
  { return lhs.get() >  rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool
operator >=(
    const ValueInterface<D1, T1, R1> &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs.get() >= rhs.get(); }
template <typename T1, class D2, typename T2, typename R2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T1>, bool>::type
operator >=(
    T1 const &lhs,
    const ValueInterface<D2, T2, R2> &rhs)
  { return lhs >= rhs.get(); }
template <class D1, typename T1, typename R1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::ValueInterfaceTag, T2>, bool>::type
operator >=(
    const ValueInterface<D1, T1, R1> &lhs,
    T2 const &rhs)
  { return lhs.get() >= rhs; }

template <class T>
class Value
: public ValueInterface<Value<T>, T> {
  typedef Value<T>                      this_type;
  typedef ValueInterface<this_type, T>  base_type;

  friend class ValueInterface<this_type, T>;
private:
  T value;
protected:
  void setImpl(const T &val)
    { value = val; }
  T const &getImpl() const
    { return value; }
public:
  Value() {}
  Value(T const &val)
    : value(val) {}
  template <class DD, typename TT, typename RR>
  Value(ValueInterface<DD,TT,RR> const &val)
    : value(val.get()) {}

//You may need this if you can't rely on the default
//assignment operator to do the job correctly!
//Here we can rely on T::operator = of value.
//this_type &operator = (const this_type &val)
//  { return base_type::operator =(val); }

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VALUE_HPP
