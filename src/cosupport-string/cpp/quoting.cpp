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
#include <CoSupport/String/Environment.hpp>

//#include <cstdlib>
#include <cstring>
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

  class NoDelim {
  public:
    bool operator()(int ch) const
      { return false; }
  };

  class WhiteSpaceDelim {
  public:
    WhiteSpaceDelim(const char *delim)
      : delim(delim) {}

    bool operator()(int ch) const {
      return delim != nullptr
          ? strchr(delim, ch) != nullptr
          : isspace(ch);
    }
  private:
    const char *delim;
  };

  class StreamWhiteSpaceDelim {
  public:
    StreamWhiteSpaceDelim(std::istream const &in, const char *delim)
      : facet(std::use_facet<std::ctype<char> >(in.getloc()))
      , delim(delim) {}

    bool operator()(int ch) const {
      return delim != nullptr
          ? strchr(delim, ch) != nullptr
          : facet.is(std::ctype<char>::space, static_cast<char>(ch));
    }
  private:
    std::ctype<char> const &facet;
    const char             *delim;
  };

#define VALID_VAR_CHARS(ch) \
  ( ch == '_' || \
    (ch >= '0' && ch <= '9') || \
    (ch >= 'A' && ch <= 'Z') || \
    (ch >= 'a' && ch <= 'z'))

  template <typename INPUT, typename DELIM>
  DequotingStatus dequoteVarImpl(std::string &dst, INPUT &in, DELIM delim, Environment const &env) {
    int ch = in.get();
    if (ch == EOF || delim(ch) ||
        (ch != '{' && !VALID_VAR_CHARS(ch))) {
      if (ch != EOF)
        in.unget();
      return DequotingStatus::MISSING_VARIABLE_NAME;
    }
    std::string var;
    if (ch == EOF || delim(ch)) {
      if (ch != EOF)
        in.unget();
    } else if (ch == '{') {
      ch = in.get();
      while (ch != EOF && !delim(ch) && ch != '}') {
        var.append(1, ch);
        ch = in.get();
      }
      if (ch == EOF || delim(ch)) {
        if (ch != EOF)
          in.unget();
        var.clear();
      }
    } else if (ch >= '0' && ch <= '9') {
      var.append(1, ch);
    } else if (VALID_VAR_CHARS(ch)) {
      do {
        var.append(1, ch);
        ch = in.get();
      } while (ch != EOF && !delim(ch) && VALID_VAR_CHARS(ch));
      if (ch != EOF)
        in.unget();
    } else {
      if (ch != EOF)
        in.unget();
    }
    if (var.empty())
      return DequotingStatus::MISSING_VARIABLE_NAME;
    Environment::const_iterator iter = env.find(var);
    if (iter != env.end())
      dst.append(iter->second);
    return DequotingStatus::OK;
  }

  template <typename INPUT, typename DELIM>
  DequotingStatus dequoteImpl(std::string &dst, INPUT &in, QuoteMode qm, DELIM delim, Environment const *env) {
    int ch = in.get();

    bool variableSubsitution = env != nullptr;

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
              case '$':
                if (variableSubsitution) {
                  DequotingStatus status = qmActive == QuoteMode::DOUBLE_NO_QUOTES
                      ? dequoteVarImpl(dst, in, NoDelim(), *env)
                      : dequoteVarImpl(dst, in, WhiteSpaceDelim("\""), *env);
                  if (status != DequotingStatus::OK)
                    return status;
                } else
                  dst.append(1, ch);
                break;
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
            if (ch == '$' && variableSubsitution) {
              DequotingStatus status = qmActive == QuoteMode::SINGLE_NO_QUOTES
                  ? dequoteVarImpl(dst, in, NoDelim(), *env)
                  : dequoteVarImpl(dst, in, WhiteSpaceDelim("'"), *env);
              if (status != DequotingStatus::OK)
                return status;
            } else
              dst.append(1, ch);
            ch = in.get();
          }
          if (qm != QuoteMode::AUTO || ch == EOF)
            goto EndLoop;
          qmActive = QuoteMode::AUTO;
          ch       = in.get();
          break;
        case QuoteMode::AUTO:
          if (delim(ch)) {
            in.unget();
            goto EndLoop;
          }
          switch (ch) {
            case '\\':
              ch = in.get();
              if (ch == EOF)
                return DequotingStatus::MISSING_ESCAPE_CHAR;
              else if (ch != '\n')
                dst.append(1, ch);
              break;
            case '"':
              variableSubsitution = env != nullptr;
              qmActive = QuoteMode::DOUBLE_WITH_QUOTES;
              break;
            case '\'':
              variableSubsitution = false;
              qmActive = QuoteMode::SINGLE_WITH_QUOTES;
              break;
            case '$':
              if (env != nullptr) {
                DequotingStatus status = dequoteVarImpl(dst, in, delim, *env);
                if (status != DequotingStatus::OK)
                  return status;
              } else
                dst.append(1, ch);
              break;
            default:
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
      case DequotingStatus::TRAILING_GARBAGE:
        msg = "Trailing garbage after closing single or double quote";
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
      case DequotingStatus::MISSING_VARIABLE_NAME:
        msg = "Missing variable name";
        break;
      case DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM:
        msg = "Specified quote mode does not allow for delimiters";
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
    case DequotingStatus::TRAILING_GARBAGE:
      return out << "DequotingStatus::TRAILING_GARBAGE";
    case DequotingStatus::HEX_ESCAPE_WITHOUT_HEX_DIGIT:
      return out << "DequotingStatus::HEX_ESCAPE_WITHOUT_HEX_DIGIT";
    case DequotingStatus::OCT_ESCAPE_EXCEEDS_CHAR_RANGE:
      return out << "DequotingStatus::OCT_ESCAPE_EXCEEDS_CHAR_RANGE";
    case DequotingStatus::MISSING_ESCAPE_CHAR:
      return out << "DequotingStatus::MISSING_ESCAPE_CHAR";
    case DequotingStatus::ILLEGAL_ESCAPE_CHAR:
      return out << "DequotingStatus::ILLEGAL_ESCAPE_CHAR";
    case DequotingStatus::MISSING_VARIABLE_NAME:
      return out << "DequotingStatus::MISSING_VARIABLE_NAME";
    case DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM:
      return out << "DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM";
    default:
      assert(!"Unknown DequotingStatus!");
      return out;
  }
}

