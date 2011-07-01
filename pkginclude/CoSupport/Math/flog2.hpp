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

#ifndef INCLUDED_COSUPPORT_FLOG2_HPP
#define INCLUDED_COSUPPORT_FLOG2_HPP


#include <stddef.h>
#include "../commondefs.h"
#ifdef _MSC_VER
#include "../compatibility-glue/integertypes.h"
#else
#include <stdint.h>
#endif // _MSC_VER

namespace CoSupport {

// Returns the log base 2 of an integer (floor variant)
// e.g. flog2f(1) == 0
//      ...
//      flog2f(7) == 2
//      flog2f(8) == 3
//      flog2f(9) == 3

size_t flog2f(uint32_t n);
size_t flog2f(uint64_t n);

// Returns the log base 2 of an integer (ceil variant)
// e.g. flog2c(7) == 3
//      flog2c(8) == 3
//      flog2c(9) == 4
//      (Special case: flog2c(1) == 1)

size_t flog2c(uint32_t n);
size_t flog2c(uint64_t n);

COSUPPORT_ATTRIBUTE_DEPRECATED
static inline
size_t flog2(uint32_t n)
  { return flog2c(n); }

COSUPPORT_ATTRIBUTE_DEPRECATED
static inline
size_t flog2(uint64_t n)
  { return flog2c(n); }

} // namespace CoSupport

#endif // INCLUDED_COSUPPORT_FLOG2_HPP
