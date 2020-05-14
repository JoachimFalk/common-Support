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

#include "export_config.h"

namespace CoSupport { namespace SmartPtr {

class COSUPPORT_SMARTPTR_API
RefCount {
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

COSUPPORT_SMARTPTR_API
inline
void intrusive_ptr_add_ref(CoSupport::SmartPtr::RefCount *p) {
  p->add_ref();
}

#endif // _INCLUDED_COSUPPORT_SMARTPTR_REFCOUNT_HPP
