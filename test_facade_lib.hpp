#ifndef _INCLUDED_TEST_FACADE_LIB_HPP
#define _INCLUDED_TEST_FACADE_LIB_HPP

#include <CoSupport/DataTypes/Facade.hpp>
#include <CoSupport/SmartPtr/intrusive_refcount_ptr.hpp>

namespace Something {

namespace Detail {

  class AImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(AImpl, PAImpl);

  class BImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(BImpl, PBImpl);

  class CImpl;
  DECL_INTRUSIVE_REFCOUNT_PTR(CImpl, PCImpl);

} // namespace Detail

class A
: public CoSupport::DataTypes::FacadeFoundation<
    A,
    Detail::AImpl>
{
  typedef A this_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType>;
protected:
  A(const SmartPtr &p);
public:
  A();
  A(const this_type &);
};

class B
: public CoSupport::DataTypes::FacadeFoundation<
    B,
    Detail::BImpl,
    A,
    Detail::PBImpl>
{
  typedef B this_type;
  typedef A base_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType, base_type, SmartPtr>;
protected:
  ImplType *getImpl() const;

  B(const SmartPtr &p);
public:
  B();
  B(const this_type &);

  static Ptr upcast(const A &);
};

class C
: public CoSupport::DataTypes::FacadeFoundation<
    C,
    Detail::CImpl,
    A,
    Detail::PCImpl>
{
  typedef C this_type;
  typedef A base_type;

  friend class CoSupport::DataTypes::FacadeFoundation<this_type, ImplType, base_type, SmartPtr>;
protected:
  ImplType *getImpl() const;

  C(const SmartPtr &p);
public:
  C();
  C(const this_type &);

  static Ptr upcast(const A &);
};

} // namespace Something

#endif // _INCLUDED_TEST_FACADE_LIB_HPP