DequotingError::DequotingError(DequotingStatus error, const char *from, const char *to)
  : std::runtime_error(composeErrorMessage(error, from, to)), error(error) {}

DequotingStatus dequote(
    std::string &str
  , const char *&in, const char *end
  , QuoteMode qm
  , const char *delim
  , Environment const *env) throw()
{
  if (qm != QuoteMode::AUTO && delim)
    return DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM;
  CharRange inStream(in, end);
  DequotingStatus status = dequoteImpl(str, inStream, qm, WhiteSpaceDelim(delim), env);
  in = inStream.in;
  return status;
}

std::string dequote(
    const char *&in, const char *end
  , QuoteMode qm
  , const char *delim
  , Environment const *env)
{
  if (qm != QuoteMode::AUTO && delim)
    throw DequotingError(DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM, nullptr, nullptr);
  std::string str;
  CharRange inStream(in, end);
  DequotingStatus status = dequoteImpl(str, inStream, qm, WhiteSpaceDelim(delim), env);
  const char *start = in;
  in = inStream.in;
  if (status != DequotingStatus::OK)
    throw DequotingError(status, start, inStream.in);
  return str;
}

DequotingStatus dequote(
    std::string &str
  , QuoteMode qm
  , const char *in, const char *end
  , Environment const *env) throw()
{
  CharRange inStream(in, end);
  DequotingStatus status = dequoteImpl(str, inStream, qm, NoDelim(), env);
  if (status != DequotingStatus::OK)
    return status;
  if (inStream.in != end)
    return DequotingStatus::TRAILING_GARBAGE;
  return DequotingStatus::OK;
}

