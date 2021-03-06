// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STREAMS_COLORSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_COLORSTREAMBUF_HPP

#include <stddef.h>
//#include <iostream>
#include <string>

#include "FilterStreambuf.hpp"

#include "export_config.h"

namespace CoSupport { namespace Streams {

/**
 * stream manipulator for the ColorStreambuf custom
 * streambuffer
 */
struct COSUPPORT_STREAMS_API
Color {
  /// console escape code for this color
  std::string escape;
  
  /// constructs a new object with the specified attribute
  /// and foreground color
  Color(size_t color, size_t attr);
  
  /// constructs a new object whith the specified attribute
  /// (no change to the foreground color)
  Color(size_t attr);

  // Color(const Color &c)
  // {
  //   std::cerr << "Enter Color(const Color &c)" << std::endl;
  //   std::cerr << escape << std::endl;
  //   std::cerr.flush();
  //   std::cerr << "&c = " << &c << std::endl;
  //   std::cerr.flush();
  //   escape = c.escape;
  //   std::cerr << escape << std::endl;
  //   std::cerr.flush();
  //   std::cerr << "Leave Color(const Color &c)" << std::endl;
  // }


  /// predefined colors 
  /// We do not use static variables in order to avoid racing conditions
  /// when initializing global variables.
  static const Color &Auto()         { static Color c(0)    ; return c; }
  static const Color &Black()        { static Color c(30, 0); return c; }
  static const Color &Red()          { static Color c(31, 0); return c; }
  static const Color &BrightRed()    { static Color c(31, 1); return c; }
  static const Color &Green()        { static Color c(32, 0); return c; }
  static const Color &BrightGreen()  { static Color c(32, 1); return c; }
  static const Color &Brown()        { static Color c(33, 0); return c; }
  static const Color &BrightBrown()  { static Color c(33, 1); return c; }
  static const Color &Blue()         { static Color c(34, 0); return c; }
  static const Color &BrightBlue()   { static Color c(34, 1); return c; }
  static const Color &Purple()       { static Color c(35, 0); return c; }
  static const Color &BrightPurple() { static Color c(35, 1); return c; }
  static const Color &Cyan()         { static Color c(36, 0); return c; }
  static const Color &BrightCyan()   { static Color c(36, 1); return c; }
  static const Color &Gray()         { static Color c(37, 0); return c; }
  static const Color &BrightGray()   { static Color c(37, 1); return c; }
};

/**
 * colors each line with the specified color
 */
class COSUPPORT_STREAMS_API
ColorStreambuf
: public FilterStreambuf {
public:
  template <class Base = FilterOStream> class Stream;
public:
  /// constructs a new object with the specified
  /// colors
  ColorStreambuf(
                 const Color &color = Color::Auto(),
                 const Color &reset = Color::Auto(),
                 std::streambuf *next = 0);

  /// set a new line color
  void setColor(const Color &c);
 
  /// set a new reset color
  void setReset(const Color &c);
 
  /// index obtained with std::ostream::xalloc
  static const int index;
protected:
  int overflow(int c);
  
  /// see Color 
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;

private:
  /// color of the printed chars
  Color color;

  /// color to set after newline
  Color reset;

  /// indicator if newline was encountered
  bool newline;
};

template <class Base>
class ColorStreambuf::Stream: public Base {
public:
  /// construct a new object which uses the streambuffer
  /// of the specified stream as initial target
  Stream(std::ostream &os)
    : Base(os) { this->insert(colorer); }
private:
  ColorStreambuf colorer;
};

/// output operator for the Color manipulator
COSUPPORT_STREAMS_API
std::ostream &operator << (std::ostream &os, const Color &c);

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_FILTER_OSTREAM_HPP
