// vim: set sw=2 ts=8:

#ifndef _INCLUDED_ONEOF_HPP
#define _INCLUDED_ONEOF_HPP

#include <assert.h>
#include <memory>
#include <iostream>
#include <typeinfo>

#define _ONEOFDEBUG(x) do {} while (0)
//#define _ONEOFDEBUG(x) std::cerr << x << std::endl

namespace CoSupport {

typedef int oneof_typeid;

struct NILTYPE;

namespace detail {

  struct void2_st {};
  struct void3_st {};
  struct void4_st {};
  struct void5_st {};

  static inline
  std::ostream &operator << (std::ostream &o, const void2_st &) { return o; }

  static inline
  std::ostream &operator << (std::ostream &o, const void3_st &) { return o; }

  static inline
  std::ostream &operator << (std::ostream &o, const void4_st &) { return o; }

  static inline
  std::ostream &operator << (std::ostream &o, const void5_st &) { return o; }

  template <typename T>
  struct CallVisitor {
    template <class V> static
    typename V::result_type apply(const T &obj, const V &v)
      { return v(obj); }
    
    template <class V> static
    typename V::result_type apply(const T &obj, V &v)
      { return v(obj); }
  };
  
  template <>
  struct CallVisitor<void2_st> {
    template <class V> static
    typename V::result_type apply(const void2_st &obj, const V &v)
      { assert(1 ? 0 : "oneof contains void2_st !"); }
    
    template <class V> static
    typename V::result_type apply(const void2_st &obj, V &v)
      { assert(1 ? 0 : "oneof contains void2_st !"); }
  };
  
  template <>
  struct CallVisitor<void3_st> {
    template <class V> static
    typename V::result_type apply(const void3_st &obj, const V &v)
      { assert(1 ? 0 : "oneof contains void3_st !"); }
    
    template <class V> static
    typename V::result_type apply(const void3_st &obj, V &v)
      { assert(1 ? 0 : "oneof contains void3_st !"); }
  };

  template <>
  struct CallVisitor<void4_st> {
    template <class V> static
    typename V::result_type apply(const void4_st &obj, const V &v)
      { assert(1 ? 0 : "oneof contains void4_st !"); }
    
    template <class V> static
    typename V::result_type apply(const void3_st &obj, V &v)
      { assert(1 ? 0 : "oneof contains void3_st !"); }
  };

  template <>
  struct CallVisitor<void5_st> {
    template <class V> static
    typename V::result_type apply(const void5_st &obj, const V &v)
      { assert(1 ? 0 : "oneof contains void5_st !"); }
    
    template <class V> static  
    typename V::result_type apply(const void5_st &obj, V &v)
      { assert(1 ? 0 : "oneof contains void5_st !"); }
  };

  template <>
  struct CallVisitor<NILTYPE> {
    template <class V> static
    typename V::result_type apply(const V &v)
      { assert(1 ? 0 : "oneof contains NILTYPE !"); }
    
    template <class V> static
    typename V::result_type apply(V &v)
      { assert(1 ? 0 : "oneof contains NILTYPE !"); }
  };

  struct OutputVisitor {
    typedef void result_type;

    std::ostream &output;

    OutputVisitor(std::ostream &output): output(output) {}

    template <typename T>
    void operator()(const T &e) const
      { output << "oneof(" << typeid(T).name() << ":" << e << ")"; }
  };
};

template <
  typename T1,
  typename T2 = detail::void2_st,
  typename T3 = detail::void3_st,
  typename T4 = detail::void4_st,
  typename T5 = detail::void5_st >
class oneof;

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
bool isType( const oneof<T1,T2,T3,T4,T5> &of );

namespace detail {

  template <typename, typename>
  struct oneofTypeid;

  template <typename T, typename T2, typename T3, typename T4, typename T5>
  struct oneofTypeid<oneof<T,T2,T3,T4,T5>,T>
    { static oneof_typeid type() { return 1; } };
  template <typename T, typename T2, typename T3, typename T4, typename T5>
  struct oneofTypeid<const oneof<T,T2,T3,T4,T5>,T>
    { static oneof_typeid type() { return 1; } };

  template <typename T, typename T1, typename T3, typename T4, typename T5>
  struct oneofTypeid<oneof<T1,T,T3,T4,T5>,T>
    { static oneof_typeid type() { return 2; } };
  template <typename T, typename T1, typename T3, typename T4, typename T5>
  struct oneofTypeid<const oneof<T1,T,T3,T4,T5>,T>
    { static oneof_typeid type() { return 2; } };

  template <typename T, typename T1, typename T2, typename T4, typename T5>
  struct oneofTypeid<oneof<T1,T2,T,T4,T5>,T>
    { static oneof_typeid type() { return 3; } };
  template <typename T, typename T1, typename T2, typename T4, typename T5>
  struct oneofTypeid<const oneof<T1,T2,T,T4,T5>,T>
    { static oneof_typeid type() { return 3; } };

