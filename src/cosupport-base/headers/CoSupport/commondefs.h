// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2012 Christian Zebelein <christian.zebelein@uni-rostock.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_COMMONDEFS_H
#define _INCLUDED_COSUPPORT_COMMONDEFS_H

#include <CoSupport/cosupport_config.h>

#if __GNUC__ > 3 || (__GNUC__ == 3 && (__GNUC_MINOR__ > 3 || (__GNUC_MINOR__ == 3 && __GNUC_PATCHLEVEL__ > 5)))
# define COSUPPORT_ATTRIBUTE_DEPRECATED __attribute__ ((deprecated))
#else
# define COSUPPORT_ATTRIBUTE_DEPRECATED
#endif
#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 0)
# define COSUPPORT_ATTRIBUTE_UNUSED __attribute__ ((unused))
#else
# define COSUPPORT_ATTRIBUTE_UNUSED
#endif

// Generic helper definitions for shared library support
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__ // Windows, either gcc or msvc
# define COSUPPORT_ATTRIBUTE_DLL_IMPORT __declspec(dllimport)
# define COSUPPORT_ATTRIBUTE_DLL_EXPORT __declspec(dllexport)
# define COSUPPORT_ATTRIBUTE_DLL_LOCAL
#else // Not windows
# if __GNUC__ >= 4 // Visibility support
#   define COSUPPORT_ATTRIBUTE_DLL_IMPORT __attribute__ ((visibility ("default")))
#   define COSUPPORT_ATTRIBUTE_DLL_EXPORT __attribute__ ((visibility ("default")))
#   define COSUPPORT_ATTRIBUTE_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
# else // No visibility support
#   define COSUPPORT_ATTRIBUTE_DLL_IMPORT
#   define COSUPPORT_ATTRIBUTE_DLL_EXPORT
#   define COSUPPORT_ATTRIBUTE_DLL_LOCAL
# endif
#endif // Not windows

#define COSUPPORT_STRINGISE_IMPL(x) #x
#define COSUPPORT_STRINGISE(x) COSUPPORT_STRINGISE_IMPL(x)
#define COSUPPORT_FILE_LINE_LINK __FILE__ "(" COSUPPORT_STRINGISE(__LINE__) ") : "

// Use: #pragma message WARN("My message")
#ifdef _MSC_VER
# define COSUPPORT_WARN(exp) (FILE_LINE_LINK "warning: " exp)
#else //__GNUC__ - may need other defines for different compilers
# define COSUPPORT_WARN(exp) ("warning: " exp)
#endif

#endif /* _INCLUDED_COSUPPORT_COMMONDEFS_H */
