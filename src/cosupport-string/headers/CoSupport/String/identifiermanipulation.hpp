// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Tobias Schwarzer <tobias.schwarzer@fau.de>
 *   2014 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STRING_IDENTIFIERMANIPULATION_HPP
#define _INCLUDED_COSUPPORT_STRING_IDENTIFIERMANIPULATION_HPP

#include <iostream>
#include <string>

#include "export_config.h"

namespace CoSupport { namespace String {

/// Searches the string for the last occurancre of the string specified
/// by the \p delimiter and returns the substring starting after the last
/// occurrence of specified string.
/// \param[in] str       input string
/// \param[in] delimiter delimiter string
/// \return substring starting after the last occurrence of specified character
COSUPPORT_STRING_API
std::string hierarchyBasename (const std::string &str, std::string delimiter);

/// Searches the string for the last occurancre of the string specified
/// by the \p delimiter and returns the substring starting from the first
/// charater of \p str up to and including the string given in \p deltimeter.
/// \param[in] str       input string
/// \param[in] delimiter delimiter string
/// \return substring from the first character up to and including the last occurance of the delimiter string
COSUPPORT_STRING_API
std::string hierarchyDirname(const std::string &str, std::string delimiter);

} } // namespace CoSupport::String

#endif // _INCLUDED_COSUPPORT_STRING_IDENTIFIERMANIPULATION_HPP
