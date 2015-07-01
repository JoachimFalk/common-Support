// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2011 Hardware-Software-CoDesign, University of
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

#include <CoSupport/Streams/HeaderFooterStreambuf.hpp>

namespace CoSupport { namespace Streams {

Header::Header(const std::string &value, bool add)
  : value(value),
    add(add) {}

Header::Header(const char *value, bool add)
  : value(value),
    add(add) {}

const Header Header::Start("", true);
const Header Header::End("", false);

Footer::Footer(const std::string &value, bool add)
  : value(value),
    add(add) {}

Footer::Footer(const char *value, bool add)
  : value(value),
    add(add) {}

const Footer Footer::Start("", true);
const Footer Footer::End("", false);

HeaderFooterStreambuf::HeaderFooterStreambuf(
    const std::string &header,
    const std::string &footer,
    bool add_header,
    bool add_footer,
    std::streambuf *next)
  : FilterStreambuf(next),
    header(header),
    footer(footer),
    add_header(add_header),
    add_footer(add_footer),
    newline(true)
{
  //std::cerr << "Enter HeaderFooterStreambuf::HeaderFooterStreambuf" << std::endl;
  //std::cerr << "Leave HeaderFooterStreambuf::HeaderFooterStreambuf" << std::endl;
}

const std::string &HeaderFooterStreambuf::getHeader() const
  { return header; }

void HeaderFooterStreambuf::setHeader(const std::string &value) {
  if (add_header)
    header += value;
  else
    header = value;
}

const std::string &HeaderFooterStreambuf::getFooter() const
  { return footer; }

void HeaderFooterStreambuf::setFooter(const std::string &value) {
  if (add_footer)
    footer += value;
  else
    footer = value;
}

void HeaderFooterStreambuf::setAddHeader(bool value)
  { add_header = value; }

void HeaderFooterStreambuf::setAddFooter(bool value)
  { add_footer = value; }

int HeaderFooterStreambuf::overflow(int c) {
  if (add_header || add_footer) {
    if (add_header)
      header += c;
    if (add_footer)
      footer += c;
    return 1;
  }
  if (newline) {
    next->sputn(header.c_str(), header.size());
    newline = false;
  }
  if (!newline && c == '\n') {
    next->sputn(footer.c_str(), footer.size());
    newline = true;
  } 
  return next->sputc(c);
}

bool HeaderFooterStreambuf::hasManip() const
  { return true; }

int HeaderFooterStreambuf::getIndex() const
  { return index; }

const int HeaderFooterStreambuf::index(std::ostream::xalloc());

std::ostream &operator << (std::ostream &os, const Header &p) {
  HeaderFooterStreambuf *buf =
    static_cast<HeaderFooterStreambuf *>(os.pword(HeaderFooterStreambuf::index));
  if (buf) {
    buf->setHeader(p.value);
    buf->setAddHeader(p.add);
  }
  return os;
}

std::ostream &operator << (std::ostream &os, const Footer &p) {
  HeaderFooterStreambuf *buf =
    static_cast<HeaderFooterStreambuf *>(os.pword(HeaderFooterStreambuf::index));
  if (buf) {
    buf->setFooter(p.value);
    buf->setAddFooter(p.add);
  }
  return os;
}

} } // namespace CoSupport::Streams
