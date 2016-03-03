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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEINTERFACE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEINTERFACE_HPP

#include <ostream>

#include <boost/blank.hpp>
#include <boost/variant.hpp>

#include "ValueInterface.hpp"

namespace CoSupport { namespace DataTypes {

namespace Detail {

  class MaybeValueInterfaceTag {};

  template <typename tag, class D, typename T, typename CR>
  class MaybeValueTypeDecorator: public ValueTypeDecorator<tag, D, T, CR> {};
/*
  template <class D, typename T, typename CR>
  class MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, CR>
  : public ValueTypeDecorator<value_type_charptr_tag_t, D, T, CR>
  {
    typedef MaybeValueTypeDecorator<value_type_charptr_tag_t, D, T, CR> this_type;
  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
  };

  template <class D, typename T, typename CR>
  class MaybeValueTypeDecorator<value_type_std_string_tag_t, D, T, CR>
  : public ValueTypeDecorator<value_type_std_string_tag_t, D, T, CR>
  {
    typedef MaybeValueTypeDecorator<value_type_std_string_tag_t, D, T, CR> this_type;
  protected:
    D       *getDerived()
      { return static_cast<D *>(this); }

    D const *getDerived() const
      { return static_cast<D const *>(this); }
  public:
  };
 */

} // namespace Detail

/// This class represents the interface for a storage which may contain a value of type T.
/// \example test_maybevalue.cpp
template <
  class D,
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class MaybeValueInterface
: public Detail::MaybeValueTypeDecorator<
    typename Detail::ValueTypeClassifier<T>::tag, D, T, CR
  >,
  public Detail::MaybeValueInterfaceTag {
  typedef MaybeValueInterface<D,T,CR> this_type;
protected:
  D       *getDerived()
    { return static_cast<D *>(this); }

  D const *getDerived() const
    { return static_cast<D const *>(this); }
public:
  template <class DD, typename TT, typename CRCR>
  D &operator = (const MaybeValueInterface<DD, TT, CRCR> &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const this_type &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const T &val)
    { this->set(val); return *getDerived(); }
  D &operator = (const boost::blank &)
    { this->undef(); return *getDerived(); }

  operator CR() const
    { return this->get(); }

  template <class DD, typename TT, typename CRCR>
  void set(const MaybeValueInterface<DD,TT,CRCR> &val)
    { if (val.isDefined()) this->set(val.get()); else this->undef(); }
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
  // implUndef is an interface method which must be implemented in D!
  void undef() {
    BOOST_STATIC_ASSERT((
      boost::is_same<
          typename boost::function_types::result_type<BOOST_TYPEOF(&D::implUndef)>::type,
          void>
        ::value));
    getDerived()->implUndef();
  }
  // implIsDefined is an interface method which must be implemented in D!
  bool isDefined() const {
    return getDerived()->implIsDefined();
  }
};

template <class DD, typename TT, typename CRCR>
std::ostream &operator << (std::ostream &out, MaybeValueInterface<DD,TT,CRCR> const &x)
  { return x.isDefined() ? out << x.get() : out << "undef"; }

template <class D1, typename T1, typename TCR1, class D2, typename T2, typename TCR2>
bool
operator ==(
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs) {
  bool def;
  return (def = lhs.isDefined()) == rhs.isDefined() &&
    (!def || lhs.get() == rhs.get());
}
template <typename T1, class D2, typename T2, typename TCR2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T1>, bool>::type
operator ==(
    T1 const &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs)
  { return rhs.isDefined() && lhs == rhs.get(); }
template <class D1, typename T1, typename TCR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T2>, bool>::type
operator ==(
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    T2 const &rhs)
  { return lhs.isDefined() && lhs.get() == rhs; }

template <class D1, typename T1, typename TCR1, class D2, typename T2, typename TCR2>
bool
operator !=(
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs) {
  bool def;
  return (def = lhs.isDefined()) != rhs.isDefined() ||
    (def && lhs.get() != rhs.get());
}
template <typename T1, class D2, typename T2, typename TCR2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T1>, bool>::type
operator !=(
    T1 const &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs)
  { return !rhs.isDefined() || lhs != rhs.get(); }
template <class D1, typename T1, typename TCR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T2>, bool>::type
operator !=(
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    T2 const &rhs)
  { return !lhs.isDefined() || lhs.get() != rhs; }

// We postolate undef as smaller than all valid values.
template <class D1, typename T1, typename TCR1, class D2, typename T2, typename TCR2>
bool
operator < (
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs) {
  bool def;
  // 0 0 => def=0 => 0
  // 0 1 => 1
  // 1 0 => def=0 => 0
  // 1 1 => def=1 => lhs.get() <  rhs.get()
  return lhs.isDefined() < (def = rhs.isDefined()) ||
    (def && lhs.get() <  rhs.get());
}
template <typename T1, class D2, typename T2, typename TCR2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T1>, bool>::type
operator < (
    T1 const &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs)
  { return rhs.isDefined() && lhs <  rhs.get(); }
template <class D1, typename T1, typename TCR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T2>, bool>::type
operator < (
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    T2 const &rhs)
  { return !lhs.isDefined() || lhs.get() <  rhs; }

template <class D1, typename T1, typename TCR1, class D2, typename T2, typename TCR2>
bool
operator <=(
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs) {
  // 0 0 => 1
  // 0 1 => 1
  // 1 0 => 0
  // 1 1 => lhs.get() <= rhs.get()
  return !lhs.isDefined() ||
    (rhs.isDefined() && lhs.get() <= rhs.get());
}
template <typename T1, class D2, typename T2, typename TCR2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T1>, bool>::type
operator <=(
    T1 const &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs)
  { return rhs.isDefined() && lhs <= rhs.get(); }
template <class D1, typename T1, typename TCR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T2>, bool>::type
operator <=(
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    T2 const &rhs)
  { return !lhs.isDefined() || lhs.get() <= rhs; }

template <class D1, typename T1, typename TCR1, class D2, typename T2, typename TCR2>
bool
operator > (
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs) {
  bool def;
  // 0 0 => def=0 => 0
  // 0 1 => def=0 => 0
  // 1 0 => 1
  // 1 1 => def=1 => lhs.get() >  rhs.get()
  return (def = lhs.isDefined()) > rhs.isDefined() ||
    (def && lhs.get() >  rhs.get());
}
template <typename T1, class D2, typename T2, typename TCR2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T1>, bool>::type
operator > (
    T1 const &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs)
  { return !rhs.isDefined() || lhs >  rhs.get(); }
template <class D1, typename T1, typename TCR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T2>, bool>::type
operator > (
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    T2 const &rhs)
  { return lhs.isDefined() && lhs.get() >  rhs; }

template <class D1, typename T1, typename TCR1, class D2, typename T2, typename TCR2>
bool
operator >=(
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs) {
  // 0 0 => 1
  // 0 1 => 0
  // 1 0 => 1
  // 1 1 => lhs.get() >= rhs.get()
  return !rhs.isDefined() ||
    (lhs.isDefined() && lhs.get() >= rhs.get());
}
template <typename T1, class D2, typename T2, typename TCR2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T1>, bool>::type
operator >=(
    T1 const &lhs,
    const MaybeValueInterface<D2, T2, TCR2> &rhs)
  { return !rhs.isDefined() || lhs >= rhs.get(); }
template <class D1, typename T1, typename TCR1, typename T2>
typename boost::disable_if<boost::is_base_of<Detail::MaybeValueInterfaceTag, T2>, bool>::type
operator >=(
    const MaybeValueInterface<D1, T1, TCR1> &lhs,
    T2 const &rhs)
  { return lhs.isDefined() && lhs.get() >= rhs; }

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_MAYBEVALUEINTERFACE_HPP
