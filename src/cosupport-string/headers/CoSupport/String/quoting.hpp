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

namespace CoSupport { namespace String {

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
 * \return On success, true is returned otherwise false.
 */
bool dequote(std::string &str, const char *&in, const char *end, QuoteMode qm = QuoteMode::AUTO) throw();

/**
 *  Dequote input between in and end given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[in] in specifies the input to be dequoted.
 * \param[in] end specifies the end of the input string.
 * \return On success, true is returned otherwise false.
 */
bool dequote(std::string &str, const char *in, const char *end, QuoteMode qm = QuoteMode::AUTO) throw();

/**
 *  Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[inout] in specifies the null terminate c string to be dequoted.
 * \return On success, true is returned otherwise false.
 */
bool dequote(std::string &str, const char *&in, QuoteMode qm = QuoteMode::AUTO) throw();

/**
 *  Dequote the null terminate c string given in the specified quote mode.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[in] in specifies the null terminate c string to be dequoted.
 * \return On success, true is returned otherwise false.
 */
bool dequote(std::string &str, const char *in, QuoteMode qm = QuoteMode::AUTO) throw();

/**
 *  Dequote a quoted string in the specified quote mode from the input stream in.
 *
 * \param[out] str specifies the output string that will hold the dequoted string.
 * \param[in] in specifies the input stream.
 * \return On success, true is returned otherwise false.
 *
 * In case of error, the std::ios::failbit will be set for the input stream.
 */
bool dequote(std::string &str, std::istream &in, QuoteMode qm = QuoteMode::AUTO) throw();

} } // namespace CoSupport::String

#endif // _INCLUDED_COSUPPORT_STRING_QUOTING_HPP
