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
#include <cstdio> // for EOF
#include <limits.h>
#include <ctype.h>

#include <CoSupport/String/DoubleQuotedString.hpp>

namespace CoSupport { namespace String {

std::istream &operator >>(std::istream &in, DoubleQuotedString &dst) {
  std::istream::sentry sentry(in, false);
  
  if (sentry) {
    try {
      int ch = in.get();
      if (ch == '"') {
        dst.clear();
        while ((ch = in.get()) != EOF && ch != '"') {
          if (ch == '\\') {
            switch (ch = in.get()) {
              case 'n':
                ch = '\n'; break;
              case 't':
                ch = '\t'; break;
              case 'f':
                ch = '\f'; break;
              case 'r':
                ch = '\r'; break;
              case 'v':
                ch = '\v'; break;
              case 'x': { // hex
                ch = 0;
                for (int i = 0; i < (CHAR_BIT+3)/4; ++i) {
                  int hex = in.get();
                  // FIXME:
                  if (hex >= '0' && hex <= '9')
                    ch = (ch << 4) | (hex - '0');
                  else if (hex >= 'A' && hex <= 'F')
                    ch = (ch << 4) | (hex - 'A' + 10);
                  else if (hex >= 'a' && hex <= 'f')
                    ch = (ch << 4) | (hex - 'a' + 10);
                  else if (i == 0)
                    goto error;
                  else {
                    if (hex != EOF)
                      in.unget();
                    break;
                  }
                }
                break;
              }
              case '0': // oct
                ch = 0;
                for (int i = 0; i < (CHAR_BIT+2)/3; ++i) {
                  int oct = in.get();
                  // FIXME:
                  if (oct >= '0' && oct <= '7')
                    ch = (ch << 3) | (oct - '0');
                  else {
                    if (oct != EOF)
                      in.unget();
                    break;
                  }
                }
                if (ch > UCHAR_MAX)
                  goto error;
                break;
              default:
                if (ch == EOF || !ispunct(ch))
                  goto error;
                break;
            }
          }
          dst.append(1, ch);
        }
        if (ch != '"')
error:
          in.setstate(std::ios_base::badbit);
      } else {
        if (ch != EOF)
          in.unget();
        in.setstate(std::ios_base::failbit);
      }
    } catch (...) {
      in.setstate(std::ios_base::badbit);
    }
  }
  return in;
}

std::ostream &operator <<(std::ostream &out, const DoubleQuotedString &src) {
  std::ostream::sentry sentry(out);
  
  if (sentry) {
    try {
      out << '"';
      for (DoubleQuotedString::size_type n = 0; out.good() && n < src.length(); ++n) {
        switch (src[n]) {
          case '\n':
            out << "\\n"; break;
          case '\t':
            out << "\\t"; break;
          case '\f':
            out << "\\f"; break;
          case '\r':
            out << "\\r"; break;
          case '\v':
            out << "\\v"; break;
          case '"':
            out << "\\\""; break;
          default:
            if (isprint(src[n]))
              out << src[n];
            else {
              unsigned int shift = static_cast<unsigned char>(src[n]);
              out << "\\x";
              for (int i = (CHAR_BIT+3)/4 - 1; i >= 0; --i) {
                int digit = (shift >> 4*i) & 0xF;
                out.put(digit < 10 ? '0' + digit : 'A' + digit - 10);
              }
            }
            break;
        }
      }
      out << '"';
      if (!out.good())
        out.setstate(std::ios_base::badbit);
    } catch (...) {
      out.setstate(std::ios_base::badbit);
    }
  }
  return out;
}

} } // namespace CoSupport::String
