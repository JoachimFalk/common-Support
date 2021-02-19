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
      : facet(std::use_facet<std::ctype<char> >(in.getloc())) {}

    bool operator()(int ch) const
      { return facet.is(std::ctype<char>::space, static_cast<char>(ch)); }
  private:
    std::ctype<char> const &facet;
  };

  template <typename INPUT, typename DELIM>
  DequotingStatus dequoteImpl(std::string &dst, INPUT in, DELIM delim, QuoteMode qm) {
    int ch = in.get();

    switch (qm) {
      case QuoteMode::DOUBLE_WITH_QUOTES:
        if (ch != '"') {
          if (ch != EOF)
            in.unget();
          return DequotingStatus::MISSING_OPENING_DOUBLE_QUOTE;
        }
        ch = in.get();
        break;
      case QuoteMode::SINGLE_WITH_QUOTES:
        if (ch != '\'') {
          if (ch != EOF)
            in.unget();
          return DequotingStatus::MISSING_OPENING_SINGLE_QUOTE;
        }
        ch = in.get();
        break;
      default:
        break;
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
                        return DequotingStatus::HEX_ESCAPE_WITHOUT_HEX_DIGIT;
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
                      return DequotingStatus::OCT_ESCAPE_EXCEEDS_CHAR_RANGE;
                    break;
                  default:
                    if (ch == EOF)
                      return DequotingStatus::MISSING_ESCAPE_CHAR;
                    if (!ispunct(ch))
                      return DequotingStatus::ILLEGAL_ESCAPE_CHAR;
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
          ch       = in.get();
          break;
        case QuoteMode::AUTO:
          switch (ch) {
            case '\\':
              ch = in.get();
              if (ch == EOF)
                return DequotingStatus::MISSING_ESCAPE_CHAR;
              else if (ch != '\n')
                dst.append(1, ch);
              break;
            case '"':
              qmActive = QuoteMode::DOUBLE_WITH_QUOTES;
              break;
            case '\'':
              qmActive = QuoteMode::SINGLE_WITH_QUOTES;
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
          return DequotingStatus::GENERIC_ERROR;
      }
    }
EndLoop:
    switch (qmActive) {
      case QuoteMode::DOUBLE_WITH_QUOTES:
        if (ch != '"')
          return DequotingStatus::MISSING_CLOSING_DOUBLE_QUOTE;
        break;
      case QuoteMode::SINGLE_WITH_QUOTES:
        if (ch != '\'')
          return DequotingStatus::MISSING_CLOSING_SINGLE_QUOTE;
        break;
      default:
        break;
    }
    return DequotingStatus::OK;
  }

  static
  std::string composeErrorMessage(DequotingStatus error, const char *from, const char *to) {
    std::string msg;
    switch (error) {
      case DequotingStatus::GENERIC_ERROR:
        msg = "Generic dequoting error";
        break;
      case DequotingStatus::MISSING_OPENING_SINGLE_QUOTE:
        msg = "Expecting opening single quote \"'\"";
        break;
      case DequotingStatus::MISSING_CLOSING_SINGLE_QUOTE:
        msg = "Expecting closing single quote \"'\"";
        break;
      case DequotingStatus::MISSING_OPENING_DOUBLE_QUOTE:
        msg = "Expecting opening double quote '\"'";
        break;
      case DequotingStatus::MISSING_CLOSING_DOUBLE_QUOTE:
        msg = "Expecting closing double quote '\"'";
        break;
      case DequotingStatus::HEX_ESCAPE_WITHOUT_HEX_DIGIT:
        msg = "Expecting hex digits after \\x escape sequence";
        break;
      case DequotingStatus::OCT_ESCAPE_EXCEEDS_CHAR_RANGE:
        msg = "Octal escape must encode character within char range";
        break;
      case DequotingStatus::MISSING_ESCAPE_CHAR:
        msg = "Expecting escaped character after escape char, i.e, '\\'";
        break;
      case DequotingStatus::ILLEGAL_ESCAPE_CHAR:
        msg = "Illegal escape sequence";
        break;
      default:
        assert(error != DequotingStatus::OK);
        assert(!"Unknown DequotingStatus!");
        return "Internal error: unknown DequotingStatus!";
    }
    if (from != nullptr && from != to) {
      assert(to != nullptr);
      msg.append(": ");
      msg.append(from, to);
      msg.append(" <= HERE!");
    } else {
      msg.append("!");
    }
    return msg;
  }

} // anonymous namespace

