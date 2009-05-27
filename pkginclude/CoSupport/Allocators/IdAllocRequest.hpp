// vim: set sw=2 ts=8 sts=2 et:

#ifndef _INCLUDED_COSUPPORT_ALLOCATORS_IDALLOCREQUEST_HPP
#define _INCLUDED_COSUPPORT_ALLOCATORS_IDALLOCREQUEST_HPP

#include <string>
#include <assert.h>

#include "../commondefs.h"
#include "../Math/string_hash.hpp"

namespace CoSupport { namespace Allocators {

namespace Detail {

  template <typename T>
  struct IdRange {
    static const T   min  = MIN_TYPE(T);
    static const T   max  = MAX_TYPE(T);
    static const int bits = BITS_TYPE(T);
  };

} // namespace Detail

template <typename ID> 
class IdAllocRequest {
  typedef IdAllocRequest<ID> this_type;
public:
  typedef enum { Force, Suggest, Auto, Anon } AllocType;
protected:
  ID         _id;
  AllocType  _allocType;
public:
  IdAllocRequest(AllocType a = Auto)
    : _id(0), _allocType(a) {
    assert(a == Auto || a == Anon);
  }
  IdAllocRequest(const ID id, AllocType a = Force)
    : _id(id), _allocType(a) {
    assert(a == Force || a == Suggest);
  }

  ID        id() const { return _id; }
  AllocType allocType() const { return _allocType; }

  // Derive id from name if allocType == Auto
  template <class IDRANGE>
  this_type &idFromName(const std::string &name) {
    if (_allocType == Auto) {
      CoSupport::FNV<IDRANGE::bits> hf;
      _id = IDRANGE::min + hf(name.c_str());
      assert(_id <= IDRANGE::max);
      _allocType = Suggest;
    }
    return *this;
  }
  this_type &idFromName(const std::string &name)
    { return idFromName<Detail::IdRange<ID> >(name); }
};

} } // namespace CoSupport::Allocators

#endif // _INCLUDED_COSUPPORT_ALLOCATORS_IDALLOCREQUEST_HPP
