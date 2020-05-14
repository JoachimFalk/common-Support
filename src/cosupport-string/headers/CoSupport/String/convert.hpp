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

//#include <cctype>
#include <sstream>
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <cstdint>

//#include "TypeName.hpp"

#include "export_config.h"
#include "Concat.hpp"

namespace CoSupport { namespace String {

class COSUPPORT_STRING_API
InvalidConversion
  : public std::runtime_error
{
public:
  InvalidConversion(std::string const &msg)
      : std::runtime_error(msg) {}
};

template <typename T>
class InvalidConversionTo
  : public InvalidConversion
{
public:
  InvalidConversionTo()
    : InvalidConversion(Concat("CoSupport::String::InvalidConversionTo<")(typeid(T).name())(">")) {}
};

template <typename T>
T strAs(std::string const &str) {
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

template <>
COSUPPORT_STRING_API
int8_t strAs<int8_t>(std::string const &str);

template <>
COSUPPORT_STRING_API
uint8_t strAs<uint8_t>(std::string const &str);

template <typename T>
std::string asStr(const T &value) {
  std::ostringstream str;
  str << value;
  return str.str();
}

template <>
inline
std::string asStr<char>(const char &value)
  { return std::string(1, value); }
inline
std::string asStr(const char value)
  { return std::string(1, value); }

template <>
inline
std::string asStr<int8_t>(const int8_t &value)
  { return std::to_string(value); }
inline
std::string asStr(const int8_t value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<short>(const short &value)
  { return std::to_string(value); }
inline
std::string asStr(const short value)
  { return std::to_string(value); }

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
std::string asStr<uint8_t>(const uint8_t &value)
  { return std::to_string(value); }
inline
std::string asStr(const uint8_t value)
  { return std::to_string(value); }

template <>
inline
std::string asStr<unsigned short>(const unsigned short &value)
  { return std::to_string(static_cast<unsigned>(value)); }
inline
std::string asStr(const unsigned short value)
  { return std::to_string(static_cast<unsigned>(value)); }

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

#endif // _INCLUDED_COSUPPORT_STRING_CONVERT_HPP
