
#include "test_facade_lib.hpp"

#include <CoSupport/SmartPtr/RefCount.hpp>

namespace Something {

namespace Detail {

  class AImpl: public CoSupport::SmartPtr::RefCount {
    typedef AImpl this_type;
  public:
    virtual this_type *dup() const
      { return new this_type(*this); }

    virtual ~AImpl() {}
  };
  IMPL_INTRUSIVE_REFCOUNT_PTR(AImpl);

  class BImpl: public AImpl {
    typedef BImpl this_type;
  public:
    virtual this_type *dup() const
      { return new this_type(*this); }
  };
  IMPL_INTRUSIVE_REFCOUNT_PTR(BImpl);

  class CImpl: public AImpl {
    typedef CImpl this_type;
  public:
    virtual this_type *dup() const
      { return new this_type(*this); }
  };
  IMPL_INTRUSIVE_REFCOUNT_PTR(CImpl);

} // namespace Detail

A::A()
 : FFType(new ImplType()) {}

A::A(const SmartPtr &p)
 : FFType(p) {}

A::A(const this_type &x)
 : FFType(x.getImpl()->dup()) {}

B::B()
 : FFType(new ImplType()) {}

B::B(const SmartPtr &p)
 : FFType(p) {}

B::B(const this_type &x)
 : FFType(x.getImpl()->dup()) {}

B::ImplType *B::getImpl() const
  { return static_cast<ImplType *>(this->pImpl.get()); }

B::Ptr B::upcast(const A &x)
//{ return ::boost::dynamic_pointer_cast<ImplType>(x.pImpl); }
  { return ::boost::dynamic_pointer_cast<ImplType>(static_cast<const this_type &>(x).pImpl); }

C::C()
 : FFType(new ImplType()) {}

C::C(const SmartPtr &p)
 : FFType(p) {}

C::C(const this_type &x)
 : FFType(x.getImpl()->dup()) {}

C::ImplType *C::getImpl() const
  { return static_cast<ImplType *>(this->pImpl.get()); }

C::Ptr C::upcast(const A &x)
//{ return ::boost::dynamic_pointer_cast<ImplType>(x.pImpl); }
  { return ::boost::dynamic_pointer_cast<ImplType>(static_cast<const this_type &>(x).pImpl); }

} // namespace Something

