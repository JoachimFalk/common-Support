#ifndef _FUNCTOR_HPP_
#define _FUNCTOR_HPP_

#include <cassert>

namespace CoSupport {

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
  
  ParamNode(const P &_p, const PN &_pn) : p(_p), pn(_pn) {}
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

//
// ParamAccumulator: binds parameter via operator()
// 
template<template <class,class> class A, class F, class ML, class PL>
struct ParamAccumulator;

template<template <class,class> class A, class F, class ML = typename F::MissingList, class PL = ParamNode<void> >
struct ParamAccumulator
{
  typedef ParamAccumulator<A, F, ML, PL>    this_type;
  typedef this_type                         accumulated_type;
  
  typedef ParamAccumulator<A, F,
    typename ML::ListTail,
    ParamNode<typename ML::ListHead, PL> >  accumulated_next_type;
  
  typedef typename accumulated_next_type::accumulated_type  return_type;
  
  F  f;
  PL pl;
  
  ParamAccumulator(const F& _f, const PL &_pl = PL()) : f(_f), pl(_pl) {}
  
  return_type operator()(const typename ML::ListHead &p) {
    return return_type(f, ParamNode<typename ML::ListHead, PL>(p, pl));
  }
};

template<template <class,class> class A, class F, class PL>
struct ParamAccumulator<A, F, MissingNode<void>, PL>
{
  typedef ParamAccumulator<A, F, MissingNode<void>, PL> this_type;
  typedef typename A<F,PL>::type			accumulated_type;
};

//
// FUNCTOR-macro: Creates Function-Objects
// 
#define FUNCTOR(NAME, TLIST, PLIST, CONST, PCALL, MISSING)			\
template<class R, class T TLIST>						\
struct NAME<R, R (T::*)PLIST CONST> {						\
	typedef NAME<R, R (T::*)PLIST CONST>			    this_type;	\
	typedef MISSING						    MissingList;\
	typedef R						    return_type;\
	typedef typename MissingToParamList<MissingList>::ParamList ParamList;	\
										\
	CONST T *obj;								\
	R (T::*func)PLIST CONST;						\
	const char *name;							\
										\
	template<class X>							\
	NAME(CONST X *_obj, R (T::*_func)PLIST CONST, const char *_name)	\
	  : obj(dynamic_cast<CONST T *>(_obj)), func(_func), name(_name)	\
	  { assert(obj != 0 && func != 0); }					\
										\
	R call(const ParamList &pl) const {					\
		return (obj->*func)PCALL;					\
	}									\
};

#define _CT_ , typename
#define _MISSING_0_ MissingNode<void>
#define _MISSING_1_ MissingNode<P1, MissingNode<void> >
#define _MISSING_2_ MissingNode<P1, MissingNode<P2, MissingNode<void> > >

template<class R, class F>
struct Functor;

FUNCTOR(     Functor,                ,       (),      ,              (), _MISSING_0_)
FUNCTOR(     Functor,         _CT_ P1,     (P1),      ,          (pl.p), _MISSING_1_)
FUNCTOR(     Functor, _CT_ P1 _CT_ P2, (P1, P2),      , (pl.pn.p, pl.p), _MISSING_2_)	

template<class R, class F>
struct ConstFunctor;

FUNCTOR(ConstFunctor,                ,       (), const,              (), _MISSING_0_)
FUNCTOR(ConstFunctor,         _CT_ P1,     (P1), const,          (pl.p), _MISSING_1_)
FUNCTOR(ConstFunctor, _CT_ P1 _CT_ P2, (P1, P2), const, (pl.pn.p, pl.p), _MISSING_2_)

#undef _MISSING_2_
#undef _MISSING_1_
#undef _MISSING_0_
#undef _CT_
#undef FUNCTOR

} // namespace CoSupport

#endif // _FUNCTOR_HPP_
