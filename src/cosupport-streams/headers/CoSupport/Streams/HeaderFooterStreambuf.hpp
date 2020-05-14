// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Jens Gladigau <jens.gladigau@cs.fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STREAMS_HEADERFOOTERSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_HEADERFOOTERSTREAMBUF_HPP

#include <string>

#include "FilterStreambuf.hpp"

#include "export_config.h"

namespace CoSupport { namespace Streams {

/**
 * stream manipulator for the HeaderFooterStreambuf
 * streambuffer (set a new header)
 */
struct COSUPPORT_STREAMS_API
Header {
  /// new header
  std::string value;
  
  /// flag if value (and following chars)
  /// should be added to the header
  bool add;
  
  /// constructs a new Header object
  Header(const std::string &value, bool add = false);
  
  /// constructs a new Header object
  Header(const char *value, bool add = false);
  
  /// predefined headers
  static const Header Start;
  static const Header End;
};

/**
 * stream manipulator for the HeaderFooterStreambuf
 * streambuffer (set a new footer)
 */
struct COSUPPORT_STREAMS_API
Footer {
  // new footer
  std::string value;
  
  /// flag if value (and following chars)
  /// should be added to the header
  bool add;
  
  /// constructs a new Footer object
  Footer(const std::string &value, bool add = false);
  
  /// constructs a new Footer object
  Footer(const char *value, bool add = false);
  
  /// predefined footers
  static const Footer Start;
  static const Footer End;
};

/**
 * prefixes each line with a custom header string and
 * appends to each line a custom footer string
 */
class COSUPPORT_STREAMS_API
HeaderFooterStreambuf
: public FilterStreambuf {
public:
  template <class Base = FilterOStream> class Stream;
public:
  /// constructs a new object with the specified header
  /// and footer
  HeaderFooterStreambuf(
      const std::string &header = "",
      const std::string &footer = "",
      bool add_header = false,
      bool add_footer = false,
      std::streambuf *next = 0);
  
  /// returns the current header
  const std::string &getHeader() const;
  
  /// sets a new header
  void setHeader(const std::string &value);
  
  /// returns the current footer
  const std::string &getFooter() const;
  
  /// sets a new footer
  void setFooter(const std::string &value);

  /// set flag if following chars will be appendend
  /// to the current header
  void setAddHeader(bool value);

  /// set flag if following chars will be appendend
  /// to the current footer
  void setAddFooter(bool value);
  
  /// index obtained with std::ostream::xalloc
  static const int index;
protected:
  int overflow(int c);
  
  /// see Header and Footer
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;

private:
  /// currently used header string
  std::string header;

  /// currently used footer string
  std::string footer;
  
  /// indicator if chars should be added to header
  bool add_header;

  /// indicator if chars should be added to footer
  bool add_footer;
  
  /// indicator if newline was encountered 
  bool newline;
};

template <class Base>
class HeaderFooterStreambuf::Stream: public Base {
public:
  /// construct a new object which uses the streambuffer
  /// of the specified stream as initial target
  Stream(std::ostream &os)
    : Base(os) { this->insert(headerFooter); }

  // FIXME: changing the interface broke our stuff; debugStream.setHeader(...) is not working
  // though, back to public
  //private:
  HeaderFooterStreambuf headerFooter;
};

/// output operator for the Header manipulator
COSUPPORT_STREAMS_API
std::ostream &operator<<(std::ostream &os, const Header &p);

/// output operator for the Footer manipulator
COSUPPORT_STREAMS_API
std::ostream &operator<<(std::ostream &os, const Footer &p);

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_HEADERFOOTERSTREAMBUF_HPP
