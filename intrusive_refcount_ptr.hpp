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

#ifndef _INCLUDED_COSUPPORT_INTRUSIVE_REFCOUNT_PTR_HPP
#define _INCLUDED_COSUPPORT_INTRUSIVE_REFCOUNT_PTR_HPP

#include "refcount.hpp"

#include <boost/intrusive_ptr.hpp>

#define DECL_INTRUSIVE_REFCOUNT_PTR(T,PST)	                \
  typedef ::boost::intrusive_ptr<T> PST;                        \
  void intrusive_ptr_release(T *);

#define IMPL_INTRUSIVE_REFCOUNT_PTR(T)		                \
  void intrusive_ptr_release(T *p) {		                \
    if (p->del_ref())				                \
      delete p;					                \
  }

#endif // _INCLUDED_COSUPPORT_INTRUSIVE_REFCOUNT_PTR_HPP