std::string dequote(
    QuoteMode qm
  , const char *in, const char *end
  , Environment const *env)
{
  std::string str;
  CharRange inStream(in, end);
  DequotingStatus status = dequoteImpl(str, inStream, qm, NoDelim(), env);
  if (status != DequotingStatus::OK)
    throw DequotingError(status, in, inStream.in);
  if (inStream.in != end)
    throw DequotingError(DequotingStatus::TRAILING_GARBAGE, in, inStream.in);
  return str;
}

DequotingStatus dequote(
    std::string &str
  , const char *&in
  , QuoteMode qm
  , const char *delim
  , Environment const *env) throw()
{
  if (qm != QuoteMode::AUTO && delim)
    return DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM;
  CString inStream(in);
  DequotingStatus status = dequoteImpl(str, inStream, qm, WhiteSpaceDelim(delim), env);
  in = inStream.in;
  return status;
}

std::string  dequote(
    const char *&in
  , QuoteMode qm
  , const char *delim
  , Environment const *env)
{
  if (qm != QuoteMode::AUTO && delim)
    throw DequotingError(DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM, nullptr, nullptr);
  std::string str;
  CString inStream(in);
  DequotingStatus status = dequoteImpl(str, inStream, qm, WhiteSpaceDelim(delim), env);
  const char *start = in;
  in = inStream.in;
  if (status != DequotingStatus::OK)
    throw DequotingError(status, start, inStream.in);
  return str;
}

DequotingStatus dequote(
    std::string &str
  , QuoteMode qm
  , const char *in
  , Environment const *env) throw()
{
  CString inStream(in);
  DequotingStatus status = dequoteImpl(str, inStream, qm, NoDelim(), env);
  if (status != DequotingStatus::OK)
    return status;
  if (*inStream.in != '\0')
    return DequotingStatus::TRAILING_GARBAGE;
  return DequotingStatus::OK;
}

std::string dequote(
    QuoteMode qm
  , const char *in
  , Environment const *env)
{
  std::string str;
  CString inStream(in);
  DequotingStatus status = dequoteImpl(str, inStream, qm, NoDelim(), env);
  if (status != DequotingStatus::OK)
    throw DequotingError(status, in, inStream.in);
  if (*inStream.in != '\0')
    throw DequotingError(DequotingStatus::TRAILING_GARBAGE, in, inStream.in);
  return str;
}

DequotingStatus dequote(
    std::string &str
  , std::istream &in
  , QuoteMode qm
  , const char *delim
  , Environment const *env) throw()
{
  DequotingStatus status;
  if (qm != QuoteMode::AUTO && delim)
    status = DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM;
  else
    try {
      status = dequoteImpl(str, in, qm, StreamWhiteSpaceDelim(in, delim), env);
      if (status == DequotingStatus::OK) {
        if (in.eof() && !in.bad())
            in.clear();
        return status;
      }
    } catch (...) {
      in.setstate(std::ios_base::badbit);
      return DequotingStatus::GENERIC_ERROR;
    }
  in.setstate(std::ios_base::failbit);
  return status;
}

std::string dequote(
    std::istream &in
  , QuoteMode qm
  , const char *delim
  , Environment const *env)
{
  DequotingStatus status;
  if (qm != QuoteMode::AUTO && delim)
    status = DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM;
  else
    try {
      std::string str;
      status = dequoteImpl(str, in, qm, StreamWhiteSpaceDelim(in, delim), env);
      if (status == DequotingStatus::OK) {
        if (in.eof() && !in.bad())
          in.clear();
        return str;
      }
    } catch (...) {
      in.setstate(std::ios_base::badbit);
      throw;
    }
  in.setstate(std::ios_base::failbit);
  throw DequotingError(status, nullptr, nullptr);
}

} } // namespace CoSupport::String
