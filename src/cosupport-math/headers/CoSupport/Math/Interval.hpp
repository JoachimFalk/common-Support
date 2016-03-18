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

#ifndef _INCLUDED_COSUPPORT_MATH_INTERVAL_HPP
#define _INCLUDED_COSUPPORT_MATH_INTERVAL_HPP

#include <limits>
#include <stdint.h>
#include <assert.h>
#include <stdexcept>
#include <iostream>

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/make_signed.hpp>

#include "export_config.h"

namespace CoSupport { namespace Math {

namespace Exception {

  struct COSUPPORT_MATH_API
  IntervalBoundUnderflow
    : public std::underflow_error {

    IntervalBoundUnderflow()
      : std::underflow_error("Ugh: Interval bounds underflow!") {}
  };
  struct COSUPPORT_MATH_API
  IntervalBoundOverflow
    : public std::overflow_error {

    IntervalBoundOverflow()
      : std::overflow_error("Ugh: Interval bounds overflow!") {}
  };

}

namespace Detail {

  struct COSUPPORT_MATH_API
  IntervalEmpty {};

  struct COSUPPORT_MATH_API
  IntervalAll   {};

  template <typename T, bool USE_EXCEPTION, bool IS_SIGNED>
  class IntervalLower;
  template <typename T, bool USE_EXCEPTION, bool IS_SIGNED>
  class IntervalUpper;

  template <typename T, bool IS_SIGNED>
  class IntervalConstantsHelper;

  template <typename T>
  class IntervalConstantsHelper<T, false> {
  public:
    class DummyNINF {};

    static const IntervalEmpty EMPTY;
    static const DummyNINF     NINF;
    static const T             INF;
    static const T             MIN;
    static const T             MAX;
  };
  template <typename T>
  class IntervalConstantsHelper<T, true>
  : public IntervalConstantsHelper<T, false> {
  public:
    static const IntervalAll   ALL;
    static const T             NINF;
    static const T             MIN;
  };

  template <typename T>
  bool operator ==(T, typename IntervalConstantsHelper<T, false>::DummyNINF)
    { return false; }
  template <typename T>
  bool operator ==(typename IntervalConstantsHelper<T, false>::DummyNINF, T)
    { return false; }
  template <typename T>
  bool operator !=(T, typename IntervalConstantsHelper<T, false>::DummyNINF)
    { return true; }
  template <typename T>
  bool operator !=(typename IntervalConstantsHelper<T, false>::DummyNINF, T)
    { return true; }

  template <typename T>
  const T           IntervalConstantsHelper<T, false>::INF = std::numeric_limits<T>::max();
  template <typename T>
  const T           IntervalConstantsHelper<T, false>::MIN = 0;
  template <typename T>
  const T           IntervalConstantsHelper<T, false>::MAX = std::numeric_limits<T>::max()-1;

  template <typename T>
  const T           IntervalConstantsHelper<T, true>::NINF = std::numeric_limits<T>::min();
  template <typename T>
  const T           IntervalConstantsHelper<T, true>::MIN =  std::numeric_limits<T>::min()+1;

  template <typename T>
  struct IntervalConstants
  : public IntervalConstantsHelper<T,std::numeric_limits<T>::is_signed> {};

  template <bool USE_EXCEPTION>
  struct IntervalBoundCheck;

