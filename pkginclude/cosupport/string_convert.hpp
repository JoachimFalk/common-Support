// vim: set sw=2 ts=8:
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

#ifndef _INCLUDED_COSUPPORT_STRING_CONVERT_HPP
#define _INCLUDED_COSUPPORT_STRING_CONVERT_HPP

#include <sstream>
#include <string>
#include <stdexcept>

#include "typename.hpp"

namespace CoSupport {

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
    std::ostringstream msg;
    
    msg << "Invalid conversion from '" << str << "' to " << CoSupport::Type::Name<T>::name() << " !";
    throw std::runtime_error(msg.str());
  }
  return retval;
}

template <typename T>
std::string asStr(const T &value) {
  std::ostringstream str;
  str << value;
  return str.str();
}

/**
 * function for converting any type that overloads operator<<
 * into a string
 */
template<class T>
std::string string_cast(const T& t)
{
  std::ostringstream os;
  os << t;
  return os.str();
}

/**
 * function for converting a string into any type that overloads
 * operator>>
 */
template<class T>
T string_cast(const std::string& s)
{
  std::istringstream is(s);
  T t;
  is >> t;
  return t;
}

/**
 * convenience class that can be used to concatenate
 * some values (instead of using string_cast and operator+,
 * this class is much more efficient, and allows stream
 * manipulators)
 */
class Concat {
private:
  std::ostringstream ret;
public:
  template<class T>
  Concat(const T& t)
  { ret << t; }
  
  template<class T>
  Concat& operator()(const T& t)  {
    ret << t;
    return *this;
  }
  
  Concat& operator()(std::ostream& (*manip)(std::ostream&)) {
    ret << manip;
    return *this;
  }
  
  Concat& operator()(std::ios& (*manip)(std::ios&)) {
    ret << manip;
    return *this;
  }
  
  Concat& operator()(std::ios_base& (*manip)(std::ios_base&)) {
    ret << manip;
    return *this;
  }
  
  std::string get() const
  { return ret.str(); }

  operator std::string() const
  { return ret.str(); }
};
  
/**
 * output operator for Concat
 */
inline std::ostream& operator<<(std::ostream& out, const Concat& c) {
  std::string s = c;
  return out << c;
}

}

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
