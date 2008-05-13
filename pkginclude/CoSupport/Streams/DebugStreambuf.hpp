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

#ifndef _INCLUDED_COSUPPORT_STREAMS_DEBUGSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_DEBUGSTREAMBUF_HPP

#include <ostream>

#include "FilterStreambuf.hpp"

namespace CoSupport { namespace Streams {

/**
 * stream manipulator for the DebugStreambuf custom
 * streambuffer
 */
struct Debug {
  /// new debug level
  size_t level;
  
  /// constructs a new object with the specified debug
  /// level
  Debug(size_t level);
  
  /// predefined debug levels
  static const Debug Low;
  static const Debug Medium;
  static const Debug High;
  static const Debug None;
};

/**
 * prints or discards output according to the current
 * debug level
 */
class DebugStreambuf
: public FilterStreambuf {
private:
  /// current debug level
  size_t level;

  /// flag if output will be printend or not
  bool visible;
  
public:
  /// constructs a new object with the specified debug
  /// level
  DebugStreambuf(
      const Debug &dbg = Debug::Low,
      bool visible = true,
      std::streambuf *next = 0);
  
  /// set a new debug level
  void setLevel(const Debug &dbg);
  
  /// set the visibility based on the current debug
  /// level
  void setVisibility(const Debug &dbg);
  
protected:
  int overflow(int c);
  
public:
  /// index obtained with std::ostream::xalloc
  static const int index;
 
  /// see Debug 
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the Debug manipulator
std::ostream &operator<<(std::ostream &os, const Debug &d);

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_DEBUGSTREAMBUF_HPP
