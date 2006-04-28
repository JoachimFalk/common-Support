#ifndef STRING_CONVERT_HPP
#define STRING_CONVERT_HPP

#include <sstream>
#include <string>

namespace CoSupport {
  
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

#endif // STRING_CONVERT_HPP
