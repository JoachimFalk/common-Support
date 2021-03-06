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

#ifndef _INCLUDED_COSUPPORT_STREAMS_INDENTSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_INDENTSTREAMBUF_HPP

#include <stddef.h>

#include "FilterStreambuf.hpp"
#include "FilterOStream.hpp"

#include "export_config.h"

namespace CoSupport { namespace Streams {

/**
 * stream manipulator for the IndentStreambuf custom stream
 * buffer
 */
struct COSUPPORT_STREAMS_API
Indent {
  /// how many levels to indent
  int delta_level;
  
  /// constructs a new object with the specified value
  Indent(int delta_level);
  
  /// predefined indentation values
  static const Indent Up;
  static const Indent Down;
};

/**
 * prefixes each line with spaces
 */
class COSUPPORT_STREAMS_API
IndentStreambuf
: public FilterStreambuf {
public:
  template <class Base = FilterOStream> class Stream;
public:
  /// constructs a new object with the specified delta and
  /// initial indentation
  IndentStreambuf(
      size_t delta = 2,
      size_t indent = 0,
      std::streambuf *next = 0);

  /// modify the indentation (absolute value)
  void setIndentation(int value);

  /// modify the indentation (delta)
  void setDeltaLevel(int value);

  /// index obtained with std::ostream::xalloc
  static const int index;
protected:
  int overflow(int c);

  /// see Indent
  bool hasManip() const;

  /// returns the (static) index
  int getIndex() const;
private:
  /// number of spaces to add for each indentation
  /// level
  size_t delta;

  /// current number of spaces to print for each line
  size_t indent;

  /// indicator if newline was encountered
  bool newline;
};

template <class Base>
class IndentStreambuf::Stream: public Base {
public:
  /// construct a new object which uses the streambuffer
  /// of the specified stream as initial target
  Stream(std::ostream &os)
    : Base(os) { this->insert(indenter); }
private:
  IndentStreambuf indenter;
};

/// output operator for the Indent manipulator
COSUPPORT_STREAMS_API
std::ostream &operator << (std::ostream &os, const Indent &i);

/**
 * indents specified stream at the point of instantiation;
 * reverses indentation if it goes out of scope
 */
class COSUPPORT_STREAMS_API
ScopedIndent {
private:
  std::ostream &out;
  int delta;
public:
  // constructs a new object wich indents the specified stream
  ScopedIndent(std::ostream &out, const Indent &indent = Indent::Up);

  // destructor reverses indentation
  ~ScopedIndent();
};

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_INDENTSTREAMBUF_HPP
