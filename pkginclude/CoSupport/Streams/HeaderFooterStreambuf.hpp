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

#ifndef _INCLUDED_COSUPPORT_STREAMS_HEADERFOOTERSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_HEADERFOOTERSTREAMBUF_HPP

#include <ostream>

#include "FilterStreambuf.hpp"

namespace CoSupport { namespace Streams {

/**
 * stream manipulator for the HeaderFooterStreambuf
 * streambuffer (set a new header)
 */
struct Header {
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
struct Footer {
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
class HeaderFooterStreambuf
: public FilterStreambuf {
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
  
protected:
  int overflow(int c);
  
public:
  /// index obtained with std::ostream::xalloc
  static const int index;
   
  /// see Header and Footer
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the Header manipulator
std::ostream &operator<<(std::ostream &os, const Header &p);

/// output operator for the Footer manipulator
std::ostream &operator<<(std::ostream &os, const Footer &p);

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_HEADERFOOTERSTREAMBUF_HPP
