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

#ifndef _INCLUDED_COSUPPORT_STREAMS_COLORSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_COLORSTREAMBUF_HPP

#include <ostream>

#include "FilterStreambuf.hpp"

namespace CoSupport { namespace Streams {

/**
 * stream manipulator for the ColorStreambuf custom
 * streambuffer
 */
struct Color {
  /// console escape code for this color
  std::string escape;
  
  /// constructs a new object with the specified attribute
  /// and foreground color
  Color(size_t color, size_t attr);
  
  /// constructs a new object whith the specified attribute
  /// (no change to the foreground color)
  Color(size_t attr);
  
  /// predefined colors 
  static const Color Auto;
  static const Color Black;
  static const Color Red;
  static const Color BrightRed;
  static const Color Green;
  static const Color BrightGreen;
  static const Color Brown;
  static const Color BrightBrown;
  static const Color Blue;
  static const Color BrightBlue;
  static const Color Purple;
  static const Color BrightPurple;
  static const Color Cyan;
  static const Color BrightCyan;
  static const Color Gray;
  static const Color BrightGray;
};

/**
 * colors each line with the specified color
 */
class ColorStreambuf
: public FilterStreambuf {
private:
  /// color of the printed chars
  Color color;

  /// color to set after newline
  Color reset;
  
  /// indicator if newline was encountered
  bool newline;
  
public:
  /// constructs a new object with the specified
  /// colors
  ColorStreambuf(
      const Color &color = Color::Auto,
      const Color &reset = Color::Auto,
      std::streambuf *next = 0);
  
  /// set a new line color
  void setColor(const Color &c);
  
  /// set a new reset color
  void setReset(const Color &c);
  
protected:
  int overflow(int c);
  
public:
  /// index obtained with std::ostream::xalloc
  static const int index;
 
  /// see Color 
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the Color manipulator
std::ostream &operator<<(std::ostream &os, const Color &c);

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_FILTER_OSTREAM_HPP
