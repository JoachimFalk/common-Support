// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/String/UniquePool.hpp>
#include <CoSupport/String/Concat.hpp>

#include <cassert>

namespace CoSupport { namespace String {

std::string UniquePool::operator ()(const std::string &str) {
  Pool::iterator iter = pool.find(str);
  
  if (iter != pool.end()) {
    std::string str_index;
    size_t          index = iter->second;
    
    do {
      ++index;
      assert(index != 0 || !"WTF? Out of indices in UniqePool!");
      str_index = Concat(str)("_")(index);
    } while (pool.find(str_index) != pool.end());
    pool.insert(std::make_pair(str_index, 0));
    iter->second = index;
    return str_index;
  } else {
    pool.insert(std::make_pair(str, 0));
    return str;
  }
}
  
} } // namespace CoSupport::String
