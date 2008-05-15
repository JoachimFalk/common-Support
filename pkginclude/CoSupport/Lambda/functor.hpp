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

#ifndef _INCLUDED_COSUPPORT_LAMBDA_FUNCTOR_HPP
#define _INCLUDED_COSUPPORT_LAMBDA_FUNCTOR_HPP

#include <cassert>

namespace CoSupport { namespace Lambda {

namespace Detail {
  //
  // ParamNode: Used to store bound parameters
  // 
  template<class P, class PN = void>
  struct ParamNode
  {
    typedef P  ListHead;
    typedef PN ListTail;
    
    P p;
    PN pn;
    
    ParamNode(P _p, const PN &_pn) : p(_p), pn(_pn) {}
  };

  template<>
  struct ParamNode<void,void>
  {};

  //
  // MissingNode: Used to store type information for missing parameters
  // 
  template<class M, class ML = void>
  struct MissingNode
  {
    typedef M  ListHead;
    typedef ML ListTail;
  };

  template<>
  struct MissingNode<void,void>
  {};

  // Convert MissingList type to corresponding ParamList type, e.g.,
  // MissingToParamList<MissingList>::ParamList
  template<class ML, class PL = ParamNode<void> >
  struct MissingToParamList;

  template<class ML, class PL>
  struct MissingToParamList
  {
    typedef typename MissingToParamList<
      typename ML::ListTail,
      ParamNode<typename ML::ListHead, PL> >::ParamList ParamList;
  };

  template<class PL>
  struct MissingToParamList<MissingNode<void>, PL>
  {
    typedef PL					      ParamList;
  };
} // namespace Detail

//
// ParamAccumulator: binds parameter via operator()
// 
template<template <class,class> class A, class F, class ML, class PL>
struct ParamAccumulator;

template<template <class,class> class A, class F, class ML = typename F::MissingList, class PL = Detail::ParamNode<void> >
struct ParamAccumulator
{
  typedef ParamAccumulator<A, F, ML, PL>    this_type;
  typedef this_type                         accumulated_type;
  
  typedef ParamAccumulator<A, F,
    typename ML::ListTail,
    Detail::ParamNode<typename ML::ListHead, PL> >  accumulated_next_type;
  
  typedef typename accumulated_next_type::accumulated_type  return_type;
  
  F  f;
  PL pl;
  
  ParamAccumulator(const F& _f, const PL &_pl = PL()) : f(_f), pl(_pl) {}
  
