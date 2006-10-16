/* vim: set sw=2 ts=8: */
/*
 * Copyright (C) Joachim Falk <joachim.falk@gmx.de> $Date: 2001/08/28 12:01:41 $
 *
 * refcount.h is part of the cosupport distribution of Joachim Falk;
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * The cosupport distributio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
/*
 * $Log$
 */

#ifndef _INCLUDED_COSUPPORT_INTRUSIVE_COPYONWRITE_PTR_HPP
#define _INCLUDED_COSUPPORT_INTRUSIVE_COPYONWRITE_PTR_HPP

#include "refcount.hpp"

#include <boost/intrusive_ptr.hpp>

namespace CoSupport {

  template<class T>
  class intrusive_copyonwrite_ptr {
  private:
    typedef intrusive_copyonwrite_ptr this_type;
    boost::intrusive_ptr<T> sptr;
  public:
    typedef T element_type;
    
    intrusive_copyonwrite_ptr() {}
    
    intrusive_copyonwrite_ptr(T * p, bool add_ref = true)
      : sptr( p, add_ref ) {}
    
#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)
    template<class U> intrusive_copyonwrite_ptr(intrusive_copyonwrite_ptr<U> const & rhs)
      : sptr( rhs.sptr ) {}
#endif
    intrusive_copyonwrite_ptr( this_type const & rh )
      : sptr( rh.sptr ) {}
    
    void swap(this_type & rhs) { sptr.swap( rhs.sptr ); }
    
    typedef void (this_type::*unspecified_bool_type) ( this_type & );
    
    operator unspecified_bool_type () const {
      return sptr.get() == NULL ? 0: &this_type::swap;
    }
    
    // operator! is a Borland-specific workaround
    bool operator! () const { return !sptr; }
    
    const T * get() const { return sptr.get(); }
    const T & operator*() const { return *get(); }
    const T * operator->() const { return get(); }
    
    T * get() {
      intrusive_ptr_mkunique( *this );
      return sptr.get();
    }
    T & operator*() { return *get(); }
    T * operator->() { return get(); }
  };

#define DECL_INTRUSIVE_COPYONWRITE_PTR(T,PST)	                  \
  typedef ::CoSupport::intrusive_copyonwrite_ptr<T> PST;	  \
  void intrusive_ptr_add_ref(T *);		                  \
  void intrusive_ptr_release(T *);		                  \
  void intrusive_ptr_mkunique(PST &)

#define IMPL_INTRUSIVE_COPYONWRITE_PTR(T)                         \
  void intrusive_ptr_add_ref(T *p) {		                  \
    p->add_ref();                                                 \
  }                                                               \
  void intrusive_ptr_release(T *p) {		                  \
    if (p->del_ref())				                  \
      delete p;					                  \
  }                                                               \
  void intrusive_ptr_mkunique                                     \
    ( ::CoSupport::intrusive_copyonwrite_ptr<T> &lptr ) {         \
    const ::CoSupport::intrusive_copyonwrite_ptr<T> &rptr = lptr; \
    if ( !rptr->unique_ref() )                                    \
      lptr = new T(*rptr);                                        \
  }
  
} // namesapce CoSupport

#endif // _INCLUDED_COSUPPORT_INTRUSIVE_COPYONWRITE_PTR_HPP
