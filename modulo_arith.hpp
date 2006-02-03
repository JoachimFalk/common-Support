/* vim: set sw=2 ts=8: */
/*
 * Copyright (C) Joachim Falk <joachim.falk@gmx.de> $Date: 2001/06/26 18:27:53 $
 *
 * modulo_arith.hpp is part of the jf_libs distribution of Joachim Falk;
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * The jf_libs distributio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _INCLUDED_MODULO_ARITH_HPP
#define _INCLUDED_MODULO_ARITH_HPP

#include "sassert.h"

namespace CoSupport {

template <size_t N>
class ModuloInt {
public:
  typedef ModuloInt this_type;
  
  typedef size_t (this_type::*unspecified_bool_type)() const;
protected:
  size_t v;
public:
  ModuloInt( size_t v = 0 )
    : v(v) {
    assert( v >= 0 && v < N );
  }
  ModuloInt( int _v )
    : v(_v >= 0 ? _v : N + _v) {
    assert( v >= 0 && v < N );
  }
  
  this_type &operator += (this_type n) {
    if ( (v += n.v) >= N )
      v -= N;
    assert( v >= 0 && v < N );
    return *this;
  }
  this_type &operator -= (this_type n) {
    if ( (v -= n.v) >= N )
      v += N;
    assert( v >= 0 && v < N );
    return *this;
  }
  
  this_type operator + (this_type rhs) const
    { return this_type(*this) += rhs; }
  this_type operator - (this_type rhs) const
    { return this_type(*this) -= rhs; }
  bool operator == (this_type rhs) const
    { return v == rhs.v; }
  bool operator != (this_type rhs) const
    { return !(*this == rhs); }
  
  size_t getValue() const
    { return v; }
  
  operator unspecified_bool_type() const // never throws
    { return v ? &this_type::getValue : NULL; }
  
  this_type &operator ++() {
    return *this += 1;
  }
  this_type  operator ++(int) {
    this_type retval(*this);
    *this += 1;
    return retval;
  }
  this_type &operator --() {
    return *this -= 1;
  }
  this_type  operator --(int) {
    this_type retval(*this);
    *this -= 1;
    return retval;
  }
};

} // namespace CoSupport

#endif /* _INCLUDED_MODULO_ARITH_HPP */
