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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP

#include <cassert>
#include <new>
#include <ostream>

#include <boost/blank.hpp>
#include <boost/variant.hpp>

#include "Value.hpp"

namespace CoSupport { namespace DataTypes {

namespace Detail {

  template <typename tag, class D, typename T, typename R>
  class MaybeValueTypeDecorator: public ValueTypeDecorator<tag, D, T, R> {};

  template <class D, typename T, typename R>
  class MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R>;

  template <class D, typename T, typename R>
  bool operator ==(
      std::string const &lhs,
      MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R> const &rhs)
    { return rhs.getDerived()->isDefined() && lhs == rhs.getDerived()->get(); }
  template <class D, typename T, typename R>
  bool operator !=(
      std::string const &lhs,
      MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R> const &rhs)
    { return !rhs.getDerived()->isDefined() || lhs != rhs.getDerived()->get(); }
  template <class D, typename T, typename R>
  bool operator > (
      std::string const &lhs,
      MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R> const &rhs)
    { return !rhs.getDerived()->isDefined() || lhs >  rhs.getDerived()->get(); }
  template <class D, typename T, typename R>
  bool operator >=(
      std::string const &lhs,
      MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R> const &rhs)
    { return !rhs.getDerived()->isDefined() || lhs >= rhs.getDerived()->get(); }
  template <class D, typename T, typename R>
  bool operator < (
      std::string const &lhs,
      MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R> const &rhs)
    { return rhs.getDerived()->isDefined() && lhs <  rhs.getDerived()->get(); }
  template <class D, typename T, typename R>
  bool operator <=(
      std::string const &lhs,
      MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R> const &rhs)
    { return rhs.getDerived()->isDefined() && lhs <= rhs.getDerived()->get(); }

  template <class D, typename T, typename R>
  class MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R>
  : public ValueTypeDecorator<value_type_charptr_tag_t, D, T, R>
  {
    typedef MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, R> this_type;

#ifndef _MSC_VER
    friend bool operator ==<>(std::string const &, this_type const &);
    friend bool operator !=<>(std::string const &, this_type const &);
    friend bool operator > <>(std::string const &, this_type const &);
    friend bool operator >=<>(std::string const &, this_type const &);
    friend bool operator < <>(std::string const &, this_type const &);
    friend bool operator <=<>(std::string const &, this_type const &);
  protected:
#else
  public:
#endif // _MSC_VER
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
    bool operator ==(std::string const &v) const
      { return getDerived()->isDefined() && getDerived()->get() == v; }
    bool operator !=(std::string const &v) const
      { return !getDerived()->isDefined() || getDerived()->get() != v; }
    bool operator < (std::string const &v) const
      { return !getDerived()->isDefined() || getDerived()->get() <  v; }
    bool operator <=(std::string const &v) const
      { return !getDerived()->isDefined() || getDerived()->get() <= v; }
    bool operator > (std::string const &v) const
      { return getDerived()->isDefined() && getDerived()->get() >  v; }
    bool operator >=(std::string const &v) const
      { return getDerived()->isDefined() && getDerived()->get() >= v; }
  };

