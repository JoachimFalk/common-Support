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

#ifndef _INCLUDED_COSUPPORT_STREAMS_DEBUGSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_DEBUGSTREAMBUF_HPP

#include <stddef.h>

#include <string>

#include "FilterStreambuf.hpp"
#include "FilterOStream.hpp"
#include <CoSupport/commondefs.h>

#include "export_config.h"

namespace CoSupport { namespace Streams {

/**
 * stream manipulator for the DebugStreambuf custom
 * streambuffer
 */
struct COSUPPORT_STREAMS_API
Debug {
  /// new debug level
  size_t level;

  /// new line will be prefixed with msg
  std::string prefix;

  /// constructs a new object with the specified debug
  /// level
  Debug(size_t level);

  /// constructs a new object with the specified debug
  /// level and prefix
  Debug(size_t level, const std::string &prefix);

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
class COSUPPORT_STREAMS_API
DebugStreambuf
: public FilterStreambuf {
public:
  template <class Base = FilterOStream> class Stream;
public:
  /// constructs a new object with the specified debug level
  DebugStreambuf(
      const Debug &dbg = Debug::Low,
      bool visible = true,
      std::streambuf *next = 0);

//void setLevel(const Debug &dbg) COSUPPORT_ATTRIBUTE_DEPRECATED
//  { setStreamLevel(dbg); }
  /// Set a new stream debug level.
  void setStreamLevel(const Debug &dbg);
  /// Set a new output debug level.
  /// \param dbg
  ///   A debug level to check against the debug level set via setStreamLevel.
  ///   If dbg is greater than the debug level set via setStreamLevel, then show output.
  void setOutputLevel(const Debug &dbg);
  /// Swap in a new output debug level.
  /// \param dbg
  ///   A new output debug level.
  /// \return
  ///   The old output debug level.
  Debug swapOutputLevel(const Debug &dbg);

  /// Would the output debug level dbg be visible on the stream?
  bool isVisible(Debug const &dbg) const;

  /// index obtained with std::ostream::xalloc
  static const int index;
protected:
  int overflow(int c);

  /// see Debug 
  bool hasManip() const;

  /// returns the (static) index
  int getIndex() const;
private:
  /// current stream debug level
  size_t streamLevel;
  /// current output debug level
  size_t outputLevel;

  /// line prefix
  std::string prefix;

  /// indicator if newline was encountered 
  bool newline;
};

template <class Base>
class DebugStreambuf::Stream: public Base {
public:
  /// construct a new object which uses the streambuffer
  /// of the specified stream as initial target
  Stream(std::ostream &os, const Debug &dbg = Debug::Low)
    : Base(os), debuglevel(dbg) { this->insert(debuglevel); }

  /// set a new debug level
  void setLevel(const Debug &dbg)
    { return debuglevel.setStreamLevel(dbg); }

  /// Would the output debug level dbg be visible on the stream?
  bool isVisible(Debug const &dbg) const
    { return debuglevel.isVisible(dbg); }
private:
  DebugStreambuf debuglevel;
};

/// output operator for the Debug manipulator
std::ostream &operator << (std::ostream &os, const Debug &d);

/**
 * Sets output debug level dbg for stream out;
 * reverses debug level if it goes out of scope
 */
class COSUPPORT_STREAMS_API
ScopedDebug {
private:
  DebugStreambuf *dbgBuf;
  Debug           oldDbg;
public:
  // constructs a new object which sets output debug level.
  ScopedDebug(std::ostream &out, const Debug &dbg);

  // destructor reverses debug level
  ~ScopedDebug();
};

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_DEBUGSTREAMBUF_HPP