  return_type operator()(typename ML::ListHead p) {
    return return_type(f, Detail::ParamNode<typename ML::ListHead, PL>(p, pl));
  }
};

template<template <class,class> class A, class F, class PL>
struct ParamAccumulator<A, F, Detail::MissingNode<void>, PL>
{
  typedef ParamAccumulator<A, F, Detail::MissingNode<void>, PL> this_type;
  typedef typename A<F,PL>::type			        accumulated_type;
};

//
// FUNCTOR-macro: Creates Function-Objects
// 
#define CONSTRUCT(NAME, TLIST, PLIST, CONST, PCALL, MISSING)			\
template<class R, class T TLIST>						\
struct NAME<R, R (T::*)(PLIST) CONST> {						\
	typedef NAME<R, R (T::*)(PLIST) CONST>			    this_type;	\
	typedef MISSING						    MissingList;\
	typedef R						    return_type;\
	typedef typename Detail::MissingToParamList                             \
          <MissingList>::ParamList                                  ParamList;  \
										\
	CONST T *obj;								\
	R (T::*func)(PLIST) CONST;						\
	const char *name;							\
										\
	template<class X>							\
	NAME(CONST X *_obj, R (T::*_func)(PLIST) CONST, const char *_name)	\
	  : obj(dynamic_cast<CONST T *>(_obj)), func(_func), name(_name)	\
	  { assert(obj != 0 && func != 0); }					\
										\
	R call(const ParamList &pl) const {					\
		return (obj->*func)(PCALL);					\
	}									\
};

#define MISSING_0 Detail::MissingNode<void>
#define MISSING_1 Detail::MissingNode<P1 , MISSING_0 >
#define MISSING_2 Detail::MissingNode<P2 , MISSING_1 >
#define MISSING_3 Detail::MissingNode<P3 , MISSING_2 >
#define MISSING_4 Detail::MissingNode<P4 , MISSING_3 >

#define TEMPLATELIST_0 
#define TEMPLATELIST_1 , typename P1 TEMPLATELIST_0
#define TEMPLATELIST_2 , typename P2 TEMPLATELIST_1
#define TEMPLATELIST_3 , typename P3 TEMPLATELIST_2
#define TEMPLATELIST_4 , typename P4 TEMPLATELIST_3

#define PARAMLIST_0
#define PARAMLIST_1 P1
#define PARAMLIST_2 P2 , PARAMLIST_1
#define PARAMLIST_3 P3 , PARAMLIST_2
#define PARAMLIST_4 P4 , PARAMLIST_3

#define PARAMCALL_0
#define PARAMCALL_1 pl.p
#define PARAMCALL_2 pl.pn.p , PARAMCALL_1
#define PARAMCALL_3 pl.pn.pn.p , PARAMCALL_2
#define PARAMCALL_4 pl.pn.pn.pn.p , PARAMCALL_3

template<class R, class F>
struct Functor;

CONSTRUCT(     Functor, TEMPLATELIST_0, PARAMLIST_0,      , PARAMCALL_0, MISSING_0)
CONSTRUCT(     Functor, TEMPLATELIST_1, PARAMLIST_1,      , PARAMCALL_1, MISSING_1)
CONSTRUCT(     Functor, TEMPLATELIST_2, PARAMLIST_2,      , PARAMCALL_2, MISSING_2)
CONSTRUCT(     Functor, TEMPLATELIST_3, PARAMLIST_3,      , PARAMCALL_3, MISSING_3)
CONSTRUCT(     Functor, TEMPLATELIST_4, PARAMLIST_4,      , PARAMCALL_4, MISSING_4)
CONSTRUCT(     Functor, TEMPLATELIST_0, PARAMLIST_0, const, PARAMCALL_0, MISSING_0)
CONSTRUCT(     Functor, TEMPLATELIST_1, PARAMLIST_1, const, PARAMCALL_1, MISSING_1)
CONSTRUCT(     Functor, TEMPLATELIST_2, PARAMLIST_2, const, PARAMCALL_2, MISSING_2)
CONSTRUCT(     Functor, TEMPLATELIST_3, PARAMLIST_3, const, PARAMCALL_3, MISSING_3)
CONSTRUCT(     Functor, TEMPLATELIST_4, PARAMLIST_4, const, PARAMCALL_4, MISSING_4)
  
template<class R, class F>
struct ConstFunctor;

CONSTRUCT(ConstFunctor, TEMPLATELIST_0, PARAMLIST_0, const, PARAMCALL_0, MISSING_0)
CONSTRUCT(ConstFunctor, TEMPLATELIST_1, PARAMLIST_1, const, PARAMCALL_1, MISSING_1)
CONSTRUCT(ConstFunctor, TEMPLATELIST_2, PARAMLIST_2, const, PARAMCALL_2, MISSING_2)
CONSTRUCT(ConstFunctor, TEMPLATELIST_3, PARAMLIST_3, const, PARAMCALL_3, MISSING_3)
CONSTRUCT(ConstFunctor, TEMPLATELIST_4, PARAMLIST_4, const, PARAMCALL_4, MISSING_4)
  
#undef CONSTRUCT

#undef MISSING_0
#undef MISSING_1
#undef MISSING_2
#undef MISSING_3
#undef MISSING_4

#undef TEMPLATELIST_0
#undef TEMPLATELIST_1
#undef TEMPLATELIST_2
#undef TEMPLATELIST_3
#undef TEMPLATELIST_4

#undef PARAMLIST_0
#undef PARAMLIST_1
#undef PARAMLIST_2
#undef PARAMLIST_3
#undef PARAMLIST_4

#undef PARAMCALL_0
#undef PARAMCALL_1
#undef PARAMCALL_2
#undef PARAMCALL_3
#undef PARAMCALL_4
  
} } // namespace CoSupport::Lambda

#endif // _INCLUDED_COSUPPORT_LAMBDA_FUNCTOR_HPP