  template <class D, typename T, typename R>
  class MaybeValueTypeDecorator<value_type_std_string_tag_t, D, T, R>
  : public ValueTypeDecorator<value_type_std_string_tag_t, D, T, R>
  {
    typedef MaybeValueTypeDecorator<value_type_std_string_tag_t, D, T, R> this_type;
  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
    bool operator ==(const char *v) const
      { return getDerived()->isDefined() && getDerived()->get() == v; }
    bool operator !=(const char *v) const
      { return !getDerived()->isDefined() || getDerived()->get() != v; }
    bool operator < (const char *v) const
      { return !getDerived()->isDefined() || getDerived()->get() <  v; }
    bool operator <=(const char *v) const
      { return !getDerived()->isDefined() || getDerived()->get() <= v; }
    bool operator > (const char *v) const
      { return getDerived()->isDefined() && getDerived()->get() >  v; }
    bool operator >=(const char *v) const
      { return getDerived()->isDefined() && getDerived()->get() >= v; }
  };

} // namespace Detail

template <class D, typename T, typename R = T const &>
class MaybeValueInterface
: public Detail::MaybeValueTypeDecorator<
    typename Detail::ValueTypeClassifier<T>::tag, D, T, R
  > {
  typedef MaybeValueInterface<D,T,R> this_type;
protected:
  D       *getDerived()
    { return static_cast<D *>(this); }

  D const *getDerived() const
    { return static_cast<D const *>(this); }
public:
  template <class DD, typename TT, typename RR>
  D &operator = (const MaybeValueInterface<DD, TT, RR> &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const this_type &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const T &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const boost::blank &)
    { this->undef(); return *getDerived(); }

  operator R() const
    { return this->get(); }

  template <class DD, typename TT, typename RR>
  void set(const MaybeValueInterface<DD,TT,RR> &val)
    { if (val.isDefined()) this->set(val.get()); else this->undef(); }
  // setImpl is an interface method which must be implemented in D!
  void set(const T &val)
    { getDerived()->setImpl(val); }
  // getImpl is an interface method which must be implemented in D!
  R get() const // this may throw
    { return getDerived()->getImpl(); }
  // undefImpl is an interface method which must be implemented in D!
  void undef()
    { return getDerived()->undefImpl(); }
  // isDefinedImpl is an interface method which must be implemented in D!
  bool isDefined() const
    { return getDerived()->isDefinedImpl(); }
};

template <class DD, typename TT, typename RR>
std::ostream &operator << (std::ostream &out, MaybeValueInterface<DD,TT,RR> const &x)
  { return x.isDefined() ? out << x.get() : out << "undef"; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool operator ==(
    const MaybeValueInterface<D1, T1, R1> &lhs,
    const MaybeValueInterface<D2, T2, R2> &rhs) {
  bool def;
  return (def = lhs.isDefined()) == rhs.isDefined() &&
    (!def || lhs.get() == rhs.get());
}
template <class D, typename T, typename R>
bool operator ==(
    T const &lhs,
    const MaybeValueInterface<D, T, R> &rhs)
  { return rhs.isDefined() && lhs == rhs.get(); }
template <class D, typename T, typename R>
bool operator ==(
    const MaybeValueInterface<D, T, R> &lhs,
    T const &rhs)
  { return lhs.isDefined() && lhs.get() == rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool operator !=(
    const MaybeValueInterface<D1, T1, R1> &lhs,
    const MaybeValueInterface<D2, T2, R2> &rhs) {
  bool def;
  return (def = lhs.isDefined()) != rhs.isDefined() ||
    (def && lhs.get() != rhs.get());
}
template <class D, typename T, typename R>
bool operator !=(
    T const &lhs,
    const MaybeValueInterface<D, T, R> &rhs)
  { return !rhs.isDefined() || lhs != rhs.get(); }
template <class D, typename T, typename R>
bool operator !=(
    const MaybeValueInterface<D, T, R> &lhs,
    T const &rhs)
  { return !lhs.isDefined() || lhs.get() != rhs; }

// We postolate undef as smaller than all valid values.
template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool operator < (
    const MaybeValueInterface<D1, T1, R1> &lhs,
    const MaybeValueInterface<D2, T2, R2> &rhs) {
  bool def;
  // 0 0 => def=0 => 0
  // 0 1 => 1
  // 1 0 => def=0 => 0
  // 1 1 => def=1 => lhs.get() <  rhs.get()
  return lhs.isDefined() < (def = rhs.isDefined()) ||
    (def && lhs.get() <  rhs.get());
}
template <class D, typename T, typename R>
bool operator < (
    T const &lhs,
    const MaybeValueInterface<D, T, R> &rhs)
  { return rhs.isDefined() && lhs <  rhs.get(); }
template <class D, typename T, typename R>
bool operator < (
    const MaybeValueInterface<D, T, R> &lhs,
    T const &rhs)
  { return !lhs.isDefined() || lhs.get() <  rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool operator <=(
    const MaybeValueInterface<D1, T1, R1> &lhs,
    const MaybeValueInterface<D2, T2, R2> &rhs) {
  // 0 0 => 1
  // 0 1 => 1
  // 1 0 => 0
  // 1 1 => lhs.get() <= rhs.get()
  return !lhs.isDefined() ||
    (rhs.isDefined() && lhs.get() <= rhs.get());
}
template <class D, typename T, typename R>
bool operator <=(
    T const &lhs,
    const MaybeValueInterface<D, T, R> &rhs)
  { return rhs.isDefined() && lhs <= rhs.get(); }
template <class D, typename T, typename R>
bool operator <=(
    const MaybeValueInterface<D, T, R> &lhs,
    T const &rhs)
  { return !lhs.isDefined() || lhs.get() <= rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool operator > (
    const MaybeValueInterface<D1, T1, R1> &lhs,
    const MaybeValueInterface<D2, T2, R2> &rhs) {
  bool def;
  // 0 0 => def=0 => 0
  // 0 1 => def=0 => 0
  // 1 0 => 1
  // 1 1 => def=1 => lhs.get() >  rhs.get()
  return (def = lhs.isDefined()) > rhs.isDefined() ||
    (def && lhs.get() >  rhs.get());
}
template <class D, typename T, typename R>
bool operator > (
    T const &lhs,
    const MaybeValueInterface<D, T, R> &rhs)
  { return !rhs.isDefined() || lhs >  rhs.get(); }
template <class D, typename T, typename R>
bool operator > (
    const MaybeValueInterface<D, T, R> &lhs,
    T const &rhs)
  { return lhs.isDefined() && lhs.get() >  rhs; }

template <class D1, typename T1, typename R1, class D2, typename T2, typename R2>
bool operator >=(
    const MaybeValueInterface<D1, T1, R1> &lhs,
    const MaybeValueInterface<D2, T2, R2> &rhs) {
  // 0 0 => 1
  // 0 1 => 0
  // 1 0 => 1
  // 1 1 => lhs.get() >= rhs.get()
  return !rhs.isDefined() ||
    (lhs.isDefined() && lhs.get() >= rhs.get());
}
template <class D, typename T, typename R>
bool operator >=(
    T const &lhs,
    const MaybeValueInterface<D, T, R> &rhs)
  { return !rhs.isDefined() || lhs >= rhs.get(); }
template <class D, typename T, typename R>
bool operator >=(
    const MaybeValueInterface<D, T, R> &lhs,
    T const &rhs)
  { return lhs.isDefined() && lhs.get() >= rhs; }

template <class T>
class MaybeValue
: public MaybeValueInterface<MaybeValue<T>, T> {
  typedef MaybeValue<T>                      this_type;
  typedef MaybeValueInterface<this_type, T>  base_type;

  friend class MaybeValueInterface<this_type, T>;
private:
  typedef boost::variant<boost::blank, T>    storage_type;

  storage_type value;
protected:
  void setImpl(const T &val)
    { value = val; }
  T const &getImpl() const
    { return boost::get<T>(value); }
  void undefImpl()
    { value = boost::blank(); }
  bool isDefinedImpl() const
    { return boost::get<boost::blank>(&value) == NULL; }
public:
  MaybeValue()
    : value(boost::blank()) {}
  MaybeValue(boost::blank)
    : value(boost::blank()) {}
  MaybeValue(T const &val)
    : value(val) {}
  template <class DD, typename TT, typename RR>
  MaybeValue(MaybeValueInterface<DD,TT,RR> const &val)
    : value(val.isDefined()
        ? storage_type(val.get())
        : storage_type(boost::blank())) {}

//You may need this if you can't rely on the default
//assignment operator to do the job correctly!
//Here we can rely on storage_type::operator = of value.
//this_type &operator = (const this_type &val)
//  { return base_type::operator =(val); }

  using base_type::operator =;
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUE_HPP
