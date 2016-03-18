// vim: set sw=2 ts=8:
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

#ifndef INCLUDED_COSUPPORT_STREAMS_HEXOUTPUT_HPP
#define INCLUDED_COSUPPORT_STREAMS_HEXOUTPUT_HPP

#include <ostream>
#include <sstream>

namespace CoSupport { namespace Streams {

namespace Detail {

  /// @brief Prints integer as hex number
  template<class T>
  void printHex(std::ostream &os, T t, bool prefix, bool plz, size_t bits) {
    size_t d = bits / 8;
    size_t m = bits % 8;

    static char lut[] = {'0','1','2','3','4','5','6','7',
                         '8','9','A','B','C','D','E','F'};

    const unsigned char* x = (const unsigned char*)&t;
    unsigned char p;

    if(prefix)
      os << "0x";

    if(m > 4) {
      p = (x[d] >> 4) & ((1 << (m-4)) - 1);
      if(p || plz) {
        os << lut[p];
        plz = true;
      }
      p = x[d] & 0xF;
      if(p || plz) {
        os << lut[p];
        plz = true;
      }
    }
    else if(m) {
      p = x[d] & ((1 << m) - 1);
      if(p || plz) {
        os << lut[p];
        plz = true;
      }
    }

    for(size_t i = d; i; --i) {
      p = x[i-1] >> 4;
      if(p || plz) {
        os << lut[p];
        plz = true;
      }
      p = x[i-1] & 0xF;
      if(p || plz) {
        os << lut[p];
        plz = true;
      }
    }
  }

  /**
   * @brief Hex output manipulator class
   */
  template<class T>
  struct HexOutput {
    /// @brief Number
    T t;

    /// @brief True, if "0x" should be printed
    bool prefix;

    /// @brief True, if leading zeros should be printed
    bool plz;
    
    /// @brief Number of ls. bits to be printed
    size_t bits;

    /// @brief Constructor
    HexOutput(T t, bool prefix, bool plz, size_t bits)
      : t(t), prefix(prefix), plz(plz), bits(bits)
    {}
  };

  /// @brief Output operator for HexOutput
  template<class T>
  std::ostream& operator<<(std::ostream& os, const HexOutput<T>& m)
    { printHex<T>(os, m.t, m.prefix, m.plz, m.bits); return os; }

} // namespace Detail

/// @brief Convenience function for creating an hex output manipulator
template<class T>
Detail::HexOutput<T> hex(
    T t, bool prefix = true, bool plz = true, size_t bits = sizeof(T) * 8)
  { return Detail::HexOutput<T>(t, prefix, plz, bits); }

template<class T>
std::string asHexStr(const T& t, bool prefix = true, bool plz = true, size_t bits = sizeof(T) * 8) {
  std::ostringstream os;
  Detail::printHex(os, t, prefix, plz, bits);
  return os.str();
}

} } // namespace CoSupport::Streams

#endif // INCLUDED_COSUPPORT_STREAMS_HEXOUTPUT_HPP
