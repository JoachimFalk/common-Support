// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Rafael Rosales <rafael.rosales@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_COMPAT_NULLPTR_H
#define _INCLUDED_COSUPPORT_COMPAT_NULLPTR_H

#include <CoSupport/cosupport_config.h>

#if defined(COSUPPORT_HAVE_CSTDDEF)
# include <cstddef>
#elif defined(COSUPPORT_HAVE_STDDEF_H)
# include <stddef.h>
#else
# include <stdlib.h>
#endif

/* Allow the use in C++ code.  */
#ifdef __cplusplus
namespace std {
#endif

#ifndef COSUPPORT_HAVE_STD_NULLPTR_T
typedef typeof(NULL) nullptr_t;
#endif //!COSUPPORT_HAVE_STD_NULLPTR_T

#ifdef __cplusplus
} // namespace std
#endif

#ifndef COSUPPORT_HAVE_STD_NULLPTR_T
# define nullptr NULL
#endif //!COSUPPORT_HAVE_STD_NULLPTR_T

#endif //_INCLUDED_COSUPPORT_COMPAT_NULLPTR_H
