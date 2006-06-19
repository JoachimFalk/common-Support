// vim: set sw=2 ts=8:

#ifndef _INCLUDED_COSUPPORT_PAR_ACTOR_FACTORY_HPP
#define _INCLUDED_COSUPPORT_PAR_ACTOR_FACTORY_HPP

#include "string_convert.hpp"

namespace CoSupport {
  
/**
 * simple helper class that converts the instance index into a unique
 * name, prefixed with a user-defined string (construct_helper must
 * be implemented by the user). This class is indented to be used as
 * a factory by checked_vector
 */
template<class T>
class par_actor_factory
{
private:
  std::string _prefix;

public:
  par_actor_factory(const std::string& prefix) :
    _prefix(prefix)
  {}

  void construct(T* p, size_t instance)
  { construct_helper(p, _prefix + string_cast(instance)); }

  virtual void construct_helper(T* p, const std::string& name) = 0;

  virtual ~par_actor_factory()
  {}

  const std::string& prefix() const
  { return _prefix; }
};

}

#endif // _INCLUDED_COSUPPORT_PAR_ACTOR_FACTORY_HPP
