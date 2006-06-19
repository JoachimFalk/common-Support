// vim: set sw=2 ts=8:

#ifndef _INCLUDED_COSUPPORT_PAR_ACTOR_HPP
#define _INCLUDED_COSUPPORT_PAR_ACTOR_HPP

#include "checked_vector.hpp"
#include "par_manager.hpp"

namespace CoSupport {
  
/** 
 * class for the convenient management of parallel actors
 */
template<class T>
class par_actor
{
private:
  checked_vector<T> inst;

public:
  template<class F>
  par_actor(F factory) :
    inst(par_manager::instance().count(factory.prefix()), factory)
  {}
  
  T& operator()(size_t i)
  { return inst[i]; }
  
  const T& operator()(size_t i) const
  { return inst[i]; }
  
  size_t count() const
  { return inst.size(); } 
};

}

#endif // _INCLUDED_COSUPPORT_PAR_ACTOR_HPP
