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

#ifndef _INCLUDED_COSUPPORT_STRING_QUOTEDSTRING_HPP
#define _INCLUDED_COSUPPORT_STRING_QUOTEDSTRING_HPP

#include <iostream>
#include <string>

#include "UnQuotedString.hpp"
#include "SingleQuotedString.hpp"
#include "DoubleQuotedString.hpp"

namespace CoSupport { namespace String {

class QuotedString;

std::istream &operator >>(std::istream &, QuotedString &);
std::ostream &operator <<(std::ostream &, const QuotedString &);

class QuotedString
: public std::string {
  typedef QuotedString this_type;

  friend std::istream &operator >>(std::istream &, QuotedString &);
  friend std::ostream &operator <<(std::ostream &, const QuotedString &);
public:
  // Pass constructor to std::string
  QuotedString()
    : std::string() {}
  QuotedString(const allocator_type &a)
    : std::string(a) {}
  QuotedString(const std::string &str)
    : std::string(str) {}
  QuotedString(const std::string &str, size_type pos, size_type n = this_type::npos)
    : std::string(str,pos,n) {}
  QuotedString(const std::string &str, size_type pos, size_type n, const allocator_type &a)
    : std::string(str,pos,n,a) {}
  QuotedString(const char *str, size_type n, const allocator_type &a = allocator_type())
    : std::string(str,n,a) {}
  QuotedString(const char *str, const allocator_type &a = allocator_type())
    : std::string(str,a) {}
  QuotedString(size_type n, char c, const allocator_type &a = allocator_type())
    : std::string(n,c,a) {}
  template<class InputIterator>
  QuotedString(InputIterator b, InputIterator e, const allocator_type &a = allocator_type())
    : std::string(b,e,a) {}

};

} } // namespace CoSupport::String

#endif // _INCLUDED_COSUPPORT_STRING_QUOTEDSTRING_HPP
