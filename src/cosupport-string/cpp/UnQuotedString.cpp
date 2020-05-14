// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <iostream>
#include <string>
#include <ctype.h>

#include <CoSupport/String/UnQuotedString.hpp>

namespace CoSupport { namespace String {

std::ostream &operator <<(std::ostream &out, const UnQuotedString &src) {
  std::ostream::sentry sentry(out);
  
  if (sentry) {
    try {
      for (UnQuotedString::size_type n = 0; n < src.length(); ++n) {
        if (isspace(src[n])) {
          out.setstate(std::ios_base::failbit);
          return out;
        }
      }
      out << static_cast<const std::string &>(src);
    } catch (...) {
      out.setstate(std::ios_base::badbit);
    }
  }
  return out;
}

} } // namespace CoSupport::String
