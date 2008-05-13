// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2008 Hardware-Software-CoDesign, University of
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

namespace CoSupport { namespace Streams {

namespace Detail {
  /**
   * @brief Conversion types for printHex function
   */
  template<class T>
  struct PrintHexTraits {
    /// @brief Type used to determine number of digits
    typedef T unsigned_type;
    /// @brief Type used to print number
    typedef T print_type;
  };

  /**
   * @brief Specialization of PrintHexTraits for uint8_t
   */
  template<>
  struct PrintHexTraits<uint8_t> {
    /// @brief Type used to determine number of digits
    typedef uint8_t unsigned_type;
    /// @brief Type used to print number
    typedef uint16_t print_type;
  };

  /**
   * @brief Specialization of PrintHexTraits for char
   */
  template<>
  struct PrintHexTraits<char> {
    /// @brief Type used to determine number of digits
    typedef uint8_t unsigned_type;
    /// @brief Type used to print number
    typedef uint16_t print_type;
  };

  /// @brief Prints integer as hex number
  /// @param[in] os Target ostream
  /// @param[in] t Number
  template<class T>
  void printHex(std::ostream &os, const T& t) {
    std::ios_base::fmtflags f = os.flags();
    os.setf(os.hex, os.basefield);
    os.setf(os.uppercase);

    std::streamsize w = os.width();
    os.width(std::numeric_limits<
        typename PrintHexTraits<T>::unsigned_type>::digits >> 2);

    char fl = os.fill();
    os.fill('0');

    os <<
      static_cast<typename PrintHexTraits<T>::print_type>(
      static_cast<typename PrintHexTraits<T>::unsigned_type>(t));

    os.flags(f);
    os.width(w);
    os.fill(fl);
  }

  /**
   * @brief Hex output manipulator class
   * @param T Integer type (short, long, etc.)
   */
  template<class T>
  struct hexManip {
    /// @brief Number
    T t;

    /// @brief True, if "0x" should be printed
    bool prefix;

    /// @brief Constructor
    /// @param[in] t Number
    /// @param[in] prefix True, if "0x" should be printed
    hexManip(const T& t, bool prefix = true) :
      t(t), prefix(prefix)
    {}
  };

  /// @brief Output operator for hexManip
  /// @param[in] os Target ostream
  /// @param[in] m Hex output manipulator
  template<class T>
  std::ostream& operator<<(std::ostream& os, const hexManip<T>& m)
    { if(m.prefix) os << "0x"; printHex<T>(os, m.t); return os; }
} // namespace Detail

/// @brief Convenience function for creating an hex output manipulator
/// @param[in] t Number
/// @param[in] prefix True, if "0x" should be printed
/// @return Corresponding hex output manipulator object
template<class T>
Detail::hexManip<T> hex(T t, bool prefix = true)
  { return Detail::hexManip<T>(t, prefix); }

} } // namespace CoSupport::Streams

#endif // INCLUDED_COSUPPORT_STREAMS_HEXOUTPUT_HPP
