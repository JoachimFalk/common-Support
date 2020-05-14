// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <CoSupport/SystemC/par_manager.hpp>

namespace CoSupport { namespace SystemC {

const par_manager& par_manager::instance()
{ 
  static par_manager instance;
  return instance;
}

par_manager::par_manager()
{
  const char* file = getenv("PARCONFIGURATION");
  
  if(!file)
    std::cout << "par_manager> Warning: no config file!" << std::endl;
  else {
    std::ifstream fin(file);

    if(!fin)
      std::cout << "par_manager> Warning: could not open file!" << std::endl;
    else {
      while(!fin.eof()) {
	std::string name;
	fin >> name;
	
	if(name == "")
	  continue;

	int count;
	fin >> count;
	
	if(count < 1) {
	  std::cout << "par_manager> Warning: invalid count for " << name << std::endl;
	  continue;
	}

	if(config.find(name) == config.end()) {
	  std::cout << "par_manager> " << name << ": " << count << std::endl;
	  config.insert(std::make_pair(name, count));
	} else {
	  std::cout << "par_manager> Warning: " << name << " already defined!" << std::endl;
	  continue;
	}
      }
    }
  }
}

int par_manager::count(const std::string& name) const
{
  std::map<std::string, int>::const_iterator i = config.find(name);
  if(i == config.end())
    return 1;
  else
    return i->second;
}

} } // namespace CoSupport::SystemC
