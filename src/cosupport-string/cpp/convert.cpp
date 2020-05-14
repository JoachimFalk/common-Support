// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/String/convert.hpp>

#include <cstdlib>
#include <limits>

namespace CoSupport { namespace String {

template <>
int8_t strAs<int8_t>(std::string const &str) {
  char *endptr = nullptr;

  long value = strtol(str.c_str(), &endptr, 0);
  if (str.empty() || !endptr || endptr != str.c_str() + str.size())
    throw InvalidConversionTo<int8_t>();
  if (value < std::numeric_limits<int8_t>::min())
    throw InvalidConversionTo<int8_t>();
  if (value > std::numeric_limits<int8_t>::max())
    throw InvalidConversionTo<int8_t>();
  return value;
}

template <>
uint8_t strAs<uint8_t>(std::string const &str) {
  char *endptr = nullptr;

  unsigned long value = strtoul(str.c_str(), &endptr, 0);
  if (str.empty() || !endptr || endptr != str.c_str() + str.size())
    throw InvalidConversionTo<uint8_t>();
  if (value > std::numeric_limits<uint8_t>::max())
    throw InvalidConversionTo<uint8_t>();
  return value;
}

} } // namespace CoSupport::String
