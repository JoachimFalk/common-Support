/*
 * Copyright (c) 2004-2006 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cosupport/par_manager.hpp>

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