  template <>
  struct IntervalBoundCheck<false> {
    template <typename T, typename V>
    static void checkAdd(T x, V v) {
      assert(x >= IntervalConstants<T>::MIN);
      assert(x <= IntervalConstants<T>::MAX);
      assert((!(v <  0) || (static_cast<typename boost::make_unsigned<T>::type>(x-IntervalConstants<T>::MIN)+1 + v > 0)) &&
        "Ugh: Lower bounds underflow!");
      assert((!(v >= 0) || (static_cast<typename boost::make_unsigned<T>::type>(IntervalConstants<T>::MAX-x)+1 - v > 0)) &&
        "Ugh: Lower bounds overflow!");
    }
    template <typename T, typename V>
    static void checkSub(T x, V v) {
      assert(x >= IntervalConstants<T>::MIN);
      assert(x <= IntervalConstants<T>::MAX);
      assert(((v <  0) || (static_cast<typename boost::make_unsigned<T>::type>(x-IntervalConstants<T>::MIN)+1 - v > 0)) &&
        "Ugh: Lower bounds underflow!");
      assert(((v >= 0) || (static_cast<typename boost::make_unsigned<T>::type>(IntervalConstants<T>::MAX-x)+1 + v > 0)) &&
        "Ugh: Lower bounds overflow!");
    }
    template <typename T, typename V>
    static T checkBound(V v) {
      assert((v >= 0 ||
          static_cast<typename boost::make_signed<V>::type>(v) >=
          static_cast<typename boost::make_signed<T>::type>(IntervalConstants<T>::MIN)) &&
        "Ugh: Lower bounds underflow!");
      assert((v <  0 ||
          static_cast<typename boost::make_unsigned<V>::type>(v) <=
          static_cast<typename boost::make_unsigned<T>::type>(IntervalConstants<T>::MAX)) &&
        "Ugh: Lower bounds overflow!");
      return v;
    }
  };
  template <>
  struct IntervalBoundCheck<true> {
    template <typename T, typename V>
    static void checkAdd(T x, V v) {
      assert(x >= IntervalConstants<T>::MIN);
      assert(x <= IntervalConstants<T>::MAX);
      if (!(!(v <  0) || (static_cast<typename boost::make_unsigned<T>::type>(x-IntervalConstants<T>::MIN)+1 + v > 0)))
        throw Exception::IntervalBoundUnderflow();
      if (!(!(v >= 0) || (static_cast<typename boost::make_unsigned<T>::type>(IntervalConstants<T>::MAX-x)+1 - v > 0)))
        throw Exception::IntervalBoundOverflow();
    }
    template <typename T, typename V>
    static void checkSub(T x, V v) {
      assert(x >= IntervalConstants<T>::MIN);
      assert(x <= IntervalConstants<T>::MAX);
      if (!((v <  0) || (static_cast<typename boost::make_unsigned<T>::type>(x-IntervalConstants<T>::MIN)+1 - v > 0)))
        throw Exception::IntervalBoundUnderflow();
      if (!((v >= 0) || (static_cast<typename boost::make_unsigned<T>::type>(IntervalConstants<T>::MAX-x)+1 + v > 0)))
        throw Exception::IntervalBoundOverflow();
    }
    template <typename T, typename V>
    static T checkBound(V v) {
      if (!(v >= 0 || static_cast<typename boost::make_signed<V>::type>(v) >=
                      static_cast<typename boost::make_signed<T>::type>(IntervalConstants<T>::MIN)))
        throw Exception::IntervalBoundUnderflow();
      if (!(v <  0 || static_cast<typename boost::make_unsigned<V>::type>(v) <=
                      static_cast<typename boost::make_unsigned<T>::type>(IntervalConstants<T>::MAX)))
        throw Exception::IntervalBoundOverflow();
      return v;
    }
  };

  template <typename T, bool USE_EXCEPTION>
  class IntervalLower<T, USE_EXCEPTION, true>
  : public IntervalConstants<T> {
    typedef IntervalLower<T, USE_EXCEPTION, true> this_type;

    template <typename V, bool E, bool S> friend class IntervalLower;
  protected:
    T x;
  public:
    using IntervalConstants<T>::NINF;
    using IntervalConstants<T>::INF;

    IntervalLower():              x(0) {}
    IntervalLower(IntervalEmpty): x(0) {}
    IntervalLower(IntervalAll):   x(NINF) {}

    template <typename V>
    IntervalLower(V v) { *this = v; }

    T get() const
      { return x; }

    void operator = (T v) {
      x = v == NINF
        ? NINF
        : IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(v);
    }

    template <typename V>
    void operator = (V v) {
      x = IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(v);
    }

    template <typename V, bool E>
    void operator = (IntervalLower<V,E,false> i) {
      x = IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(i.x);
    }
    template <typename V, bool E>
    void operator = (IntervalLower<V,E,true> i) {
      x = i.x == IntervalLower<V,E,true>::NINF
        ? NINF
        : IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(i.x);
    }

    template <typename V>
    void operator += (V v) {
      if (x != NINF) {
        IntervalBoundCheck<USE_EXCEPTION>::checkAdd(x, v);
        x += v;
      }
    }
    template <typename V>
    void operator -= (V v) {
      if (x != NINF) {
        IntervalBoundCheck<USE_EXCEPTION>::checkSub(x, v);
        x -= v;
      }
    }
  };

  template <typename T, bool USE_EXCEPTION>
  class IntervalLower<T, USE_EXCEPTION, false>
  : public IntervalConstants<T> {
    typedef IntervalLower<T, USE_EXCEPTION, false> this_type;

    template <typename V, bool E, bool S> friend class IntervalLower;
  protected:
    T x;
  public:
    using IntervalConstants<T>::INF;

    IntervalLower():              x(0) {}
    IntervalLower(IntervalEmpty): x(0) {}

    template <typename V>
    IntervalLower(V v) { *this = v; }

    T get() const
      { return x; }

    template <typename V>
    void operator = (V v) {
      x = IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(v);
    }

    template <typename V, bool E, bool S>
    void operator = (IntervalLower<V,E,S> i) {
      x = IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(i.x);
    }

    template <typename V>
    void operator += (V v) {
      IntervalBoundCheck<USE_EXCEPTION>::checkAdd(x, v);
      x += v;
    }
    template <typename V>
    void operator -= (V v) {
      IntervalBoundCheck<USE_EXCEPTION>::checkSub(x, v);
      x -= v;
    }
  };

