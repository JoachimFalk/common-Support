/* vim: set sw=2 ts=8: */
/*
 * Copyright (C) Joachim Falk <joachim.falk@gmx.de> $Date: 2001/08/28 12:01:41 $
 *
 * refcount.h is part of the jf-libs distribution of Joachim Falk;
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * The jf-libs distributio is distributed in the hope that it will be useful,
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

#ifndef _INCLUDED_SHARED_COPYONWRITE_PTR_H
#define _INCLUDED_SHARED_COPYONWRITE_PTR_H

#include <boost/shared_ptr.hpp>

namespace jflibs {

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

}

#endif /* _INCLUDED_SHARED_COPYONWRITE_PTR_H */
