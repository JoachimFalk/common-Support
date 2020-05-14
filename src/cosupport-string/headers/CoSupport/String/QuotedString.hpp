// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STRING_QUOTEDSTRING_HPP
#define _INCLUDED_COSUPPORT_STRING_QUOTEDSTRING_HPP

#include <iostream>
#include <string>

#include "UnQuotedString.hpp"
#include "SingleQuotedString.hpp"
#include "DoubleQuotedString.hpp"

#include "export_config.h"

namespace CoSupport { namespace String {

class COSUPPORT_STRING_API
QuotedString;

COSUPPORT_STRING_API
std::istream &operator >>(std::istream &, QuotedString &);

COSUPPORT_STRING_API
std::ostream &operator <<(std::ostream &, const QuotedString &);

class COSUPPORT_STRING_API
QuotedString
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
