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

#ifndef _INCLUDED_COSUPPORT_STRING_QUOTING_HPP
#define _INCLUDED_COSUPPORT_STRING_QUOTING_HPP

#include <CoSupport/commondefs.h>

#include "export_config.h"

#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>

namespace CoSupport { namespace String {

enum class DequotingStatus {
  /// A valid dequoting has been performed.
  OK            = 0,
  /// An unspecified error has been encountered during dequoting.
  GENERIC_ERROR = 1,
  /// The opening single quote "'" was missing, e.g., abcd'.
  MISSING_OPENING_SINGLE_QUOTE = 2,
  /// The closing single quote "'" was missing, e.g., 'abcd.
  MISSING_CLOSING_SINGLE_QUOTE = 3,
  /// The opening double quote '"' was missing, e.g., abcd".
  MISSING_OPENING_DOUBLE_QUOTE = 4,
  /// The closing double quote "'" was missing, e.g., "abcd.
  MISSING_CLOSING_DOUBLE_QUOTE = 5,
  /// Trailing garbage after closing single or double quote.
  TRAILING_GARBAGE = 10,
  /// No hex digit after \x escape sequence, e.g., "ab\xk".
  HEX_ESCAPE_WITHOUT_HEX_DIGIT = 6,
  /// Octal escape sequence encodes a character outside char range, e.g., "\400".
  OCT_ESCAPE_EXCEEDS_CHAR_RANGE = 7,
  /// After an escaped character, the input ends unexpectedly, e.g., "abc\<- input ends here!
  MISSING_ESCAPE_CHAR = 8,
  /// Illegal escape sequence, e.g., "\q". The meaning of \q is not defined.
  ILLEGAL_ESCAPE_CHAR = 9,
  /// Missing variable name after a '$', e.g., "$\n", or missing
  /// '}' for a ${VAR} substitution, e.g., "${VAR".
  MISSING_VARIABLE_NAME = 11,
  /// Illegal variable name, i.e., only 0-9, a-z, A-Z, and _ are allowed!
  ILLEGAL_VARIABLE_NAME = 12,
  /// No delimiters allowed for given quote mode
  NO_DELIMITERS_ALLOWED_FOR_QM = 13,
};

COSUPPORT_STRING_API
std::ostream &operator <<(std::ostream &, DequotingStatus status);

class COSUPPORT_STRING_API
DequotingError
  : public std::runtime_error
{
public:
  explicit
  DequotingError(DequotingStatus error, const char *from, const char *to);

  DequotingStatus getError() const
    { return error; }
protected:
  DequotingStatus error;
};

enum class QuoteMode {
  SINGLE_NO_QUOTES   = 0,
  SINGLE_WITH_QUOTES = 1,
  DOUBLE_NO_QUOTES   = 2,
  DOUBLE_WITH_QUOTES = 3,
  AUTO               = 4,
};

class Environment;

class COSUPPORT_STRING_API
Delimiters {
public:
  Delimiters(char const *str = nullptr)
    : given(str != nullptr), delims(str ? str : "") {}
  Delimiters(std::string const &str)
    : given(true), delims(str) {}
protected:
  bool        given;
  std::string delims;
};

/**
 * Dequote input between in and end given in the specified quote mode.
 *
 * \param[out]   str specifies the output string that will hold the dequoted
 *               string.
 * \param[inout] in specifies the input to be dequoted.
 * \param[in]    end specifies the end of the input string.
 * \param[in]    qm specifies how the input is quoted.
 * \param[in]    delim specifies a set of characters acting as delimiters
 *               denoting the end of the quoted string in QuoteMode::AUTO.
 * \return On success, returns DequotingStatus::OK otherwise an error code.
 *
 * While dequoting, in will be advanced. On a successful dequoting, one of the
 * following conditions holds:
 * (a) in is end,
 * (b) *in is a delimiter char in case qm is QuoteMode::AUTO,
 * (c) in[-1] is '"' in case qm is QuoteMode::DOUBLE_WITH_QUOTES
 * (d) in[-1] is '\'' in case qm is QuoteMode::SINGLE_WITH_QUOTES.
 * When no delimiter is specified, i.e., delim == nullptr, than any whitespace
 * character will act as a delimiter. Note that this is locale specific.
 * Moreover, delimiters are only considered for QuoteMode::AUTO. Specifying
 * delimiters for other quote modes will result in a
 * NO_DELIMITERS_ALLOWED_FOR_QM DequotingError error.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(
    std::string &str
  , const char *&in, const char *end
  , QuoteMode qm = QuoteMode::AUTO
  , Delimiters const &delims = Delimiters()
  , Environment const *env = nullptr) throw();

/**
 * Dequote input between in and end given in the specified quote mode.
 *
 * \param[inout] in specifies the input to be dequoted.
 * \param[in]    end specifies the end of the input string.
 * \param[in]    qm specifies how the input is quoted.
 * \param[in]    delim specifies a set of characters acting as delimiters
 *               denoting the end of the quoted string in QuoteMode::AUTO.
 * \return       On success, returns the dequoted string; otherwise, throws a
 *               DequotingError.
 *
 * While dequoting, in will be advanced. On a successful dequoting, one of the
 * following conditions holds:
 * (a) in is end,
 * (b) *in is a delimiter char in case qm is QuoteMode::AUTO,
 * (c) in[-1] is '"' in case qm is QuoteMode::DOUBLE_WITH_QUOTES
 * (d) in[-1] is '\'' in case qm is QuoteMode::SINGLE_WITH_QUOTES.
 * When no delimiter is specified, i.e., delim == nullptr, than any whitespace
 * character will act as a delimiter. Note that this is locale specific.
 * Moreover, delimiters are only considered for QuoteMode::AUTO. Specifying
 * delimiters for other quote modes will result in a
 * NO_DELIMITERS_ALLOWED_FOR_QM DequotingError exception.
 */
COSUPPORT_STRING_API
std::string dequote(
    const char *&in, const char *end
  , QuoteMode qm = QuoteMode::AUTO
  , Delimiters const &delims = Delimiters()
  , Environment const *env = nullptr);

/**
 * Dequote input between in and end given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted
 *             string.
 * \param[in]  qm specifies how the input is quoted.
 * \param[in]  in specifies the input to be dequoted.
 * \param[in]  end specifies the end of the input string.
 * \return     On success, returns DequotingStatus::OK otherwise an error code.
 *
 * This function will try to dequote the whole string between in and end. In
 * case this fails due to trailing garbage, e.g., after the closing single or
 * double quotes, a DequotingStatus::TRAILING_GARBAGE error will be returned.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(
    std::string &str
  , QuoteMode qm
  , const char *in, const char *end
  , Environment const *env = nullptr) throw();

/**
 * Dequote input between in and end given in the specified quote mode.
 *
 * \param[in] qm specifies how the input is quoted.
 * \param[in] in specifies the input to be dequoted.
 * \param[in] end specifies the end of the input string.
 * \return    On success, returns the dequoted string; otherwise, throws a
 *            DequotingError.
 *
 * This function will try to dequote the whole string between in and end. In
 * case this fails due to trailing garbage, e.g., after the closing single or
 * double quotes, a TRAILING_GARBAGE DequotingError will be thrown.
 */
COSUPPORT_STRING_API
std::string dequote(
    QuoteMode qm
  , const char *in, const char *end
  , Environment const *env = nullptr);

/**
 * Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[out]   str specifies the output string that will hold the dequoted
 *               string.
 * \param[inout] in specifies the null terminate c string to be dequoted.
 * \param[in]    qm specifies how the input is quoted.
 * \param[in]    delim specifies a set of characters acting as delimiters
 *               denoting the end of the quoted string in QuoteMode::AUTO.
 * \return       On success, returns DequotingStatus::OK otherwise an error
 *               code.
 *
 * While dequoting, in will be advanced. On a successful dequoting, one of the
 * following conditions holds:
 * (a) *in is '\0',
 * (b) *in is a delimiter char in case qm is QuoteMode::AUTO,
 * (c) in[-1] is '"' in case qm is QuoteMode::DOUBLE_WITH_QUOTES
 * (d) in[-1] is '\'' in case qm is QuoteMode::SINGLE_WITH_QUOTES.
 * When no delimiter is specified, i.e., delim == nullptr, than any whitespace
 * character will act as a delimiter. Note that this is locale specific.
 * Moreover, delimiters are only considered for QuoteMode::AUTO. Specifying
 * delimiters for other quote modes will result in a
 * NO_DELIMITERS_ALLOWED_FOR_QM error.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(
    std::string &str
  , const char *&in
  , QuoteMode qm = QuoteMode::AUTO
  , Delimiters const &delims = Delimiters()
  , Environment const *env = nullptr) throw();

/**
 * Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[inout] in specifies the null terminate c string to be dequoted.
 * \param[in]    qm specifies how the input is quoted.
 * \param[in]    delim specifies a set of characters acting as delimiters
 *               denoting the end of the quoted string in QuoteMode::AUTO.
 * \return       On success, returns the dequoted string; otherwise, throws a
 *               DequotingError.
 *
 * While dequoting, in will be advanced. On a successful dequoting, one of the
 * following conditions holds:
 * (a) *in is '\0',
 * (b) *in is a delimiter char in case qm is QuoteMode::AUTO,
 * (c) in[-1] is '"' in case qm is QuoteMode::DOUBLE_WITH_QUOTES
 * (d) in[-1] is '\'' in case qm is QuoteMode::SINGLE_WITH_QUOTES.
 * When no delimiter is specified, i.e., delim == nullptr, than any whitespace
 * character will act as a delimiter. Note that this is locale specific.
 * Moreover, delimiters are only considered for QuoteMode::AUTO. Specifying
 * delimiters for other quote modes will result in a
 * NO_DELIMITERS_ALLOWED_FOR_QM DequotingError exception.
 */
COSUPPORT_STRING_API
std::string dequote(
    const char *&in
  , QuoteMode qm = QuoteMode::AUTO
  , Delimiters const &delims = Delimiters()
  , Environment const *env = nullptr);

/**
 * Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted
 *             string.
 * \param[in]  qm specifies how the input is quoted.
 * \param[in]  in specifies the null terminate c string to be dequoted.
 * \return     On success, returns DequotingStatus::OK otherwise an error code.
 *
 * This function will try to dequote the whole in string. In case this fails
 * due to trailing garbage, e.g., after the closing single or double quotes, a
 * TRAILING_GARBAGE error will be returned.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(
    std::string &str
  , QuoteMode qm
  , const char *in
  , Environment const *env = nullptr) throw();

/**
 * Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[in] qm specifies how the input is quoted.
 * \param[in] in specifies the null terminate c string to be dequoted.
 * \return    On success, returns the dequoted string; otherwise, throws a
 *            DequotingError.
 *
 * This function will try to dequote the whole in string. In case this fails
 * due to trailing garbage, e.g., after the closing single or double quotes, a
 * TRAILING_GARBAGE DequotingError exception will be thrown.
 */
COSUPPORT_STRING_API
std::string dequote(
    QuoteMode qm
  , const char *in
  , Environment const *env = nullptr);

/**
 * Dequote a quoted string in the specified quote mode from the input stream in.
 *
 * \param[out]   str specifies the output string that will hold the dequoted
 *               string.
 * \param[inout] in specifies the input stream.
 * \param[in]    qm specifies how the input is quoted.
 * \param[in]    delim specifies a set of characters acting as delimiters
 *               denoting the end of the quoted string in QuoteMode::AUTO.
 * \return       On success, returns DequotingStatus::OK otherwise an error
 *               code.
 *
 * In case of error, the std::ios::failbit will be set for the input stream.
 * In case of a successful dequoting, in.good() will return true and one of the
 * following conditions holds:
 * (a) the next char read from in will result in EOF,
 * (b) the next char read from in is a delimiter char in case qm is QuoteMode::AUTO,
 * (c) the previous char read from in was '"' in case qm is QuoteMode::DOUBLE_WITH_QUOTES
 * (d) the previous char read from in was '\'' in case qm is QuoteMode::SINGLE_WITH_QUOTES.
 * When no delimiter is specified, i.e., delim == nullptr, than any whitespace
 * character will act as a delimiter. Note that this is locale specific.
 * Moreover, delimiters are only considered for QuoteMode::AUTO. Specifying
 * delimiters for other quote modes will result in a
 * NO_DELIMITERS_ALLOWED_FOR_QM error.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(
    std::string &str
  , std::istream &in
  , QuoteMode qm = QuoteMode::AUTO
  , Delimiters const &delims = Delimiters()
  , Environment const *env = nullptr) throw();

/**
 * Dequote a quoted string in the specified quote mode from the input stream in.
 *
 * \param[inout] in specifies the input stream.
 * \param[in]    qm specifies how the input is quoted.
 * \param[in]    delim specifies a set of characters acting as delimiters
 *               denoting the end of the quoted string in QuoteMode::AUTO.
 * \return       On success, returns the dequoted string; otherwise, throws a
 *               DequotingError.
 *
 * In case of error, the std::ios::failbit will also be set for the input
 * stream before throwing a DequotingError. In case of a successful dequoting,
 * in.good() will return true and one of the following conditions holds:
 * (a) the next char read from in will result in EOF,
 * (b) the next char read from in is a delimiter char in case qm is QuoteMode::AUTO,
 * (c) the previous char read from in was '"' in case qm is QuoteMode::DOUBLE_WITH_QUOTES
 * (d) the previous char read from in was '\'' in case qm is QuoteMode::SINGLE_WITH_QUOTES.
 * When no delimiter is specified, i.e., delim == nullptr, than any whitespace
 * character will act as a delimiter. Note that this is locale specific.
 * Moreover, delimiters are only considered for QuoteMode::AUTO. Specifying
 * delimiters for other quote modes will result in a
 * NO_DELIMITERS_ALLOWED_FOR_QM DequotingError exception.
 */
COSUPPORT_STRING_API
std::string dequote(
    std::istream &in
  , QuoteMode qm = QuoteMode::AUTO
  , Delimiters const &delims = Delimiters()
  , Environment const *env = nullptr);

} } // namespace CoSupport::String

#endif // _INCLUDED_COSUPPORT_STRING_QUOTING_HPP
