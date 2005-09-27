/* vim: set sw=2 ts=8: */
/*
 * Copyright (C) Joachim Falk <joachim.falk@gmx.de> $Date: 2003/01/24 13:49:25 $
 *
 * sassert.h is part of the jf_libs distribution of Joachim Falk;
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
 * $Log: sassert.h,v $
 * Revision 1.3  2003/01/24 13:49:25  joachim
 * Now use new build system headers in jf-libs/xxxx
 *
 * Revision 1.2  2001/01/08 15:25:55  joachim
 * Added sassert_h_rcsid
 *
 */

#ifndef _INCLUDED_SASSERT_H
#define _INCLUDED_SASSERT_H

#include <assert.h>

#ifdef NDEBUG
# define sassert(expr) ( ( expr ), assert( 1 ) )
#else
# define sassert(expr) assert( expr )
#endif

#endif /* _INCLUDED_SASSERT_H */
