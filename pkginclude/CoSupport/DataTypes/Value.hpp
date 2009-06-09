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

#include <cassert>
#include <new>
#include <ostream>

#include <boost/blank.hpp>
#include <boost/variant.hpp>

namespace CoSupport { namespace DataTypes {

namespace Detail {

  class ValueTypeDecoratorUnknown {};

  template <typename T>
  struct ValueTypeClassifier {
    template <class Derived, typename R>
    struct Decorator {
      typedef ValueTypeDecoratorUnknown type;
    };
  };

#define COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(TYPE,DECORATOR) \
  template <> \
  struct ValueTypeClassifier<TYPE> { \
    template <class Derived, typename R> \
    struct Decorator { \
      typedef DECORATOR<Derived,TYPE,R> type; \
    }; \
  }

  template <class Derived, typename T, typename R = T const &>
  class ValueTypeDecoratorNumeric {
  protected:
    Derived       *getDerived()
      { return static_cast<Derived *>(this); }

    Derived const *getDerived() const
      { return static_cast<Derived const *>(this); }
  public:
    Derived &operator +=(T const &x)
      { getDerived()->set(getDerived()->get() + x); return *getDerived(); }
    Derived &operator -=(T const &x)
      { getDerived()->set(getDerived()->get() - x); return *getDerived(); }
    Derived &operator *=(T const &x)
      { getDerived()->set(getDerived()->get() * x); return *getDerived(); }
    Derived &operator /=(T const &x)
      { getDerived()->set(getDerived()->get() / x); return *getDerived(); }
    Derived &operator %=(T const &x)
      { getDerived()->set(getDerived()->get() % x); return *getDerived(); }
    Derived &operator ++()
      { return *this += 1; }
    Derived &operator --()
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

  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(int,ValueTypeDecoratorNumeric);
  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(long,ValueTypeDecoratorNumeric);
  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(long long,ValueTypeDecoratorNumeric);
  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(unsigned int,ValueTypeDecoratorNumeric);
  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(unsigned long,ValueTypeDecoratorNumeric);
  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(unsigned long long,ValueTypeDecoratorNumeric);
  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(float,ValueTypeDecoratorNumeric);
  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(double,ValueTypeDecoratorNumeric);

  template <class Derived, typename T, typename R = T const &>
  class ValueTypeDecoratorStdString {
    typedef ValueTypeDecoratorStdString<Derived,T,R> this_type;
    template <class DD, typename TT, typename RR>
    friend std::ostream &operator << (std::ostream &, ValueTypeDecoratorStdString<DD,TT,RR> const &);
  protected:
    Derived       *getDerived()
      { return static_cast<Derived *>(this); }

    Derived const *getDerived() const
      { return static_cast<Derived const *>(this); }
  };

  template <class DD, typename TT, typename RR>
  std::ostream &operator << (std::ostream &out, ValueTypeDecoratorStdString<DD,TT,RR> const &x) {
    return out << x.getDerived()->get();
  }

  COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING(std::string,ValueTypeDecoratorStdString);

#undef COSUPPORT_CREATE_VALUETYPE_DECORATOR_MAPPING

} // namespace Detail

template <class Derived, typename T, typename R = T const &>
class ValueInterface
: public Detail::ValueTypeClassifier<T>::
    template Decorator<Derived,R>::type {
  typedef ValueInterface<Derived,T,R> this_type;
protected:
  Derived       *getDerived()
    { return static_cast<Derived *>(this); }

  Derived const *getDerived() const
    { return static_cast<Derived const *>(this); }
public:
  template <class DD, typename TT, typename RR>
  Derived &operator = (const ValueInterface<DD, TT, RR> &val)
    { this->set(val.get()); return *getDerived(); }
  Derived &operator = (const T &val)
    { this->set(val); return *getDerived(); }

  operator R() const
    { return this->get(); }

  template <class DD, typename TT, typename RR>
  void set(const ValueInterface<DD,TT,RR> &val)
    { this->set(val.get()); }
  // setImpl is an interface method which must be implemented in Derived!
  void set(const T &val)
    { getDerived()->setImpl(val); }
  // getImpl is an interface method which must be implemented in Derived!
  R get() const // this may throw
    { return getDerived()->getImpl(); }
};

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

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VALUE_HPP
