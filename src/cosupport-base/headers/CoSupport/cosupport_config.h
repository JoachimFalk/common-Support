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

#ifndef _INCLUDED_COSUPPORT_CONFIG_H
#define _INCLUDED_COSUPPORT_CONFIG_H

/* Defined if debug support is enabled */
#define COSUPPORT_ENABLE_DEBUG yes

/* Defined if SystemC support is enabled */
#define COSUPPORT_ENABLE_SYSTEMC yes

/* Defined if xerces support is enabled */
#define COSUPPORT_ENABLE_XERCES yes

/* Defined if you have the <cstddef> header */
#define COSUPPORT_HAVE_CSTDDEF 1

/* Defined if you have the <stddef.h> header */
#define COSUPPORT_HAVE_STDDEF_H 1

/* Defined if you have the <cstddef> header defining the std::nullptr_t data type */
#define COSUPPORT_HAVE_STD_NULLPTR_T 1

/* Define to 1 if you have the long long type. */
#define COSUPPORT_HAVE_LONG_LONG 1

/* Define to 1 if you have the <stddef.h> header file. */
#define COSUPPORT_HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define COSUPPORT_HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define COSUPPORT_HAVE_STDLIB_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define COSUPPORT_HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the size_t type. */
#define COSUPPORT_HAVE_SIZE_T 1

/* Define to the replacement type for size_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_SIZE_T */

/* Define to 1 if you have the ssize_t type. */
#define COSUPPORT_HAVE_SSIZE_T 1

/* Define to the replacement type for ssize_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_SSIZE_T */

/* Define to 1 if you have the int8_t type. */
#define COSUPPORT_HAVE_INT8_T 1

/* Define to the replacement type for int8_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_INT8_T */

/* Define to 1 if you have the uint8_t type. */
#define COSUPPORT_HAVE_UINT8_T 1

/* Define to the replacement type for uint8_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_UINT8_T */

/* Define to 1 if you have the int16_t type. */
#define COSUPPORT_HAVE_INT16_T 1

/* Define to the replacement type for int16_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_INT16_T */

/* Define to 1 if you have the uint16_t type. */
#define COSUPPORT_HAVE_UINT16_T 1

/* Define to the replacement type for uint16_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_UINT16_T */

/* Define to 1 if you have the int32_t type. */
#define COSUPPORT_HAVE_INT32_T 1

/* Define to the replacement type for int32_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_INT32_T */

/* Define to 1 if you have the uint32_t type. */
#define COSUPPORT_HAVE_UINT32_T 1

/* Define to the replacement type for uint32_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_UINT32_T */

/* Define to 1 if you have the int64_t type. */
#define COSUPPORT_HAVE_INT64_T 1

/* Define to the replacement type for int64_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_INT64_T */

/* Define to 1 if you have the uint64_t type. */
#define COSUPPORT_HAVE_UINT64_T 1

/* Define to the replacement type for uint64_t if this type is missing. */
/* #undef COSUPPORT_REPLTYP_UINT64_T */

#endif /* _INCLUDED_COSUPPORT_CONFIG_H */
