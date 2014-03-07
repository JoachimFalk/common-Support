/* vim: set sw=2 ts=8: */
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

#ifndef _INCLUDED_COSUPPORT_MATH_MODULOGROUP_HPP
#define _INCLUDED_COSUPPORT_MATH_MODULOGROUP_HPP

#include <boost/utility.hpp>
#include <boost/type_traits.hpp>

#include "../sassert.h"

namespace CoSupport { namespace Math {

template <typename T>
class Modulus {
public:
  typedef T value_type;
private:
  T m_;
public:
  explicit Modulus(const T &m_)
    : m_(m_) {}

  const T &m() const { return m_; }
};

// Compile time constant
template <typename T, T N>
class CTModulus {
public:
  typedef T value_type;
public:
  T m() const { return N; }
};

namespace Detail {

  template <class MODULUS, class DERIVED>
  class ModuloGroupImpl: public MODULUS {
  public:
    typedef ModuloGroupImpl this_type;

    typedef MODULUS M;

    using M::m;

    typedef typename M::value_type value_type;

    typedef size_t (this_type::*unspecified_bool_type)() const;
  protected:
    value_type e;
  private:
    DERIVED       &getDerived()
      { return *static_cast<DERIVED       *>(this); }
    DERIVED const &getDerived() const
      { return *static_cast<DERIVED const *>(this); }
  public:
    ModuloGroupImpl(value_type e = value_type(0), const M &m_ = M())
      : M(m_), e(e) { assert(e < m()); }

    template <class MM, class MD>
    DERIVED &operator += (const ModuloGroupImpl<MM, MD> &n) {
      assert(n.m() == m());
      if (e >= m() - n.getValue())
        e -= m() - n.getValue();
      else
        e += n.getValue();
      assert(e >= value_type(0) && e < m());
      return getDerived();
    }
    template <class TT>
    typename boost::enable_if<boost::is_integral<TT>, DERIVED &>::type
    operator += (const TT &n)
      { return getDerived() += DERIVED(n, *this); }

    template <class MM, class MD>
    DERIVED &operator -= (const ModuloGroupImpl<MM, MD> &n) {
      assert(n.m() == m());
      if (e < n.getValue())
        e += m() - n.getValue();
      else
        e -= n.getValue();
      assert(e >= value_type(0) && e < m());
      return getDerived();
    }
    template <class TT>
    typename boost::enable_if<boost::is_integral<TT>, DERIVED &>::type
    operator -= (const TT &n)
      { return getDerived() -= DERIVED(n, *this); }

    template <class MM, class MD>
    DERIVED operator + (const ModuloGroupImpl<MM, MD> &n) const {
      return DERIVED(getDerived()) += n;
    }
    template <class TT>
    typename boost::enable_if<boost::is_integral<TT>, DERIVED>::type
    operator + (const TT &n) const
      { return getDerived() + DERIVED(n, *this); }

    template <class MM, class MD>
    DERIVED operator - (const ModuloGroupImpl<MM, MD> &n) const {
      return DERIVED(getDerived()) -= n;
    }
    template <class TT>
    typename boost::enable_if<boost::is_integral<TT>, DERIVED>::type
    operator - (const TT &n) const
      { return getDerived() - DERIVED(n, *this); }

    template <class MM, class MD>
    bool operator == (const ModuloGroupImpl<MM, MD> &n) const {
      assert(n.m() == m());
      return e == n.getValue();
    }
    template <class TT>
    typename boost::enable_if<boost::is_integral<TT>, bool>::type
    operator == (const TT &n) const
      { return getDerived() == DERIVED(n, *this); }

    template <class MM, class MD>
    bool operator != (const ModuloGroupImpl<MM, MD> &n) const
      { return !(getDerived() == n); }
    template <class TT>
    typename boost::enable_if<boost::is_integral<TT>, bool>::type
    operator != (const TT &n) const
      { return getDerived() != DERIVED(n, *this); }

    const value_type &getValue() const
      { return e; }

    // check if a <= e <= b
    template <class AM, class AD, class BM, class BD>
    bool between(const ModuloGroupImpl<AM, AD> &a_, const ModuloGroupImpl<BM, BD> &b_) const {
      assert(a_.m() == m()); assert(b_.m() == m());
      const value_type &a = a_.getValue();
      const value_type &b = b_.getValue();
      
      // a <= e <= b in modulo arith:
      //   a == b   implies a == b == e
      //   a-b == 1 implies true
      return
        (a >  b && (
          /* ---b----------
           * ----a---------
           * ----eeeeeeeeee
           */
            a <= e ||
          /* ---b----------
           * ----a---------
           * eeee----------
           */
            e <= b)) ||
        (a <= b && (
          /* ---a----------
           * -----------b--
           * ---eeeeeeeee--
           */
            a <= e && e <= b));
    }
    template <class AM, class AD, class TT>
    typename boost::enable_if<boost::is_integral<TT>, bool>::type
    between(const ModuloGroupImpl<AM, AD> &a, const TT &b) const
      { return between(a, DERIVED(b, *this)); }
    template <class TT, class BM, class BD>
    typename boost::enable_if<boost::is_integral<TT>, bool>::type
    between(const TT &a, const ModuloGroupImpl<BM, BD> &b) const
      { return between(DERIVED(a, *this), b); }
    template <class T1, class T2>
    typename boost::enable_if<boost::mpl::and_<
      boost::is_integral<T1>, boost::is_integral<T2> >, bool>::type
    between(const T1 &a, const T2 &b)
      { return between(DERIVED(a, *this), DERIVED(b, *this)); }

    operator unspecified_bool_type() const // never throws
      { return e ? &this_type::getValue : NULL; }

    DERIVED &operator ++() {
      return getDerived() += value_type(1);
    }
    DERIVED operator ++(int) {
      DERIVED retval(getDerived());
      getDerived() += value_type(1);
      return retval;
    }
    DERIVED &operator --() {
      return getDerived() -= value_type(1);
    }
    DERIVED operator --(int) {
      DERIVED retval(getDerived());
      getDerived() -= value_type(1);
      return retval;
    }
  };

} // namespace Detail

template <class MODULUS>
class ModuloGroup: public Detail::ModuloGroupImpl<MODULUS, ModuloGroup<MODULUS> > {
  typedef ModuloGroup<MODULUS>                        this_type;
  typedef Detail::ModuloGroupImpl<MODULUS, this_type> base_type;
public:
  typedef typename this_type::value_type value_type;
  typedef typename this_type::M          M;
public:

  ModuloGroup(const M &m = M())
    : base_type(value_type(0), m) {}
  template <typename T>
  ModuloGroup(T e, const M &m = M())
    : base_type((e < 0 ? m.m() + e : e), m) {}
};

} } // namespace CoSupport::Math

#endif /* _INCLUDED_COSUPPORT_MATH_MODULOGROUP_HPP */
