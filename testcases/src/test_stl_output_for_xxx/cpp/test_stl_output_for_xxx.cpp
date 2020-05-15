// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/Streams/stl_output_for_list.hpp>
#include <CoSupport/Streams/stl_output_for_vector.hpp>
#include <CoSupport/Streams/stl_output_for_set.hpp>
#include <CoSupport/Streams/stl_output_for_map.hpp>

#include <iostream>
#include <string>

#include <list>
#include <vector>
#include <set>
#include <map>

#include <utility>

int main(int argc, char *argv[]) {
  std::list<int>              list;
  std::vector<int>            vector;
  std::set<int>               set;
  std::map<int, std::string>  map;

  list.push_back(1); list.push_back(2); std::cout << list << std::endl;
  vector.push_back(1); vector.push_back(2); std::cout << vector << std::endl;
  set.insert(1); set.insert(2); std::cout << set << std::endl;
  map.insert(std::make_pair(1, "A")); map.insert(std::make_pair(2, "B")); std::cout << map << std::endl;
  return 0;
}