  template <typename T, bool USE_EXCEPTION, bool IS_SIGNED>
  class IntervalUpper
  : public IntervalConstants<T> {
    typedef IntervalUpper<T, USE_EXCEPTION, IS_SIGNED> this_type;

    template <typename V, bool E, bool S> friend class IntervalUpper;
  protected:
    T x;
  public:
    using IntervalConstants<T>::INF;

    IntervalUpper():              x(0) {}
    IntervalUpper(IntervalEmpty): x(0) {}
    IntervalUpper(IntervalAll):   x(INF) {}

    template <typename V>
    IntervalUpper(V v) { *this = v; }

    T get() const
      { return x; }

    void operator = (T v) {
      x = v == INF
        ? INF
        : IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(v);
    }

    template <typename V>
    void operator = (V v) {
      x = IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(v);
    }

    template <typename V, bool E, bool S>
    void operator = (IntervalUpper<V,E,S> i) {
      x = i.x == IntervalUpper<V,E,S>::INF
        ? INF
        : IntervalBoundCheck<USE_EXCEPTION>::template checkBound<T>(i.x);
    }

    template <typename V>
    void operator += (V v) {
      if (x != INF) {
        IntervalBoundCheck<USE_EXCEPTION>::checkAdd(x, v);
        x += v;
      }
    }
    template <typename V>
    void operator -= (V v) {
      if (x != INF) {
        IntervalBoundCheck<USE_EXCEPTION>::checkSub(x, v);
        x -= v;
      }
    }
  };

} // namespace Detail

template <typename T, bool USE_EXCEPTION = false>
class Interval
  : public Detail::IntervalConstants<T>
{
  typedef Interval<T, USE_EXCEPTION>   this_type;
  typedef Detail::IntervalConstants<T> base_type;

  template <typename V, bool UE> friend class Interval;
protected:
  // The bounds, interval is lower <= x < upper
  Detail::IntervalLower<T, USE_EXCEPTION, std::numeric_limits<T>::is_signed> lower;
  Detail::IntervalUpper<T, USE_EXCEPTION, std::numeric_limits<T>::is_signed> upper;
public:
  typedef T bound_type;

  using   base_type::EMPTY;

  Interval(Detail::IntervalEmpty d = EMPTY)
    : lower(d), upper(d) {}
  Interval(Detail::IntervalAll d)
    : lower(d), upper(d) {}
  template <typename L, typename U>
  Interval(L l, U u)
    : lower(l), upper(u) {}
  template <typename V>
  Interval(V v)
    : lower(v), upper(v) { upper += 1; }

  template <typename V, bool UE>
  Interval(const Interval<V,UE> &x)
    : lower(x.lower), upper(x.upper) {}

  template <typename V>
  this_type &operator = (V v)
    { lower = v; upper = v; upper += 1; return *this; }

  template <typename V, bool UE>
  this_type &operator = (const Interval<V,UE> &v)
    { lower = v.lower; upper = v.upper; return *this; }

  template <typename V>
  this_type &operator += (V v)
    { lower += v; upper += v; return *this; }
  template <typename V>
  this_type &operator -= (V v)
    { lower -= v; upper -= v; return *this; }

  template <typename V>
  this_type operator + (V v) const
    { return this_type(*this) += v; }
  template <typename V>
  this_type operator - (V v) const
    { return this_type(*this) -= v; }

  bound_type getLowerBound() const
    { return lower.get(); }
  bound_type getUpperBound() const
    { return upper.get(); }
};

template <typename T, bool UE1, bool UE2>
Interval<T, UE1&&UE2>
intersect(const Interval<T,UE1> &a, const Interval<T,UE2> &b) {
  return Interval<T, UE1&&UE2>(
    std::max(a.getLowerBound(), b.getLowerBound()),
    std::min(a.getUpperBound(), b.getUpperBound()));
}

template <typename T, bool E>
std::ostream &operator <<(std::ostream &out, const Interval<T, E> &i) {
  if (i.getLowerBound() == Interval<T, E>::NINF) {
    if (i.getUpperBound() == Interval<T, E>::INF) {
      return out << "ALL";
    } else {
      return out << "<" << i.getUpperBound();
    }
  } else {
    if (i.getUpperBound() == Interval<T, E>::INF) {
      return out << ">=" << i.getLowerBound();
    } else if (i.getUpperBound() == i.getLowerBound()+1) {
      return out << "=" << i.getLowerBound();
    } else if (i.getUpperBound() == i.getLowerBound()) {
      return out << "EMPTY";
    } else {
      return out << "[" << i.getLowerBound() << "," << i.getUpperBound() << ")";
    }
  }
}

} } // namespace CoSupport::Math

#endif // _INCLUDED_COSUPPORT_MATH_INTERVAL_HPP
