/* vim: set sw=2 ts=8: */
/*
 * Copyright (C) Joachim Falk <joachim.falk@gmx.de> $Date: 2003/01/24 13:49:25 $
 *
 * commondefs.h is part of the jf_libs distribution of Joachim Falk;
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * The jf_libs distributio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
/*
 * $Log: commondefs.h,v $
 * Revision 1.2  2003/01/24 13:49:25  joachim
 * Now use new build system headers in cosupport/xxxx
 *
 * Revision 1.1  2001/09/01 17:51:54  joachim
 * Initial revision
 *
 */

#ifndef _INCLUDED_COMMONDEFS_H
#define _INCLUDED_COMMONDEFS_H

#define PE_PTROK(x)		( ((unsigned long) (x)) >= 0x400 )
#define PE_PTRERROR(x)		((void *) (x))
#define PE_PTRERRORNR(x)	((unsigned long) (x))

#ifndef __SCFE__
# ifdef __cplusplus
#   define GNU89_STATIC_INLINE	static inline
#   define GNU89_EXTERN_INLINE  inline
#   define GNU89_INLINE         inline
# else // !__cplusplus
#   ifndef HAVE_C99_INLINE
#     define GNU89_STATIC_INLINE	static inline
#     define GNU89_EXTERN_INLINE	extern inline
#     define GNU89_INLINE		inline
#   else
#     define GNU89_STATIC_INLINE	static inline
#     define GNU89_EXTERN_INLINE	inline
#     define GNU89_INLINE		extern inline
#   endif
# endif // __cplusplus
#else
# define GNU89_STATIC_INLINE	
# define GNU89_EXTERN_INLINE	
# define GNU89_INLINE		
#endif

#define GNUC_EXPLICIT_TEMPLATE(x) \
  extern template x

#ifdef NDEBUG
# define GROWALGO(x)    ( (x) + ((x) >> 1) + 256 )
# define SHRINKALGO(x)  ( (x) / 3 * 2 )
#else
# define GROWALGO(x)    ( (x) + ((x) >> 1) + 1 )
# define SHRINKALGO(x)  ( (x) / 3 * 2 )
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && (__GNUC_MINOR__ > 3 || (__GNUC_MINOR__ == 3 && __GNUC_PATCHLEVEL__ > 5)))
# define COSUPPORT_ATTRIBUTE_DEPRECATED __attribute__ ((deprecated))
#else
# define COSUPPORT_ATTRIBUTE_DEPRECATED
#endif

/*
 * macros for getting the type and its respective min,max values
 */

#ifdef HAVE_LONG_LONG
# define MAX_STYPE( type )  ( (long long) ( (unsigned long long) -1 >>					\
				     ((sizeof(unsigned long long)-sizeof(type))*8+1) ) )
# define MIN_STYPE( type )  ( (long long) ( (long long) -1 <<						\
				     (sizeof(type)*8-1) ) )
# define MAX_UTYPE( type ) ( (unsigned long long) ( (unsigned long long) -1 >>				\
					      ((sizeof(unsigned long long)-sizeof(type))*8) ) )
# define MIN_UTYPE( type ) ( (unsigned long long) 0 )

# define LONGLONG_MAX  MAX_STYPE(long long)
# define LONGLONG_MIN  MIN_STYPE(long long)
# define ULONGLONG_MAX MAX_UTYPE(unsigned long long)

#else // !defined(HAVE_LONG_LONG)
# define MAX_STYPE( type )  ( (long) ( (unsigned long) -1 >>					\
				     ((sizeof(unsigned long)-sizeof(type))*8+1) ) )
# define MIN_STYPE( type )  ( (long) ( (long) -1 <<						\
				     (sizeof(type)*8-1) ) )
# define MAX_UTYPE( type ) ( (unsigned long) ( (unsigned long) -1 >>				\
					      ((sizeof(unsigned long)-sizeof(type))*8) ) )
# define MIN_UTYPE( type ) ( (unsigned long) 0 )
#endif // !defined(HAVE_LONG_LONG)

#define ISSIGNED_TYPE( type ) ( (type) -1 < 0 )

#define MAX_TYPE( type ) ( (type) ( ISSIGNED_TYPE( type ) ? MAX_STYPE( type ) : MAX_UTYPE( type ) ) )
#define MIN_TYPE( type ) ( (type) ( ISSIGNED_TYPE( type ) ? MIN_STYPE( type ) : MIN_UTYPE( type ) ) )

#endif /* _INCLUDED_COMMONDEFS_H */