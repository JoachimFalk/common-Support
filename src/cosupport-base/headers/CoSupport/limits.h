// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2012 Christian Zebelein <christian.zebelein@uni-rostock.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_LIMITS_H
#define _INCLUDED_COSUPPORT_LIMITS_H

#include <limits.h>

#include <CoSupport/cosupport_config.h>

/*
 * macros for getting the TYPE and its respective min,max values
 */

#define BITS_TYPE(TYPE) (sizeof(TYPE)*CHAR_BIT)

#ifdef COSUPPORT_HAVE_LONG_LONG
# define MAX_STYPE(TYPE) ((long long) ((unsigned long long) -1 >>			\
                            ((sizeof(unsigned long long)-sizeof(TYPE))*CHAR_BIT+1)))
# define MIN_STYPE(TYPE) ((long long) ((long long) -1 <<				\
                            (sizeof(TYPE)*CHAR_BIT-1)))
# define MAX_UTYPE(TYPE) ((unsigned long long) ((unsigned long long) -1 >>		\
                            ((sizeof(unsigned long long)-sizeof(TYPE))*CHAR_BIT)))
# define MIN_UTYPE(TYPE) ((unsigned long long) 0)

//# define LONGLONG_MAX  MAX_STYPE(long long)
//# define LONGLONG_MIN  MIN_STYPE(long long)
//# define ULONGLONG_MAX MAX_UTYPE(unsigned long long)

#else // !defined(COSUPPORT_HAVE_LONG_LONG)
# define MAX_STYPE(TYPE) ((long) ((unsigned long) -1 >>					\
                            ((sizeof(unsigned long)-sizeof(TYPE))*CHAR_BIT+1)))
# define MIN_STYPE(TYPE) ((long) ((long) -1 <<						\
                            (sizeof(TYPE)*CHAR_BIT-1) ) )
# define MAX_UTYPE(TYPE) ((unsigned long) ((unsigned long) -1 >>			\
                            ((sizeof(unsigned long)-sizeof(TYPE))*CHAR_BIT)))
# define MIN_UTYPE(TYPE) ((unsigned long) 0 )
#endif // !defined(COSUPPORT_HAVE_LONG_LONG)

#define ISSIGNED_TYPE(TYPE) ((TYPE)-1 < 0)

#define MAX_TYPE(TYPE) ((TYPE) (ISSIGNED_TYPE(TYPE) ? MAX_STYPE(TYPE) : MAX_UTYPE(TYPE)))
#define MIN_TYPE(TYPE) ((TYPE) (ISSIGNED_TYPE(TYPE) ? MIN_STYPE(TYPE) : MIN_UTYPE(TYPE)))

#endif /* _INCLUDED_COSUPPORT_LIMITS_H */
