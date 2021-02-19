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
#include <cstring>

#include <CoSupport/String/QuotedString.hpp>
#include <CoSupport/String/quoting.hpp>

namespace CoSupport { namespace String {

std::istream &operator >>(std::istream &in, QuotedString &dst) {
  std::istream::sentry sentry(in, false);
  
  if (sentry)
    dequote(dst, in, QuoteMode::AUTO);
  return in;
}

std::ostream &operator <<(std::ostream &out, const QuotedString &src) {
  std::ostream::sentry sentry(out);
  
  if (sentry) {
    bool needQuote = false;
    bool hasCtrl   = false;
    for (QuotedString::size_type n = 0;
         n < src.length() && !hasCtrl;
         ++n) {
      if (src[n] >= 'a' && src[n] <= 'z')
        continue;
      if (src[n] >= 'A' && src[n] <= 'Z')
        continue;
      if (src[n] >= '0' && src[n] <= '9')
        continue;
      if (strchr("_.,:-+/", src[n]))
        continue;
      needQuote = true;
      hasCtrl   |= !isprint(src[n]) || src[n] == '\'';
    }
    if (!needQuote && !src.empty())
      out << reinterpret_cast<const UnQuotedString &>(src);
    else if (!hasCtrl)
      out << reinterpret_cast<const SingleQuotedString &>(src);
    else
      out << reinterpret_cast<const DoubleQuotedString &>(src);
  }
  return out;
}

} } // namespace CoSupport::String
