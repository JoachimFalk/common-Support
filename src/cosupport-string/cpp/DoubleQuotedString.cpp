// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Tobias Schwarzer <tobias.schwarzer@fau.de>
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
              case 'a': // Alert (Beep, Bell) (added in C89)
                ch = '\a'; break;
              case 'b': // Backspace
                ch = '\b'; break;
              case 'e': // Escape character
                ch = '\x1B'; break;
              case 'f': // Formfeed Page Break
                ch = '\f'; break;
              case 'n': // Newline (Line Feed)
                ch = '\n'; break;
              case 'r': // Carriage Return
                ch = '\r'; break;
              case 't': // Horizontal Tab
                ch = '\t'; break;
              case 'v': // Vertical Tab
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
              case '0': case '1': case '2': case '3':
              case '4': case '5': case '6': case '7': // oct
                in.unget();
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
          case '\a': // Alert (Beep, Bell) (added in C89)
            out << "\\a"; break;
          case '\b': // Backspace
            out << "\\b"; break;
//        case '\x1B': // Escape character
//          out << "\\e"; break;
          case '\f': // Formfeed Page Break
            out << "\\f"; break;
          case '\n': // Newline (Line Feed)
            out << "\\n"; break;
          case '\r': // Carriage Return
            out << "\\r"; break;
          case '\t': // Horizontal Tab
            out << "\\t"; break;
          case '\v': // Vertical Tab
            out << "\\v"; break;
          case '"':
            out << "\\\""; break;
          case '\\':
            out << "\\\\"; break;
          // Paranoia, in case this string finds a way to a shell!
          case '$':
            out << "\\x24"; break;
          case '`':
            out << "\\x60"; break;
          case '!':
            out << "\\x21"; break;
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
