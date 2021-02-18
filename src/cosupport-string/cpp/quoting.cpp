// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2021 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/String/quoting.hpp>

#include <cstdlib>
#include <climits>
#include <cassert>

#include <iostream>

namespace CoSupport { namespace String {

namespace {

  class CharRange {
  public:
    CharRange(const char *in, const char *end)
      : in(in), end(end) {}

    int get()
      { return in == end ? EOF : (unsigned char) *in++; }
    int peek() const
      { return in == end ? EOF : (unsigned char) *in; }
    void unget()
      { --in; }

    const char *in;
    const char *end;
  };

  class CString {
  public:
    CString(const char *in)
      : in(in) {}

    int get()
      { return !*in ? EOF : (unsigned char) *in++; }
    int peek() const
      { return !*in ? EOF : (unsigned char) *in; }
    void unget()
      { --in; }

    const char *in;
  };

  class WhiteSpaceDelim {
  public:
    bool operator()(int ch) const
      { return isspace(ch); }
  };

  class StreamWhiteSpaceDelim {
  public:
    StreamWhiteSpaceDelim(std::istream const &in)
      : in(in) {}

    bool operator()(int ch) const
      { return isspace(static_cast<char>(ch), in.getloc()); }
  private:
    std::istream const &in;
  };

  template <typename INPUT, typename DELIM>
  bool dequoteImpl(std::string &dst, INPUT in, DELIM delim, QuoteMode qm) {
    int quote = EOF;

    switch (qm) {
      case QuoteMode::DOUBLE_WITH_QUOTES: quote = '"';  break;
      case QuoteMode::SINGLE_WITH_QUOTES: quote = '\''; break;
      default: break;
    }

    int ch = in.get();

    if (quote != EOF) {
      if (ch !=  quote) {
        if (ch != EOF)
          in.unget();
        return false;
      } else
        ch = in.get();
    }

    dst.clear();

    QuoteMode qmActive = qm;

    while (ch != EOF) {
      switch (qmActive) {
        case QuoteMode::DOUBLE_WITH_QUOTES:
        case QuoteMode::DOUBLE_NO_QUOTES:
          while (ch != EOF && (ch != '"' || qmActive == QuoteMode::DOUBLE_NO_QUOTES)) {
            switch (ch) {
              case '\\': {
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
                        return false;
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
                      return false;
                    break;
                  default:
                    if (ch == EOF || !ispunct(ch))
                      return false;
                    break;
                }
                dst.append(1, ch);
                break;
              }
              default:
                dst.append(1, ch);
                break;
            }
            ch = in.get();
          }
          if (qm != QuoteMode::AUTO || ch == EOF)
            goto EndLoop;
          qmActive = QuoteMode::AUTO;
          quote    = EOF;
          ch       = in.get();
          break;
        case QuoteMode::SINGLE_WITH_QUOTES:
        case QuoteMode::SINGLE_NO_QUOTES:
          while (ch != EOF && (ch != '\'' || qmActive == QuoteMode::SINGLE_NO_QUOTES)) {
            dst.append(1, ch);
            ch = in.get();
          }
          if (qm != QuoteMode::AUTO || ch == EOF)
            goto EndLoop;
          qmActive = QuoteMode::AUTO;
          quote    = EOF;
          ch       = in.get();
          break;
        case QuoteMode::AUTO:
          switch (ch) {
            case '\\':
              ch = in.get();
              if (ch == EOF)
                return false;
              else if (ch != '\n')
                dst.append(1, ch);
              break;
            case '"':
              qmActive = QuoteMode::DOUBLE_WITH_QUOTES;
              quote = '"';
              break;
            case '\'':
              qmActive = QuoteMode::SINGLE_WITH_QUOTES;
              quote = '\'';
              break;
            default:
              if (delim(ch)) {
                in.unget();
                goto EndLoop;
              } else
                dst.append(1, ch);
              break;
          }
          ch = in.get();
          break;
        default:
          assert(!"Oops, unknown quote mode!");
          return false;
      }
    }
EndLoop:
    if (quote != EOF && ch != quote) {
      return false;
    }
    return true;
  }

} // anonymous namespace

bool dequote(std::string &str, const char *&in, const char *end, QuoteMode qm) throw() {
  CharRange inStream(in, end);
  bool      success = dequoteImpl<CharRange &>(str, inStream, WhiteSpaceDelim(), qm);
  in = inStream.in;
  return success;
}

bool dequote(std::string &str, const char *in, const char *end, QuoteMode qm) throw() {
  return dequoteImpl(str, CharRange(in, end), WhiteSpaceDelim(), qm);
}

bool dequote(std::string &str, const char *&in, QuoteMode qm) throw() {
  CString inStream(in);
  bool    success = dequoteImpl<CString &>(str, inStream, WhiteSpaceDelim(), qm);
  in = inStream.in;
  return success;
}

bool dequote(std::string &str, const char *in, QuoteMode qm) throw() {
  return dequoteImpl(str, CString(in), WhiteSpaceDelim(), qm);
}

bool dequote(std::string &str, std::istream &in, QuoteMode qm) throw() {
  try {
    bool success = dequoteImpl<std::istream &>(str, in, StreamWhiteSpaceDelim(in),  qm);
    if (!success)
      in.setstate(std::ios_base::failbit);
    else if (in.eof() && !in.bad())
      in.clear();
    return success;
  } catch (...) {
    in.setstate(std::ios_base::badbit);
    return false;
  }
}

} } // namespace CoSupport::String
