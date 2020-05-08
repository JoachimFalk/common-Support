// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2020 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_STRING_CONVERT_HPP
#define _INCLUDED_COSUPPORT_STRING_CONVERT_HPP

#include <cctype>
#include <sstream>
#include <string>
#include <exception>
#include <typeinfo>

#include "TypeName.hpp"

#include "export_config.h"

namespace CoSupport { namespace String {

struct COSUPPORT_STRING_API
InvalidConversion: public std::exception {
};

template <typename T>
struct InvalidConversionTo: public InvalidConversion {
  const char *what() const throw() {
    std::stringstream str;
    str << "CoSupport::String::InvalidConversionTo<" << typeid(T).name() << ">";
    return str.str().c_str();
  }
};

template <typename T>
T strAs(const std::string &str) {
  T                   retval;
  std::istringstream  in(str);
  
  in >> retval;
  
  if (!in.fail()) {
    while (in.good()) {
      if (!std::isspace(in.get()))
        { in.unget(); break; }
    }
    if (in.eof())
      in.clear(std::ios::eofbit);
  }
  if (in.fail() || !in.eof()) {
    throw InvalidConversionTo<T>();
//  std::ostringstream msg;
//  msg << "Invalid conversion from '" << str << "' to " << TypeName<T>::name() << " !";
//  throw std::runtime_error(msg.str());
  }
  return retval;
}

template <typename T>
std::string asStr(const T &value) {
  std::ostringstream str;
  str << value;
  return str.str();
}

template <>
inline
std::string asStr<int>(const int &value)
  { return std::to_string(value); }
inline
std::string asStr(const int value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<long>(const long &value)
  { return std::to_string(value); }
inline
std::string asStr(const long value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<long long>(const long long &value)
  { return std::to_string(value); }
inline
std::string asStr(const long long value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<unsigned int>(const unsigned int &value)
  { return std::to_string(value); }
inline
std::string asStr(const unsigned int value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<unsigned long>(const unsigned long &value)
  { return std::to_string(value); }
inline
std::string asStr(const unsigned long value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<unsigned long long>(const unsigned long long &value)
  { return std::to_string(value); }
inline
std::string asStr(const unsigned long long value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<float>(const float &value)
  { return std::to_string(value); }
inline
std::string asStr(const float value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<double>(const double &value)
  { return std::to_string(value); }
inline
std::string asStr(const double value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<long double>(const long double  &value)
  { return std::to_string(value); }
inline
std::string asStr(const long double  value)
  { return std::to_string(value); }

} } // namespace CoSupport::String

/*
#include <iostream>

using namespace CoSupport;

int main() {
  
  // parameter type automatically deduced
  std::string s = string_cast(99);
  std::cout << s << std::endl;
  
  // need to provide return type!
  double d = string_cast<double>(" 50.5e-1 blub");
  std::cout << d << std::endl;

  return 0;
}
*/

#endif // _INCLUDED_COSUPPORT_STRING_CONVERT_HPP
