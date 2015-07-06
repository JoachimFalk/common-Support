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

#ifndef _INCLUDED_COSUPPORT_ENDIANESS_H
#define _INCLUDED_COSUPPORT_ENDIANESS_H

#include <endian.h>
#include <byteswap.h>

#include <stdint.h>

// based on <netinet/in.h>
#if __BYTE_ORDER == __BIG_ENDIAN // big endian
static inline
uint16_t toBigEndian16(uint16_t x)
  { return x; }
static inline
uint16_t fromBigEndian16(uint16_t x)
  { return x; }

static inline
uint32_t toBigEndian32(uint32_t x)
  { return x; }
static inline
uint32_t fromBigEndian32(uint32_t x)
  { return x; }

static inline
uint64_t toBigEndian64(uint64_t x)
  { return x; }
static inline
uint64_t fromBigEndian64(uint64_t x)
  { return x; }

static inline
uint16_t toLittleEndian16(uint16_t x)
  { return bswap_16(x); }
static inline
uint16_t fromLittleEndian16(uint16_t x)
  { return bswap_16(x); }

static inline
uint32_t toLittleEndian32(uint32_t x)
  { return bswap_32(x); }
static inline
uint32_t fromLittleEndian32(uint32_t x)
  { return bswap_32(x); }

static inline
uint64_t toLittleEndian64(uint64_t x)
  { return bswap_64(x); }
static inline
uint64_t fromLittleEndian64(uint64_t x)
  { return bswap_64(x); }
#elif __BYTE_ORDER == __LITTLE_ENDIAN // little endian
static inline
uint16_t toBigEndian16(uint16_t x)
  { return bswap_16(x); }
static inline
uint16_t fromBigEndian16(uint16_t x)
  { return bswap_16(x); }

static inline
uint32_t toBigEndian32(uint32_t x)
  { return bswap_32(x); }
static inline
uint32_t fromBigEndian32(uint32_t x)
  { return bswap_32(x); }

static inline
uint64_t toBigEndian64(uint64_t x)
  { return bswap_64(x); }
static inline
uint64_t fromBigEndian64(uint64_t x)
  { return bswap_64(x); }

static inline
uint16_t toLittleEndian16(uint16_t x)
  { return x; }
static inline
uint16_t fromLittleEndian16(uint16_t x)
  { return x; }

static inline
uint32_t toLittleEndian32(uint32_t x)
  { return x; }
static inline
uint32_t fromLittleEndian32(uint32_t x)
  { return x; }

static inline
uint64_t toLittleEndian64(uint64_t x)
  { return x; }
static inline
uint64_t fromLittleEndian64(uint64_t x)
  { return x; }
#else // unknown endianess
# error "Unknown endianess!"
#endif // unknown endianess

#ifdef __cplusplus
static inline uint16_t toBigEndian(uint16_t x)    { return toBigEndian16(x); }
static inline uint32_t toBigEndian(uint32_t x)    { return toBigEndian32(x); }
static inline uint64_t toBigEndian(uint64_t x)    { return toBigEndian64(x); }
static inline uint16_t toLittleEndian(uint16_t x) { return toLittleEndian16(x); }
static inline uint32_t toLittleEndian(uint32_t x) { return toLittleEndian32(x); }
static inline uint64_t toLittleEndian(uint64_t x) { return toLittleEndian64(x); }

static inline uint16_t fromBigEndian(uint16_t x)    { return fromBigEndian16(x); }
static inline uint32_t fromBigEndian(uint32_t x)    { return fromBigEndian32(x); }
static inline uint64_t fromBigEndian(uint64_t x)    { return fromBigEndian64(x); }
static inline uint16_t fromLittleEndian(uint16_t x) { return fromLittleEndian16(x); }
static inline uint32_t fromLittleEndian(uint32_t x) { return fromLittleEndian32(x); }
static inline uint64_t fromLittleEndian(uint64_t x) { return fromLittleEndian64(x); }
#endif // __cplusplus

#endif /* _INCLUDED_COSUPPORT_ENDIANESS_H */
