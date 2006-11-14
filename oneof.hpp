// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2006 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_ONEOF_HPP
#define _INCLUDED_ONEOF_HPP

#include <assert.h>
#include <new>
#include <iostream>
#include <typeinfo>

#define _ONEOFDEBUG(x) do {} while (0)
//#define _ONEOFDEBUG(x) std::cerr << x << std::endl

namespace CoSupport {

typedef unsigned char oneof_typeid;

struct NILTYPE;

namespace detail {

  struct void2_st {};
  struct void3_st {};
  struct void4_st {};
  struct void5_st {};

  static inline
  std::ostream &operator << (std::ostream &o, const void2_st &) { return o; }

  static inline
  std::ostream &operator << (std::ostream &o, const void3_st &) { return o; }

  static inline
  std::ostream &operator << (std::ostream &o, const void4_st &) { return o; }

  static inline
  std::ostream &operator << (std::ostream &o, const void5_st &) { return o; }

  template <typename T>
  struct CallVisitor {
    template <class V> static
    typename V::result_type apply(const T &obj, V &v)
      { return v(obj); }
  };
  
  template <>
  struct CallVisitor<void2_st> {
    template <class V> static
    typename V::result_type apply(const void2_st &obj, V &v)
      { assert(1 ? 0 : "oneof contains void2_st !"); }
  };
  
  template <>
  struct CallVisitor<void3_st> {
    template <class V> static
    typename V::result_type apply(const void3_st &obj, V &v)
      { assert(1 ? 0 : "oneof contains void3_st !"); }
  };

  template <>
  struct CallVisitor<void4_st> {
    template <class V> static
    typename V::result_type apply(const void4_st &obj, V &v)
      { assert(1 ? 0 : "oneof contains void4_st !"); }
  };

  template <>
  struct CallVisitor<void5_st> {
    template <class V> static
    typename V::result_type apply(const void5_st &obj, V &v)
      { assert(1 ? 0 : "oneof contains void5_st !"); }
  };

  template <>
  struct CallVisitor<NILTYPE> {
    template <class V> static
    typename V::result_type apply(V &v)
      { assert(1 ? 0 : "oneof contains NILTYPE !"); }
  };

  struct OutputVisitor {
    typedef void result_type;

    std::ostream &output;

    OutputVisitor(std::ostream &output): output(output) {}

