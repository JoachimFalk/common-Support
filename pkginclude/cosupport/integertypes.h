/* vim: set sw=2 ts=8: */
/*
 * Copyright (C) Joachim Falk <joachim.falk@gmx.de> $Date: 2001/09/01 17:51:54 $
 *
 * mystdint.h is part of the generic containers package
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * The jflibs package is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
/*
 * $Log: mystdint.h,v $
 * Revision 1.4  2001/09/01 17:51:54  joachim
 * Added stuff from typemac.h
 *
 * Revision 1.3  2001/05/07 00:43:14  joachim
 * handle case that we have only some intxx_t and not neccessary in stdint.h
 *
 * Revision 1.2  2001/04/15 03:59:34  joachim
 * Added extern "C" { xxx } protection for c++ header inclusion
 *
 * Revision 1.1  2001/03/13 10:35:10  joachim
 * Initial revision
 *
 */

#ifndef _INCLUDED_INTEGERTYPES_H
#define _INCLUDED_INTEGERTYPES_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STDDEF_H
# include <stddef.h>
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif
#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAVE_INT8_T
  /* Find uint8_t & int8_t */
# if SIZEOF_CHAR == 1
typedef unsigned char		uint8_t;
typedef signed   char		int8_t;
# elif SIZEOF_SHORT == 1
typedef unsigned short		uint8_t;
typedef signed   short		int8_t;
# elif SIZEOF_INT == 1
typedef unsigned int		uint8_t;
typedef signed   int		int8_t;
# elif SIZEOF_LONG == 1
typedef unsigned long		uint8_t;
typedef signed   long		int8_t;
# elif SIZEOF_LONG_LONG == 1
typedef unsigned long long	uint8_t;
typedef signed   long long	int8_t;
# endif
#endif

#ifndef HAVE_INT16_T
  /* Find uint16_t & int16_t */
# if SIZEOF_SHORT == 2
typedef unsigned short		uint16_t;
typedef signed   short		int16_t;
# elif SIZEOF_INT == 2
typedef unsigned int		uint16_t;
typedef signed   int		int16_t;
# elif SIZEOF_LONG == 2
typedef unsigned long		uint16_t;
typedef signed   long		int16_t;
# elif SIZEOF_LONG_LONG == 2
typedef unsigned long long	uint16_t;
typedef signed   long long	int16_t;
# endif
#endif


#ifndef HAVE_INT32_T
  /* Find uint32_t & int32_t */
# if SIZEOF_SHORT == 4
typedef unsigned short		uint32_t;
typedef signed   short		int32_t;
# elif SIZEOF_INT == 4
typedef unsigned int		uint32_t;
typedef signed   int		int32_t;
# elif SIZEOF_LONG == 4
typedef unsigned long		uint32_t;
typedef signed   long		int32_t;
# elif SIZEOF_LONG_LONG == 4
typedef unsigned long long	uint32_t;
typedef signed   long long	int32_t;
# endif
#endif

#ifndef HAVE_INT64_T
  /* Find uint64_t & int64_t */
# if SIZEOF_SHORT == 8
typedef unsigned short		uint64_t;
typedef signed short		int64_t;
# elif SIZEOF_INT == 8
typedef unsigned int		uint64_t;
typedef signed int		int64_t;
# elif SIZEOF_LONG == 8
typedef unsigned long		uint64_t;
typedef signed long		int64_t;
# elif SIZEOF_LONG_LONG == 8
typedef unsigned long long	uint64_t;
typedef signed long long	int64_t;
# endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDED_INTEGERTYPES_H */