#ifndef _INCLUDED_COSUPPORT_REFCOUNT_HPP
#define _INCLUDED_COSUPPORT_REFCOUNT_HPP

#include <boost/detail/lightweight_mutex.hpp>

namespace CoSupport {

  class RefCount {
  private:
    typedef boost::detail::lightweight_mutex mutex_type;
    
#if defined(BOOST_HAS_THREADS)
    mutable mutex_type mtx_;
#endif
    /* how many references are there */
    long use_count_;
  public:
    RefCount()
      : use_count_(0) {}
    /* each copy start refcounting anew */
    RefCount(const RefCount &x)
      : use_count_(0) {}
    
    ~RefCount() // nothrow
      {}
    
    void add_ref( void ) {
#if defined(BOOST_HAS_THREADS)
      mutex_type::scoped_lock lock(mtx_);
#endif
      ++use_count_;
    }
    
    bool del_ref( void ) {
#if defined(BOOST_HAS_THREADS)
      mutex_type::scoped_lock lock(mtx_);
#endif
      --use_count_;
      return use_count_ == 0;
    }
    
    bool unique_ref() const { // nothrow
#if defined(BOOST_HAS_THREADS)
      mutex_type::scoped_lock lock(mtx_);
#endif
      return use_count_ == 1;
    }
  };

} // namespace CoSupport 

#endif // _INCLUDED_COSUPPORT_REFCOUNT_HPP
