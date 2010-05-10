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

#ifndef INCLUDED_COSUPPORT_STRING_HASH_HPP
#define INCLUDED_COSUPPORT_STRING_HASH_HPP

#include "../commondefs.h"
#include "../compatibility-glue/integertypes.h"
#include <stdint.h> // uint64_t

namespace CoSupport { namespace Math {

namespace Detail {

  // calculates smallest x = 2^k; x > N  (e.g. 29 -> 32, 32 -> 64)
  template<uint64_t N, uint64_t S = 1>
  struct NextPower2
    { static const size_t value = NextPower2<(N >> 1), (S << 1)>::value; };

  template<uint64_t S>
  struct NextPower2<0, S>
    { static const uint64_t value = S; };

  // http://isthe.com/chongo/tech/comp/fnv/
  template<class T, T INIT, T PRIME>
  class FNVBase {
  public:
    typedef T result_type;

    template<class N>
    result_type operator()(N n) const {
      result_type h = INIT;
      while(*n) {
        h *= PRIME;
        h ^= *n++;
      }
      return h;
    }
  };

} // namespace Detail

// http://isthe.com/chongo/tech/comp/fnv/
// (BITS <= 64)
template<size_t BITS>
class FNV {
public:
  typedef uint64_t result_type;

  template<class N>
  result_type operator()(N n) const
    { return xorFold(fnv(n)); }

private:
  static const result_type MASK = (1ull << BITS) - 1ull;

  FNV<Detail::NextPower2<BITS>::value> fnv;

  result_type xorFold(result_type h) const
    { return (h >> BITS) ^ (h & MASK); }
};

// http://isthe.com/chongo/tech/comp/fnv/
template<>
class FNV<32> : public Detail::FNVBase<
  uint32_t,
  0x811C9DC5ul,
  0x01000193ul>
{};

// http://isthe.com/chongo/tech/comp/fnv/
template<>
class FNV<64> : public Detail::FNVBase<
  uint64_t,
  0xCBF29CE484222325ull,
  0x00000100000001B3ull>
{};

} } // namespace CoSupport::Math

// Backward compatibility cruft
namespace CoSupport {

template<size_t BITS>
class FNV: public Math::FNV<BITS> {
public:
  COSUPPORT_ATTRIBUTE_DEPRECATED FNV() {}
};

} // namespace CoSupport

#endif // INCLUDED_COSUPPORT_STRING_HASH_HPP
