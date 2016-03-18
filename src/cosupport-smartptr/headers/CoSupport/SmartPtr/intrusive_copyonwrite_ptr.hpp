/* vim: set sw=2 ts=8: */
/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifndef _INCLUDED_COSUPPORT_SMARTPTR_INTRUSIVE_COPYONWRITE_PTR_HPP
#define _INCLUDED_COSUPPORT_SMARTPTR_INTRUSIVE_COPYONWRITE_PTR_HPP

#include <CoSupport/compatibility-glue/nullptr.h>
#include "RefCount.hpp"

#include <boost/intrusive_ptr.hpp>

namespace CoSupport { namespace SmartPtr {

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
    return sptr.get() == nullptr ? 0: &this_type::swap;
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
  typedef ::CoSupport::SmartPtr::intrusive_copyonwrite_ptr<T> PST;\
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
    (::CoSupport::SmartPtr::intrusive_copyonwrite_ptr<T> &lptr) { \
    const ::CoSupport::SmartPtr::intrusive_copyonwrite_ptr<T>     \
      &rptr = lptr;                                               \
    if ( !rptr->unique_ref() )                                    \
      lptr = new T(*rptr);                                        \
  }
  
} } // namesapce CoSupport::SmartPtr

#endif // _INCLUDED_COSUPPORT_SMARTPTR_INTRUSIVE_COPYONWRITE_PTR_HPP
