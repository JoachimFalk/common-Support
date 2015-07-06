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

#ifndef _INCLUDED_COSUPPORT_SMARTPTR_INTRUSIVE_REFCOUNT_PTR_HPP
#define _INCLUDED_COSUPPORT_SMARTPTR_INTRUSIVE_REFCOUNT_PTR_HPP

#include "RefCount.hpp"

#include <boost/intrusive_ptr.hpp>

namespace CoSupport { namespace SmartPtr {

#define DECL_INTRUSIVE_REFCOUNT_PTR(T,PST)	                \
  typedef ::boost::intrusive_ptr<T> PST;                        \
  void intrusive_ptr_add_ref(T *);                              \
  void intrusive_ptr_release(T *);

#define IMPL_INTRUSIVE_REFCOUNT_PTR(T)		                \
  void intrusive_ptr_add_ref(T *p) {		                \
    p->add_ref();                                               \
  }                                                             \
  void intrusive_ptr_release(T *p) {		                \
    if (p->del_ref())				                \
      delete p;					                \
  }

  // This object temporarily boosts the reference
  // count of obj to prevent another temporary
  // reference pointer from deleting said object.
  template <typename T = RefCount>
  class ScopedRefCountBooster
  : private boost::noncopyable {
  private:
    T *obj;
  public:
    explicit ScopedRefCountBooster(T *obj)
      : obj(obj) { intrusive_ptr_add_ref(obj); }
    // Decrement reference count but never delete object!
    ~ScopedRefCountBooster()
      { obj->del_ref(); }
  };

  template <typename T>
  class ScopedRefCountPtr {
  private:
    T obj;
  public:
    explicit ScopedRefCountPtr()
      : obj() { intrusive_ptr_add_ref(&obj); }
    template <typename T1>
    explicit ScopedRefCountPtr(const T1 &p1)
      : obj(p1) { intrusive_ptr_add_ref(&obj); }
    template <typename T1, typename T2>
    explicit ScopedRefCountPtr(const T1 &p1, const T2 &p2)
      : obj(p1, p2) { intrusive_ptr_add_ref(&obj); }
    template <typename T1, typename T2, typename T3>
    explicit ScopedRefCountPtr(const T1 &p1, const T2 &p2, const T3 &p3)
      : obj(p1, p2, p3) { intrusive_ptr_add_ref(&obj); }
    template <typename T1, typename T2, typename T3, typename T4>
    explicit ScopedRefCountPtr(const T1 &p1, const T2 &p2, const T3 &p3, const T4 &p4)
      : obj(p1, p2, p3, p4) { intrusive_ptr_add_ref(&obj); }
    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    explicit ScopedRefCountPtr(const T1 &p1, const T2 &p2, const T3 &p3, const T4 &p4, const T5 &p5)
      : obj(p1, p2, p3, p4, p5) { intrusive_ptr_add_ref(&obj); }

    operator T *()
      { return &obj; }
    operator const T *() const
      { return &obj; }

    template <typename TT>
    operator ::boost::intrusive_ptr<TT>()
      { return static_cast<TT *>(&obj); }
    template <typename TT>
    operator ::boost::intrusive_ptr<const TT>() const
      { return static_cast<const TT *>(&obj); }

    T *operator ->()
      { return &obj; }
    const T *operator ->() const
      { return &obj; }

    T &operator *()
      { return obj; }
    const T &operator *() const
      { return obj; }

    ~ScopedRefCountPtr()
      { sassert(obj.del_ref()); }
  };

} } // namespace CoSupport::SmartPtr

#endif // _INCLUDED_COSUPPORT_SMARTPTR_INTRUSIVE_REFCOUNT_PTR_HPP
