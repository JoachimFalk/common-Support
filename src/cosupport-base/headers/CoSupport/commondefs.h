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

#ifndef _INCLUDED_COSUPPORT_COMMONDEFS_H
#define _INCLUDED_COSUPPORT_COMMONDEFS_H

#include <CoSupport/cosupport_config.h>

#if __GNUC__ > 3 || (__GNUC__ == 3 && (__GNUC_MINOR__ > 3 || (__GNUC_MINOR__ == 3 && __GNUC_PATCHLEVEL__ > 5)))
# define COSUPPORT_ATTRIBUTE_DEPRECATED __attribute__ ((deprecated))
#else
# define COSUPPORT_ATTRIBUTE_DEPRECATED
#endif

#if __GNUC__ > 3
// GCC
# define COSUPPORT_ATTRIBUTE_EXPORT __attribute__((visibility("default")))
# define COSUPPORT_ATTRIBUTE_IMPORT
#elif defined(_MSC_VER)
// Microsoft 
# define COSUPPORT_ATTRIBUTE_EXPORT __declspec(dllexport)
# define COSUPPORT_ATTRIBUTE_IMPORT __declspec(dllimport)
#else
// Do nothing and hope for the best?
# define COSUPPORT_ATTRIBUTE_EXPORT
# define COSUPPORT_ATTRIBUTE_IMPORT
# warning "Warning Unknown dynamic link import/export semantics."
#endif

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
