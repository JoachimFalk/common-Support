// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2012 brentrop <brentrop.tobias@gmail.com>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/String/UniqueCIdentifierPool.hpp>

namespace CoSupport { namespace String {

std::string UniqueCIdentifierPool::makeCIdentifier(const std::string &str) {
  std::string result = str;
  if (!result.empty()) {
    if (!((result[0] >= 'a' && result[0] <= 'z') || 
	  (result[0] >= 'A' && result[0] <= 'Z'))) {
      result[0] = '_';
    }
    for (size_t index = 1; index < result.length(); ++index) {
      if (!((result[index] >= 'a' && result[index] <= 'z') || 
	    (result[index] >= 'A' && result[index] <= 'Z') ||
	    (result[index] >= '0' && result[index] <= '9'))) {
	result[index] = '_';
      }
    }
  } else {
    result = "_";
  }
  return result;
}

} } // namespace CoSupport::String