    template <typename T>
    void operator()(const T &e) const
      { output << "oneof(" << typeid(T).name() << ":" << e << ")"; }
  };
};

template <
  typename T1,
  typename T2 = detail::void2_st,
  typename T3 = detail::void3_st,
  typename T4 = detail::void4_st,
  typename T5 = detail::void5_st >
class oneof;


template <typename, typename>
struct oneofTypeid;

// 1
template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<oneof<T1,T2,T3,T4,T5>,T1>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<oneof<T1,T2,T3,T4,T5>,T1>::type = 1;

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T1>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T1>::type = 1;

// 2
template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<oneof<T1,T2,T3,T4,T5>,T2>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<oneof<T1,T2,T3,T4,T5>,T2>::type = 2;

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T2>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T2>::type = 2;

// 3
template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<oneof<T1,T2,T3,T4,T5>,T3>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<oneof<T1,T2,T3,T4,T5>,T3>::type = 3;

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T3>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T3>::type = 3;

// 4
template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<oneof<T1,T2,T3,T4,T5>,T4>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<oneof<T1,T2,T3,T4,T5>,T4>::type = 4;

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T4>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T4>::type = 4;

// 5
template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<oneof<T1,T2,T3,T4,T5>,T5>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<oneof<T1,T2,T3,T4,T5>,T5>::type = 5;

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T5>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<const oneof<T1,T2,T3,T4,T5>,T5>::type = 5;

// 0
template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<oneof<T1,T2,T3,T4,T5>,NILTYPE>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<oneof<T1,T2,T3,T4,T5>,NILTYPE>::type = 0;

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct oneofTypeid<const oneof<T1,T2,T3,T4,T5>,NILTYPE>
  { static const oneof_typeid type; };
template <typename T1, typename T2, typename T3, typename T4, typename T5>
const oneof_typeid oneofTypeid<const oneof<T1,T2,T3,T4,T5>,NILTYPE>::type = 0;


template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
static inline
bool isType( const oneof<T1,T2,T3,T4,T5> &of )
  { return oneofTypeid<oneof<T1,T2,T3,T4,T5>,T>::type == of.type(); }

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class oneof {
  public:
    typedef oneof<T1,T2,T3,T4,T5> this_type;
  private:
    union {
      long long alignment_dummy;
      char e1[sizeof(T1)];
      char e2[sizeof(T2)];
      char e3[sizeof(T3)];
      char e4[sizeof(T4)];
      char e5[sizeof(T5)];
    } mem;
    
    oneof_typeid valid;
    
    template <typename T>
    void _construct(const T &e) {
      assert( isType<NILTYPE>(*this) );
      valid = oneofTypeid<this_type,T>::type;
      new(reinterpret_cast<T*>(&mem)) T(e);
    }
    template <typename T>
    T &_element() {
      assert(isType<T>(*this));
      return *reinterpret_cast<T*>(&mem);
    }
    template <typename T>
    const T &_element() const {
      assert(isType<T>(*this));
      return *reinterpret_cast<const T*>(&mem);
    }
    template <typename T>
    void _destroy() {
      assert(isType<T>(*this));
      _call_destructor(reinterpret_cast<T*>(&mem));
      valid = oneofTypeid<this_type,NILTYPE>::type;
    }
    template <class T> void _call_destructor( T  *x ) { x->~T(); }
    template <typename T> void _call_destructor( T ) {}
  public:
    oneof(): valid(oneofTypeid<this_type,NILTYPE>::type)
      { _ONEOFDEBUG("oneof()"); }
    oneof(const this_type &x): valid(oneofTypeid<this_type,NILTYPE>::type) {
      if ( x.valid != oneofTypeid<this_type,NILTYPE>::type )
        _ONEOFDEBUG("oneof(const oneof &) (T" << static_cast<size_t>(x.valid) << ")");
      else
        _ONEOFDEBUG("oneof(const oneof &) ()");
      *this = x;
    }
    oneof(const T1 &e): valid(oneofTypeid<this_type,NILTYPE>::type) {
      _ONEOFDEBUG("oneof( const " << typeid(T1).name() << " & )");
      _construct<T1>(e);
    }
    oneof(const T2 &e): valid(oneofTypeid<this_type,NILTYPE>::type) {
      _ONEOFDEBUG("oneof( const " << typeid(T2).name() << " & )");
      _construct<T2>(e);
    }
    oneof(const T3 &e): valid(oneofTypeid<this_type,NILTYPE>::type) {
      _ONEOFDEBUG("oneof( const " << typeid(T3).name() << " & )");
      _construct<T3>(e);
    }
    oneof(const T4 &e): valid(oneofTypeid<this_type,NILTYPE>::type) {
      _ONEOFDEBUG("oneof( const " << typeid(T4).name() << " & )");
      _construct<T4>(e);
    }
    oneof(const T5 &e): valid(oneofTypeid<this_type,NILTYPE>::type) {
      _ONEOFDEBUG("oneof( const " << typeid(T5).name() << " & )");
      _construct<T5>(e);
    }
    
    this_type &operator = (const this_type &x) {
      if ( x.valid != oneofTypeid<this_type,NILTYPE>::type )
        _ONEOFDEBUG("oneof = const oneof & (T" << static_cast<size_t>(x.valid) << ")");
      else
        _ONEOFDEBUG("oneof = const oneof & ()");
      switch( x.valid ) {
        case oneofTypeid<this_type, T1>::type:
          *this = (x._element<T1>());
          break;
        case oneofTypeid<this_type, T2>::type:
          *this = (x._element<T2>());
          break;
        case oneofTypeid<this_type, T3>::type:
          *this = (x._element<T3>());
          break;
        case oneofTypeid<this_type, T4>::type:
          *this = (x._element<T4>());
          break;
        case oneofTypeid<this_type, T5>::type:
          *this = (x._element<T5>());
          break;
        default:
          assert( isType<NILTYPE>(x) );
          reset();
      }
      return *this;
    }
    this_type &operator = (const T1 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T1).name() << " &");
      reset(); _construct<T1>(x); return *this;
    }
    this_type &operator = (const T2 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T2).name() << " &");
      reset(); _construct<T2>(x); return *this;
    }
    this_type &operator = (const T3 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T3).name() << " &");
      reset(); _construct<T3>(x); return *this;
    }
    this_type &operator = (const T4 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T4).name() << " &");
      reset(); _construct<T4>(x); return *this;
    }
    this_type &operator = (const T5 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T5).name() << " &");
      reset(); _construct<T5>(x); return *this;
    }
    
    operator       T1 &()       { return _element<T1>(); }
    operator const T1 &() const { return _element<T1>(); }
    operator       T2 &()       { return _element<T2>(); }
    operator const T2 &() const { return _element<T2>(); }
    operator       T3 &()       { return _element<T3>(); }
    operator const T3 &() const { return _element<T3>(); }
    operator       T4 &()       { return _element<T4>(); }
    operator const T4 &() const { return _element<T4>(); }
    operator       T5 &()       { return _element<T5>(); }
    operator const T5 &() const { return _element<T5>(); }
    
    void reset() {
      if ( valid != oneofTypeid<this_type,NILTYPE>::type )
        _ONEOFDEBUG("oneof.reset() (T" << static_cast<size_t>(valid) << ")");
      else
        _ONEOFDEBUG("oneof.reset() ()");
      switch( valid ) {
        case oneofTypeid<this_type, T1>::type:
          _destroy<T1>();
          break;
        case oneofTypeid<this_type, T2>::type:
          _destroy<T2>();
          break;
        case oneofTypeid<this_type, T3>::type:
          _destroy<T3>();
          break;
        case oneofTypeid<this_type, T4>::type:
          _destroy<T4>();
          break;
        case oneofTypeid<this_type, T5>::type:
          _destroy<T5>();
          break;
        default:
          assert( isType<NILTYPE>(*this) );
      }
    }
    
    oneof_typeid type() const { return valid; }
    
    ~oneof() { reset(); }
};

template <typename V, typename T1, typename T2, typename T3, typename T4, typename T5>
static inline
typename V::result_type applyVisitor(const oneof<T1,T2,T3,T4,T5> &of, V &v) {
  typedef oneof<T1,T2,T3,T4,T5> this_type;
  switch( of.type() ) {
    case oneofTypeid<this_type, T1>::type:
      return detail::CallVisitor<T1>::apply(of, v);
    case oneofTypeid<this_type, T2>::type:
      return detail::CallVisitor<T2>::apply(of, v);
    case oneofTypeid<this_type, T3>::type:
      return detail::CallVisitor<T3>::apply(of, v);
    case oneofTypeid<this_type, T4>::type:
      return detail::CallVisitor<T4>::apply(of, v);
    case oneofTypeid<this_type, T5>::type:
      return detail::CallVisitor<T5>::apply(of, v);
    default:
      return detail::CallVisitor<NILTYPE>::apply(v);
  }
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
static inline
std::ostream &operator << (std::ostream &output, const oneof<T1,T2,T3,T4,T5> &of)
  {
    detail::OutputVisitor ov(output);
    applyVisitor(of, ov);
    return output;
  }

} // namespace CoSupport

#endif // _INCLUDED_ONEOF_HPP
