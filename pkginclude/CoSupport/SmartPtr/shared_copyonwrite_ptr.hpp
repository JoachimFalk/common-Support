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
