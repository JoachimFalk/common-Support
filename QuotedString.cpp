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

#include <iostream>
#include <string>

#include <CoSupport/String/QuotedString.hpp>

namespace CoSupport { namespace String {

std::istream &operator >>(std::istream &in, QuotedString &dst) {
  std::istream::sentry sentry(in, false);
  
  if (sentry) {
    int ch = in.peek();
    if (ch == '\'')
      in >> reinterpret_cast<SingleQuotedString &>(dst);
    else if (ch == '"')
      in >> reinterpret_cast<DoubleQuotedString &>(dst);
    else
      in >> reinterpret_cast<UnQuotedString &>(dst);
  }
  return in;
}

std::ostream &operator <<(std::ostream &out, const QuotedString &src) {
  std::ostream::sentry sentry(out);
  
  if (sentry) {
    bool hasSpace = false;
    bool hasCtrl  = false;
    for (QuotedString::size_type n = 0;
         n < src.length() && !hasCtrl;
         ++n) {
      hasSpace |= isspace(src[n]);
      hasCtrl  |= !isprint(src[n]) || src[n] == '\'';
    }
    if (!hasCtrl) {
      if (!hasSpace)
        out << reinterpret_cast<const UnQuotedString &>(src);
      else
        out << reinterpret_cast<const SingleQuotedString &>(src);
    } else
      out << reinterpret_cast<const DoubleQuotedString &>(src);
  }
  return out;
}

} } // namespace CoSupport::String
