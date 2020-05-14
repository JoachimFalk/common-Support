// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Sebastian Graf <sebastian.graf@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_COMPAT_INTEGERTYPES_H
#define _INCLUDED_COSUPPORT_COMPAT_INTEGERTYPES_H

#include <CoSupport/cosupport_config.h>

#ifdef COSUPPORT_HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef COSUPPORT_HAVE_STDDEF_H
# include <stddef.h>
#endif
#ifdef COSUPPORT_HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef COSUPPORT_HAVE_STDINT_H
# include <stdint.h>
#endif

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef COSUPPORT_HAVE_INT8_T
  /* Find int8_t */
# if defined(_MSC_VER)
typedef signed __int8              int8_t;
# else // !_MSC_VER
typedef COSUPPORT_REPLTYP_INT8_T   int8_t;
# endif
#endif // !COSUPPORT_HAVE_INT8_T
#ifndef COSUPPORT_HAVE_UINT8_T
  /* Find uint8_t */
# if defined(_MSC_VER)
typedef unsigned __int8            uint8_t;
# else // !_MSC_VER
typedef COSUPPORT_REPLTYP_UINT8_T  uint8_t;
# endif
#endif // !COSUPPORT_HAVE_UINT8_T

#ifndef COSUPPORT_HAVE_INT16_T
  /* Find int16_t */
# if defined(_MSC_VER)
typedef signed __int16             int16_t;
# else // !_MSC_VER
typedef COSUPPORT_REPLTYP_INT16_T  int16_t;
# endif
#endif // !COSUPPORT_HAVE_INT16_T
#ifndef COSUPPORT_HAVE_UINT16_T
  /* Find uint16_t */
# if defined(_MSC_VER)
typedef unsigned __int16           uint16_t;
# else // !_MSC_VER
typedef COSUPPORT_REPLTYP_UINT16_T uint16_t;
# endif
#endif // !COSUPPORT_HAVE_UINT16_T

#ifndef COSUPPORT_HAVE_INT32_T
  /* Find int32_t */
# if defined(_MSC_VER)
typedef signed __int32             int32_t;
# else // !_MSC_VER
typedef COSUPPORT_REPLTYP_INT32_T  int32_t;
# endif
#endif // !COSUPPORT_HAVE_INT32_T
#ifndef COSUPPORT_HAVE_UINT32_T
  /* Find uint32_t */
# if defined(_MSC_VER)
typedef unsigned __int32           uint32_t;
# else // !_MSC_VER
typedef COSUPPORT_REPLTYP_UINT32_T uint32_t;
# endif
#endif // !COSUPPORT_HAVE_UINT32_T

#ifndef COSUPPORT_HAVE_INT64_T
  /* Find int64_t */
# if defined(_MSC_VER)
typedef signed __int64             int64_t;
# else // !_MSC_VER
typedef COSUPPORT_REPLTYP_INT64_T  int64_t;
# endif
#endif // !COSUPPORT_HAVE_INT64_T
#ifndef COSUPPORT_HAVE_UINT64_T
  /* Find uint64_t */
# if defined(_MSC_VER)
typedef unsigned __int64           uint64_t;
# else // !_MSC_VER
typedef COSUPPORT_REPLTYP_UINT64_T uint64_t;
# endif
#endif // !COSUPPORT_HAVE_UINT64_T

#ifndef COSUPPORT_HAVE_SIZE_T
  /* Find size_t */
typedef COSUPPORT_REPLTYP_SIZE_T   size_t;
#endif // !COSUPPORT_HAVE_SIZE_T
#ifndef COSUPPORT_HAVE_SSIZE_T
  /* Find ssize_t */
typedef COSUPPORT_REPLTYP_SSIZE_T  ssize_t;
#endif // !COSUPPORT_HAVE_SSIZE_T

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDED_COSUPPORT_COMPAT_INTEGERTYPES_H */
