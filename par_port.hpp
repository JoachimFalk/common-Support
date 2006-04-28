#ifndef PAR_PORT_HPP
#define PAR_PORT_HPP

#include "checked_vector.hpp"

namespace CoSupport {
  
/**
 * class for the convenient management of Ports
 * used by dispatchers
 */
template<class T>
class par_port
{
private:
  checked_vector<T> inst;
public:
  par_port(size_t count) :
    inst(count)
  {}
  
  T& operator()(size_t i)
  { return inst[i]; }
  
  size_t count() const
  { return inst.size(); }
};

/**
 * convenience structs. they remain here until template typedefs
 * are in the standard! (include smoc_port.hpp to use them)
 */
template<class T>
struct par_port_out { typedef par_port< smoc_port_out<T> > ty; };
template<class T>
struct par_port_in { typedef par_port< smoc_port_in<T> > ty; };

}

#endif // PAR_PORT_HPP