std::ostream &operator <<(std::ostream &out, DequotingStatus status) {
  switch (status) {
    case DequotingStatus::OK:
      return out << "DequotingStatus::OK";
    case DequotingStatus::GENERIC_ERROR:
      return out << "DequotingStatus::GENERIC_ERROR";
    case DequotingStatus::MISSING_OPENING_SINGLE_QUOTE:
      return out << "DequotingStatus::MISSING_OPENING_SINGLE_QUOTE";
    case DequotingStatus::MISSING_CLOSING_SINGLE_QUOTE:
      return out << "DequotingStatus::MISSING_CLOSING_SINGLE_QUOTE";
    case DequotingStatus::MISSING_OPENING_DOUBLE_QUOTE:
      return out << "DequotingStatus::MISSING_OPENING_DOUBLE_QUOTE";
    case DequotingStatus::MISSING_CLOSING_DOUBLE_QUOTE:
      return out << "DequotingStatus::MISSING_CLOSING_DOUBLE_QUOTE";
    case DequotingStatus::HEX_ESCAPE_WITHOUT_HEX_DIGIT:
      return out << "DequotingStatus::HEX_ESCAPE_WITHOUT_HEX_DIGIT";
    case DequotingStatus::OCT_ESCAPE_EXCEEDS_CHAR_RANGE:
      return out << "DequotingStatus::OCT_ESCAPE_EXCEEDS_CHAR_RANGE";
    case DequotingStatus::MISSING_ESCAPE_CHAR:
      return out << "DequotingStatus::MISSING_ESCAPE_CHAR";
    case DequotingStatus::ILLEGAL_ESCAPE_CHAR:
      return out << "DequotingStatus::ILLEGAL_ESCAPE_CHAR";
    default:
      assert(!"Unknown DequotingStatus!");
      return out;
  }
}

DequotingError::DequotingError(DequotingStatus error, const char *from, const char *to)
  : std::runtime_error(composeErrorMessage(error, from, to)), error(error) {}

DequotingStatus dequote(std::string &str, const char *&in, const char *end, QuoteMode qm) throw() {
  CharRange inStream(in, end);
  DequotingStatus status = dequoteImpl<CharRange &>(str, inStream, WhiteSpaceDelim(), qm);
  in = inStream.in;
  return status;
}

std::string dequote(const char *&in, const char *end, QuoteMode qm) {
  std::string str;
  CharRange inStream(in, end);
  DequotingStatus status = dequoteImpl<CharRange &>(str, inStream, WhiteSpaceDelim(), qm);
  const char *start = in;
  in = inStream.in;
  if (status != DequotingStatus::OK)
    throw DequotingError(status, start, inStream.in);
  return str;
}

DequotingStatus dequote(std::string &str, QuoteMode qm, const char *in, const char *end) throw() {
  return dequoteImpl(str, CharRange(in, end), WhiteSpaceDelim(), qm);
}

std::string dequote(QuoteMode qm, const char *in, const char *end) {
  std::string str;
  CharRange inStream(in, end);
  DequotingStatus status = dequoteImpl<CharRange &>(str, inStream, WhiteSpaceDelim(), qm);
  if (status != DequotingStatus::OK)
    throw DequotingError(status, in, inStream.in);
  return str;
}

DequotingStatus dequote(std::string &str, const char *&in, QuoteMode qm) throw() {
  CString inStream(in);
  DequotingStatus status = dequoteImpl<CString &>(str, inStream, WhiteSpaceDelim(), qm);
  in = inStream.in;
  return status;
}

std::string  dequote(const char *&in, QuoteMode qm) {
  std::string str;
  CString inStream(in);
  DequotingStatus status = dequoteImpl<CString &>(str, inStream, WhiteSpaceDelim(), qm);
  const char *start = in;
  in = inStream.in;
  if (status != DequotingStatus::OK)
    throw DequotingError(status, start, inStream.in);
  return str;
}

DequotingStatus dequote(std::string &str, QuoteMode qm, const char *in) throw() {
  return dequoteImpl(str, CString(in), WhiteSpaceDelim(), qm);
}

std::string dequote(QuoteMode qm, const char *in) {
  std::string str;
  CString inStream(in);
  DequotingStatus status = dequoteImpl<CString &>(str, inStream, WhiteSpaceDelim(), qm);
  if (status != DequotingStatus::OK)
    throw DequotingError(status, in, inStream.in);
  return str;
}

DequotingStatus dequote(std::string &str, std::istream &in, QuoteMode qm) throw() {
  try {
    DequotingStatus status = dequoteImpl<std::istream &>(str, in, StreamWhiteSpaceDelim(in),  qm);
    if (status != DequotingStatus::OK)
      in.setstate(std::ios_base::failbit);
    else if (in.eof() && !in.bad())
      in.clear();
    return status;
  } catch (...) {
    in.setstate(std::ios_base::badbit);
    return DequotingStatus::GENERIC_ERROR;
  }
}

std::string dequote(std::istream &in, QuoteMode qm) {
  try {
    std::string str;
    DequotingStatus status = dequoteImpl<std::istream &>(str, in, StreamWhiteSpaceDelim(in),  qm);
    if (status != DequotingStatus::OK) {
      in.setstate(std::ios_base::failbit);
      throw DequotingError(status, nullptr, nullptr);
    } else if (in.eof() && !in.bad())
      in.clear();
    return str;
  } catch (...) {
    in.setstate(std::ios_base::badbit);
    throw;
  }
}

} } // namespace CoSupport::String