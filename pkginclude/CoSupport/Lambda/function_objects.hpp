// vim: set sw=2 ts=8:
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

#ifndef _INCLUDED_COSUPPORT_FUNCTION_OBJECTS_HPP
#define _INCLUDED_COSUPPORT_FUNCTION_OBJECTS_HPP

#include <functional>
#include <iostream>
#include <cassert>

namespace CoSupport
{
/**
 * h(x) = f(g(x))
 */
template<class F, class G>
struct composition_f_g_x :
  public std::unary_function<
    typename G::argument_type,
    typename F::result_type>
{
  typedef typename G::argument_type _A;
  typedef typename F::result_type   _R;

  F f;
  G g;

  composition_f_g_x(const F& f, const G& g) :
    f(f), g(g)
  {}

  _R operator()(const _A& x) const
  { return f(g(x)); }

  _R operator()(const _A& x)
  { return f(g(x)); }
};

template<class F, class G>
composition_f_g_x<F,G> compose_f_g_x(const F &f, const G &g)
{ return composition_f_g_x<F,G>(f, g); }


/**
 * h(x,y) = f(g(x,y))
 */
template<class F, class G>
struct composition_f_g_x_y :
  public std::binary_function<
    typename G::first_argument_type,
    typename G::second_argument_type,
    typename F::result_type>
{
  typedef typename G::first_argument_type   _A;
  typedef typename G::second_argument_type  _B;
  typedef typename F::result_type           _R;

  F f;
  G g;

  composition_f_g_x_y(const F& f, const G& g) :
    f(f), g(g)
  {}

  _R operator()(const _A& a, const _B& b) const
  { return f(g(a,b)); }

  _R operator()(const _A& a, const _B& b)
  { return f(g(a,b)); } 
};

template<class F, class G>
composition_f_g_x_y<F,G> compose_f_g_x_y(const F& f, const G& g)
{ return composition_f_g_x_y<F,G>(f, g); }


/**
 * h(x) = f(g1(x), g2(x))
 */
template<class F, class G1, class G2>
struct composition_f_g1_x_g2_x :
  public std::unary_function<
    typename G1::argument_type,
    typename F::result_type>
{
  typedef typename G1::argument_type  _A;
  typedef typename F::result_type     _R;
  
  F f;
  G1 g1;
  G2 g2;
  
  composition_f_g1_x_g2_x(const F& f, const G1& g1, const G2& g2) :
    f(f), g1(g1), g2(g2)
  {}
  
  _R operator()(const _A& a) const
  { return f(g1(a), g2(a)); }

  _R operator()(const _A& a)
  { return f(g1(a), g2(a)); }
};

template<class F, class G1, class G2>
composition_f_g1_x_g2_x<F,G1,G2> compose_f_g1_x_g2_x(const F& f, const G1& g1, const G2& g2)
{ return composition_f_g1_x_g2_x<F,G1,G2>(f, g1, g2); }


/**
 * h(x,y) = f(g1(x), g2(y))
 */
template<class F, class G1, class G2>
struct composition_f_g1_x_g2_y :
  public std::binary_function<
    typename G1::argument_type,
    typename G2::argument_type,
    typename F::result_type>
{
  typedef typename G1::argument_type  _A;
  typedef typename G2::argument_type  _B;
  typedef typename F::result_type     _R;

  F f;
  G1 g1;
  G2 g2;

  composition_f_g1_x_g2_y(const F& f, const G1& g1, const G2& g2) :
    f(f), g1(g1), g2(g2)
  {}

  _R operator()(const _A& a, const _B& b) const
  { return f(g1(a), g2(b)); }

  _R operator()(const _A& a, const _B& b)
  { return f(g1(a), g2(b)); }
};

template<class F, class G1, class G2>
composition_f_g1_x_g2_y<F,G1,G2> compose_f_g1_x_g2_y(const F& f, const G1& g1, const G2& g2)
{ return composition_f_g1_x_g2_y<F,G1,G2>(f, g1, g2); }


/**
 * print to std::cout (unary)
 */
template<class A>
struct print_unary :
  public std::unary_function<A, void>
{
  void operator()(const A& a) const
  { std::cout << a << std::endl; }
};


/**
 * print to std::cout (binary)
 */
template<class A, class B>
struct print_binary :
  public std::binary_function<A, B, void>
{
  void operator()(const A& a, const B& b) const
  { std::cout << a << ", " << b << std::endl; }
};


/**
 * return argument
 */
template<class A>
struct identity :
  public std::unary_function<A, A>
{
  A& operator()(A& a) const
  { return a; }

  const A& operator()(const A& a) const
  { return a; }
};


/**
 * remember max. argument
 */
template<class A>
struct max_arg :
  public std::unary_function<A, void>
{
  A value;
  bool found;
  
  max_arg() :
    found(false)
  {}

  void operator()(const A& a)
  {
    if(!found || a > value) {
      found = true;
      value = a;
    }
  }
};

/**
 * remember min. argument
 */
template<class A>
struct min_arg :
  public std::unary_function<A, void>
{
  A value;
  bool found;
  
  min_arg() :
    found(false)
  {}

  void operator()(const A& a)
  {
    if(!found || a < value) {
      found = true;
      value = a;
    }
  }
};

/**
 * pointer-to-member function object
 */
template<class C, class T>
struct mem_ptr_t :
  public std::unary_function<C,T>
{
  T C::*ptm;
  
  mem_ptr_t(T C::*ptm) :
    ptm(ptm)
  {}
  
  const T& operator()(const C& c) const
  { return c.*ptm; }
};

template<class C, class T>
mem_ptr_t<C,T> mem_ptr(T C::*ptm)
{ return mem_ptr_t<C,T>(ptm); }

/**
 * for_each with binary function
 */
template<
  class InputIterator1, class InputIterator2,
  class BinaryFunction>
BinaryFunction for_each(
    InputIterator1 firstA, InputIterator1 lastA,
    InputIterator2 firstB,
    BinaryFunction f)
{
  for(; firstA != lastA; ++firstA, ++firstB) {
    f(*firstA, *firstB);
  }
  return f;
}


/**
 * copy arguments from different input iterators
 * depending on binary predicate function
 */
template<
  class InputIterator1, class InputIterator2,
  class OutputIterator, class BinaryFunction>
void choose(
    InputIterator1 firstA, InputIterator1 lastA,
    InputIterator2 firstB, OutputIterator result,
    BinaryFunction f)
{
  for(; firstA != lastA; ++firstA, ++firstB, ++result) {
    if(f(*firstA, *firstB))
      *result = *firstA;
    else
      *result = *firstB;
  }
}


/**
 * does an element exist for which the predicate evaluates to true?
 */
template<
  class InputIterator,
  class UnaryFunction>
bool exists(InputIterator first, InputIterator last, UnaryFunction f)
{
  for(; first != last; ++first) {
    if(f(*first))
      return true;
  }
  return false;
} 


/**
 * does a pair of elements exist for which the predicate evaluates to true?
 */
template<
  class InputIterator1, class InputIterator2,
  class BinaryFunction>
bool exists(
    InputIterator1 firstA, InputIterator1 lastA,
    InputIterator2 firstB, BinaryFunction f)
{
  for(; firstA != lastA; ++firstA, ++firstB) {
    if(f(*firstA, *firstB))
      return true;
  }
  return false;
} 


/**
 * does the predicate evaluate to true for all elements?
 */
template<
  class InputIterator,
  class UnaryFunction>
bool all(
    InputIterator first, InputIterator last,
    UnaryFunction f)
{
  for(; first != last; ++first) {
    if(!f(*first))
      return false;
  }
  return true;
} 


/**
 * does the predicate evaluate to true for all pairs of elements?
 */
template<
  class InputIterator1, class InputIterator2,
  class BinaryFunction>
bool all(
    InputIterator1 firstA, InputIterator1 lastA,
    InputIterator2 firstB, BinaryFunction f)
{
  for(; firstA != lastA; ++firstA, ++firstB) {
    if(!f(*firstA, *firstB))
      return false;
  }
  return true;
}


/**
 * select elements from container based on projection indices
 */
template<
  class RandomAccessContainer,
  class InputIterator,
  class OutputIterator>
void project(
    const RandomAccessContainer& source,
    InputIterator first, InputIterator last,
    OutputIterator result)
{
  for(; first != last; ++first, ++result) {
    *result = source[*first];
  }
}

} // namespace CoSupport

#endif // _INCLUDED_COSUPPORT_FUNCTION_OBJECTS_HPP
