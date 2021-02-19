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
  OK            = 0,
  GENERIC_ERROR = 1,
  MISSING_OPENING_SINGLE_QUOTE = 2,
  MISSING_CLOSING_SINGLE_QUOTE = 3,
  MISSING_OPENING_DOUBLE_QUOTE = 4,
  MISSING_CLOSING_DOUBLE_QUOTE = 5,
  HEX_ESCAPE_WITHOUT_HEX_DIGIT = 6,
  OCT_ESCAPE_EXCEEDS_CHAR_RANGE = 7,
  MISSING_ESCAPE_CHAR = 8,
  ILLEGAL_ESCAPE_CHAR = 9
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

/**
 *  Dequote input between in and end given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[inout] in specifies the input to be dequoted.
 * \param[in] end specifies the end of the input string.
 * \param[in] qm specifies how the input is quoted.
 * \return On success, returns \code DequotingStatus::OK otherwise an error code.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(std::string &str, const char *&in, const char *end, QuoteMode qm = QuoteMode::AUTO) throw();

/**
 *  Dequote input between in and end given in the specified quote mode.
 *
 * \param[inout] in specifies the input to be dequoted.
 * \param[in] end specifies the end of the input string.
 * \param[in] qm specifies how the input is quoted.
 * \return On success, returns the dequoted string; otherwise, throws a \code DequotingError.
 */
COSUPPORT_STRING_API
std::string dequote(const char *&in, const char *end, QuoteMode qm = QuoteMode::AUTO);

/**
 *  Dequote input between in and end given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[in] qm specifies how the input is quoted.
 * \param[in] in specifies the input to be dequoted.
 * \param[in] end specifies the end of the input string.
 * \return On success, returns \code DequotingStatus::OK otherwise an error code.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(std::string &str, QuoteMode qm, const char *in, const char *end) throw();

/**
 *  Dequote input between in and end given in the specified quote mode.
 *
 * \param[in] qm specifies how the input is quoted.
 * \param[in] in specifies the input to be dequoted.
 * \param[in] end specifies the end of the input string.
 * \return On success, returns the dequoted string; otherwise, throws a \code DequotingError.
 */
COSUPPORT_STRING_API
std::string dequote(QuoteMode qm, const char *in, const char *end);

/**
 *  Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[inout] in specifies the null terminate c string to be dequoted.
 * \param[in] qm specifies how the input is quoted.
 * \return On success, returns \code DequotingError::OK otherwise an error code.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(std::string &str, const char *&in, QuoteMode qm = QuoteMode::AUTO) throw();

/**
 *  Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[inout] in specifies the null terminate c string to be dequoted.
 * \param[in] qm specifies how the input is quoted.
 * \return On success, returns the dequoted string; otherwise, throws a \code DequotingError.
 */
COSUPPORT_STRING_API
std::string dequote(const char *&in, QuoteMode qm = QuoteMode::AUTO);

/**
 *  Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[in] qm specifies how the input is quoted.
 * \param[in] in specifies the null terminate c string to be dequoted.
 * \return On success, returns \code DequotingStatus::OK otherwise an error code.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(std::string &str, QuoteMode qm, const char *in) throw();

/**
 *  Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[in] qm specifies how the input is quoted.
 * \param[in] in specifies the null terminate c string to be dequoted.
 * \return On success, returns the dequoted string; otherwise, throws a \code DequotingError.
 */
COSUPPORT_STRING_API
std::string dequote(QuoteMode qm, const char *in);

/**
 *  Dequote a quoted string in the specified quote mode from the input stream in.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[in] in specifies the input stream.
 * \param[in] qm specifies how the input is quoted.
 * \return On success, returns \code DequotingStatus::OK otherwise an error code.
 *
 * In case of error, the std::ios::failbit will be set for the input stream.
 */
COSUPPORT_STRING_API
DequotingStatus dequote(std::string &str, std::istream &in, QuoteMode qm = QuoteMode::AUTO) throw();

/**
 *  Dequote a quoted string in the specified quote mode from the input stream in.
 *
 * \param[in] in specifies the input stream.
 * \param[in] qm specifies how the input is quoted.
 * \return On success, returns the dequoted string; otherwise, throws a \code DequotingError.
 *
 * In case of error, the std::ios::failbit will also be set for the input stream before throwing a \code DequotingError.
 */
COSUPPORT_STRING_API
std::string dequote(std::istream &in, QuoteMode qm = QuoteMode::AUTO);

} } // namespace CoSupport::String

#endif // _INCLUDED_COSUPPORT_STRING_QUOTING_HPP
