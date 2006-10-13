#ifndef _INCLUDED_COSUPPORT_REFCOUNT_OBJECT_HPP
#define _INCLUDED_COSUPPORT_REFCOUNT_OBJECT_HPP

#include "refcount.hpp"
#include "commondefs.h"

namespace CoSupport {

  class RefCountObject: public RefCount {
  public:
    virtual ~RefCountObject();
  };

} // namespace CoSupport 

#ifndef _COMPILEHEADER_INTRUSIVE_PTR_RELEASE_REFCOUNTOBJECT
GNU89_EXTERN_INLINE
#endif
void intrusive_ptr_release(CoSupport::RefCountObject *p) {
  if (p->del_ref())
    // RefCountObject has virtual destructor
    delete p;
}

#endif // _INCLUDED_COSUPPORT_REFCOUNT_OBJECT_HPP
