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
# if defined(_MSC_VER)
typedef unsigned __int8		uint8_t;
typedef signed   __int8		int8_t;
# elif SIZEOF_CHAR == 1
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
# if defined(_MSC_VER)
typedef unsigned __int16	uint16_t;
typedef signed   __int16	int16_t;
# elif SIZEOF_SHORT == 2
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
# if defined(_MSC_VER)
typedef unsigned __int32	uint32_t;
typedef signed   __int32	int32_t;
# elif SIZEOF_SHORT == 4
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
# if defined(_MSC_VER)
typedef unsigned __int64	uint64_t;
typedef signed   __int64	int64_t;
# elif SIZEOF_SHORT == 8
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