  template <typename T, typename T1, typename T2, typename T3, typename T5>
  struct oneofTypeid<oneof<T1,T2,T3,T,T5>,T>
    { static oneof_typeid type() { return 4; } };
  template <typename T, typename T1, typename T2, typename T3, typename T5>
  struct oneofTypeid<const oneof<T1,T2,T3,T,T5>,T>
    { static oneof_typeid type() { return 4; } };

  template <typename T, typename T1, typename T2, typename T3, typename T4>
  struct oneofTypeid<oneof<T1,T2,T3,T4,T>,T>
    { static oneof_typeid type() { return 5; } };
  template <typename T, typename T1, typename T2, typename T3, typename T4>
  struct oneofTypeid<const oneof<T1,T2,T3,T4,T>,T>
    { static oneof_typeid type() { return 5; } };

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  struct oneofTypeid<oneof<T1,T2,T3,T4,T5>,NILTYPE>
    { static oneof_typeid type() { return 0; } };
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  struct oneofTypeid<const oneof<T1,T2,T3,T4,T5>,NILTYPE>
    { static oneof_typeid type() { return 0; } };
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class oneof {
  public:
    typedef oneof<T1,T2,T3,T4,T5> this_type;
  private:
    oneof_typeid valid;
    
    union {
      char e1[sizeof(T1)];
      char e2[sizeof(T2)];
      char e3[sizeof(T3)];
      char e4[sizeof(T4)];
      char e5[sizeof(T5)];
    } mem;
    
    template <typename T>
    void _construct(const T &e) {
      assert( isType<NILTYPE>(*this) );
      valid = detail::oneofTypeid<this_type,T>::type();
      new(reinterpret_cast<T*>(&mem)) T(e);
    }
    template <typename T>
    T &_element() {
      assert(isType<T>(*this));
      return *reinterpret_cast<T*>(&mem);
    }
    template <typename T>
    const T &_element() const {
      assert(isType<T>(*this));
      return *reinterpret_cast<const T*>(&mem);
    }
    template <typename T>
    void _destroy() {
      assert(isType<T>(*this));
      _call_destructor(reinterpret_cast<T*>(&mem));
      valid =  detail::oneofTypeid<this_type,NILTYPE>::type();
    }
    template <class T> void _call_destructor( T  *x ) { x->~T(); }
    template <typename T> void _call_destructor( T ) {}
  public:
    oneof(): valid(detail::oneofTypeid<this_type,NILTYPE>::type())
      { _ONEOFDEBUG("oneof()"); }
    oneof(const this_type &x): valid(detail::oneofTypeid<this_type,NILTYPE>::type()) {
      if ( x.valid != detail::oneofTypeid<this_type,NILTYPE>::type() )
        _ONEOFDEBUG("oneof(const oneof &) (T" << x.valid << ")");
      else
        _ONEOFDEBUG("oneof(const oneof &) ()");
      *this = x;
    }
    oneof(const T1 &e): valid(detail::oneofTypeid<this_type,NILTYPE>::type()) {
      _ONEOFDEBUG("oneof( const " << typeid(T1).name() << " & )");
      _construct<T1>(e);
    }
    oneof(const T2 &e): valid(detail::oneofTypeid<this_type,NILTYPE>::type()) {
      _ONEOFDEBUG("oneof( const " << typeid(T2).name() << " & )");
      _construct<T2>(e);
    }
    oneof(const T3 &e): valid(detail::oneofTypeid<this_type,NILTYPE>::type()) {
      _ONEOFDEBUG("oneof( const " << typeid(T3).name() << " & )");
      _construct<T3>(e);
    }
    oneof(const T4 &e): valid(detail::oneofTypeid<this_type,NILTYPE>::type()) {
      _ONEOFDEBUG("oneof( const " << typeid(T4).name() << " & )");
      _construct<T4>(e);
    }
    oneof(const T5 &e): valid(detail::oneofTypeid<this_type,NILTYPE>::type()) {
      _ONEOFDEBUG("oneof( const " << typeid(T5).name() << " & )");
      _construct<T5>(e);
    }
    
    this_type &operator = (const this_type &x) {
      if ( x.valid != detail::oneofTypeid<this_type,NILTYPE>::type() )
        _ONEOFDEBUG("oneof = const oneof & (T" << x.valid << ")");
      else
        _ONEOFDEBUG("oneof = const oneof & ()");
      if ( isType<T1>(x) ) {
        *this = (x._element<T1>());
      } else if ( isType<T2>(x) ) {
        *this = (x._element<T2>());
      } else if ( isType<T3>(x) ) {
        *this = (x._element<T3>());
      } else if ( isType<T4>(x) ) {
        *this = (x._element<T4>());
      } else if ( isType<T5>(x) ) {
        *this = (x._element<T5>());
      } else {
        assert( isType<NILTYPE>(x) );
        reset();
      }
      return *this;
    }
    this_type &operator = (const T1 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T1).name() << " &");
      reset(); _construct<T1>(x); return *this;
    }
    this_type &operator = (const T2 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T2).name() << " &");
      reset(); _construct<T2>(x); return *this;
    }
    this_type &operator = (const T3 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T3).name() << " &");
      reset(); _construct<T3>(x); return *this;
    }
    this_type &operator = (const T4 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T4).name() << " &");
      reset(); _construct<T4>(x); return *this;
    }
    this_type &operator = (const T5 &x) {
      _ONEOFDEBUG("oneof = const " << typeid(T5).name() << " &");
      reset(); _construct<T5>(x); return *this;
    }
    
    operator       T1 &()       { return _element<T1>(); }
    operator const T1 &() const { return _element<T1>(); }
    operator       T2 &()       { return _element<T2>(); }
    operator const T2 &() const { return _element<T2>(); }
    operator       T3 &()       { return _element<T3>(); }
    operator const T3 &() const { return _element<T3>(); }
    operator       T4 &()       { return _element<T4>(); }
    operator const T4 &() const { return _element<T4>(); }
    operator       T5 &()       { return _element<T5>(); }
    operator const T5 &() const { return _element<T5>(); }
    
    void reset() {
      if ( valid != detail::oneofTypeid<this_type,NILTYPE>::type() )
        _ONEOFDEBUG("oneof.reset() (T" << valid << ")");
      else
        _ONEOFDEBUG("oneof.reset() ()");
      if ( isType<T1>(*this) )
        _destroy<T1>();
      else if ( isType<T2>(*this) )
        _destroy<T2>();
      else if ( isType<T3>(*this) )
        _destroy<T3>();
      else if ( isType<T4>(*this) )
        _destroy<T4>();
      else if ( isType<T5>(*this) )
        _destroy<T5>();
      else
        assert( isType<NILTYPE>(*this) );
    }
    
    oneof_typeid type() const { return valid; }
    
    ~oneof() { reset(); }
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
bool isType( const oneof<T1,T2,T3,T4,T5> &of )
  { return detail::oneofTypeid<oneof<T1,T2,T3,T4,T5>,T>::type() == of.type(); }

