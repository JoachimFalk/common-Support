// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#ifndef _INCLUDED_COSUPPORT_SMARTPTR_SHARED_COPYONWRITE_PTR_HPP
#define _INCLUDED_COSUPPORT_SMARTPTR_SHARED_COPYONWRITE_PTR_HPP

#include <boost/shared_ptr.hpp>

namespace CoSupport { namespace SmartPtr {

template<class T>
class shared_copyonwrite_ptr
: public boost::shared_ptr<T> {
public:
  shared_copyonwrite_ptr() {}
  
  template<class Y>
  explicit shared_copyonwrite_ptr(Y * p)
    : boost::shared_ptr<T>(p) {}
  
  template<class Y, class D>
  shared_copyonwrite_ptr(Y * p, D d)
    : boost::shared_ptr<T>(p,d) {}
  
/*  shared_copyonwrite_ptr( shared_copyonwrite_ptr const & rh )
    : boost::shared_ptr<T>( rh.get() ) {}*/
  
  const T * get() const { return boost::shared_ptr<T>::get(); }
  const T & operator*() const { return *get(); }
  const T * operator->() const { return get(); }

  T * get() {
    if ( !unique() )
      copyonwrite_ptr_copy
        (boost::shared_ptr<T>::get()).swap(*this);
    return boost::shared_ptr<T>::get();
  }
  T & operator*() { return *get(); }
  T * operator->() { return get(); }
};

} } // namespace CoSupport::SmartPtr

#endif // _INCLUDED_COSUPPORT_SMARTPTR_SHARED_COPYONWRITE_PTR_HPP
