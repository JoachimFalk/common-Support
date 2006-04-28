
/*******************************************************************************
 *                        Copyright 2004
 *                Lehrstuhl fuer Hardware-Software-Codesign
 *                  Universitaet Erlangen-Nuernberg
 *                    All rights reserved
 *
 * Title: InfiniBand HCA
 * Comment:
 * -----------------------------------------------------------------------------
 * par_manager.cpp
 * -----------------------------------------------------------------------------
 * Modifications History:
 * -----------------------------------------------------------------------------
 * Notes:
 * -----------------------------------------------------------------------------
 * $log$
 ******************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <par_manager.hpp>

std::auto_ptr<par_manager> par_manager::inst(new par_manager());

const par_manager& par_manager::instance()
{ return *inst; }

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