template <typename V, typename T1, typename T2, typename T3, typename T4, typename T5>
static inline
typename V::result_type applyVisitor( const oneof<T1,T2,T3,T4,T5> &of, const V &v ) {
  if ( isType<T1>(of) ) {
    return detail::CallVisitor<T1>::apply(static_cast<const T1 &>(of), v);
  } else if ( isType<T2>(of) ) {
    return detail::CallVisitor<T2>::apply(static_cast<const T2 &>(of), v);
  } else if ( isType<T3>(of) ) {
    return detail::CallVisitor<T3>::apply(static_cast<const T3 &>(of), v);
  } else if ( isType<T4>(of) ) {
    return detail::CallVisitor<T4>::apply(static_cast<const T4 &>(of), v);
  } else if ( isType<T5>(of) ) {
    return detail::CallVisitor<T5>::apply(static_cast<const T5 &>(of), v);
  } else {
    return detail::CallVisitor<NILTYPE>::apply(v);
  }
}

template <typename V, typename T1, typename T2, typename T3, typename T4, typename T5>
static inline
typename V::result_type applyVisitor( const oneof<T1,T2,T3,T4,T5> &of )
  { return applyVisitor(of, V()); }

template <typename V, typename T1, typename T2, typename T3, typename T4, typename T5>
static inline
typename V::result_type applyVisitor( const oneof<T1,T2,T3,T4,T5> &of, V &v ) {
  if ( isType<T1>(of) ) {
    return detail::CallVisitor<T1>::apply(static_cast<const T1 &>(of), v);
  } else if ( isType<T2>(of) ) {
    return detail::CallVisitor<T2>::apply(static_cast<const T2 &>(of), v);
  } else if ( isType<T3>(of) ) {
    return detail::CallVisitor<T3>::apply(static_cast<const T3 &>(of), v);
  } else if ( isType<T4>(of) ) {
    return detail::CallVisitor<T4>::apply(static_cast<const T4 &>(of), v);
  } else if ( isType<T5>(of) ) {
    return detail::CallVisitor<T5>::apply(static_cast<const T5 &>(of), v);
  } else {
    return detail::CallVisitor<NILTYPE>::apply(v);
  }
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
static inline
std::ostream &operator << (std::ostream &output, const oneof<T1,T2,T3,T4,T5> &of)
  { applyVisitor(of, detail::OutputVisitor(output)); return output; }

} // namespace CoSupport

#endif // _INCLUDED_ONEOF_HPP
