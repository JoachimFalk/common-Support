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

#ifndef _INCLUDED_COSUPPORT_SMARTPTR_REFCOUNT_HPP
#define _INCLUDED_COSUPPORT_SMARTPTR_REFCOUNT_HPP

#include <CoSupport/commondefs.h>
#include <CoSupport/sassert.h>
#include <cassert>

#if defined(_REENTRANT)
// boost/thread/mutex.hpp seems to define _REENTRANT
# include <boost/thread/mutex.hpp>
#endif
#include <boost/noncopyable.hpp>

namespace CoSupport { namespace SmartPtr {

  class RefCount {
  private:
#if defined(_REENTRANT)
    typedef boost::mutex mutex_type;

    mutable mutex_type mtx_;
#endif

    /* how many references are there */
    unsigned long use_count_;
  public:
    RefCount()
      : use_count_(0) {}
    /* each copy start refcounting anew */
    RefCount(const RefCount &x)
      : use_count_(0) {}

    /* Do not overwrite reference counter ! */
    RefCount &operator = (const RefCount &) {
      return *this;
    }

    ~RefCount() {
      assert(use_count_ == 0);
#ifndef NDEBUG
      use_count_ = 0xDEADBEAF;
#endif
    }

    void add_ref( void ) {
#if defined(_REENTRANT)
      mutex_type::scoped_lock lock(mtx_);
#endif
      assert(use_count_ != 0xDEADBEAF && "WTF?! Taking ownership of deleted object!");
      ++use_count_;
    }

    bool del_ref( void ) {
#if defined(_REENTRANT)
      mutex_type::scoped_lock lock(mtx_);
#endif
      --use_count_;
      return use_count_ == 0;
    }

    bool unique_ref() const { // nothrow
#if defined(_REENTRANT)
      mutex_type::scoped_lock lock(mtx_);
#endif
      return use_count_ == 1;
    }
  };

} } // namespace CoSupport::SmartPtr

#ifndef _COMPILEHEADER_INTRUSIVE_PTR_ADD_REF_REFCOUNT
GNU89_EXTERN_INLINE
#endif
void intrusive_ptr_add_ref(CoSupport::SmartPtr::RefCount *p) {
  p->add_ref();
}

#endif // _INCLUDED_COSUPPORT_SMARTPTR_REFCOUNT_HPP