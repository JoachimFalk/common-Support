// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/String/convert.hpp>
#include <CoSupport/String/TypeName.hpp>

#include <cstdlib>
#include <limits>

namespace CoSupport { namespace String {

template <>
class InvalidConversionTo<bool>
  : public InvalidConversion
{
public:
  InvalidConversionTo(std::string const &value)
    : InvalidConversion("Could not convert '"+value+"' to a "+TypeName<bool>::name()) {}
  COSUPPORT_ATTRIBUTE_DEPRECATED
  InvalidConversionTo()
    : InvalidConversion(std::string("Invalid conversion to type ")+TypeName<bool>::name()) {}
};

template <>
bool strAs<bool>(std::string const &str)
{
  if (str == "1" || str == "true")
    return true;
  else if (str == "0" || str == "false")
    return false;
  else
    throw InvalidConversionTo<bool>(str);
}

template <typename T>
class InvalidConversionUnderflowOf
  : public InvalidConversionUnderflow
{
public:
  InvalidConversionUnderflowOf(std::string const &value)
      : InvalidConversionUnderflow(
          "Value "+value+" is too negative "
          "(< "+std::to_string(std::numeric_limits<T>::min())+")"
          " to fit the range of a "+TypeName<T>::name()) {}
};

template <typename T>
class InvalidConversionOverflowOf
  : public InvalidConversionOverflow
{
public:
  InvalidConversionOverflowOf(std::string const &value)
      : InvalidConversionOverflow(
          "Value "+value+" is too large "
          "(> "+std::to_string(std::numeric_limits<T>::max())+")"
          " to fit the range of a "+TypeName<T>::name()) {}
};

template <
    typename T
  , bool     USELL = sizeof(T) >= sizeof(long)
  , bool     ERRNO = sizeof(T) >= sizeof(long long)
  >
struct ToSigned;

template <typename T>
struct ToSigned<T, false, false> {

  static T convert(std::string const &str) {
    char *endptr = nullptr;

    long value = strtol(str.c_str(), &endptr, 0);
    if (str.empty() || !endptr || endptr != str.c_str() + str.size())
      throw InvalidConversionTo<T>(str);
    if (value < std::numeric_limits<T>::min())
      throw InvalidConversionUnderflowOf<T>(str);
    if (value > std::numeric_limits<T>::max())
      throw InvalidConversionOverflowOf<T>(str);
    return value;
  }

};

template <typename T>
struct ToSigned<T, true, false> {

  static T convert(std::string const &str) {
    char *endptr = nullptr;

    long long value = strtoll(str.c_str(), &endptr, 0);
    if (str.empty() || !endptr || endptr != str.c_str() + str.size())
      throw InvalidConversionTo<T>(str);
    if (value < std::numeric_limits<T>::min())
      throw InvalidConversionUnderflowOf<T>(str);
    if (value > std::numeric_limits<T>::max())
      throw InvalidConversionOverflowOf<T>(str);
    return value;
  }

};

template <typename T>
struct ToSigned<T, true, true> {

  static T convert(std::string const &str) {
    char *endptr = nullptr;

    errno = 0;
    long long value = strtoll(str.c_str(), &endptr, 0);
    if (str.empty() || !endptr || endptr != str.c_str() + str.size())
      throw InvalidConversionTo<T>(str);
    if (value <  std::numeric_limits<T>::min() ||
        (value == std::numeric_limits<long long>::min() && errno == ERANGE))
      throw InvalidConversionUnderflowOf<short>(str);
    if (value >  std::numeric_limits<T>::max() ||
        (value == std::numeric_limits<long long>::max() && errno == ERANGE))
      throw InvalidConversionOverflowOf<T>(str);
    return value;
  }

};

template <
    typename T
  , bool     USELL = sizeof(T) >= sizeof(unsigned long)
  , bool     ERRNO = sizeof(T) >= sizeof(unsigned long long)
  >
struct ToUnsigned;

template <typename T>
struct ToUnsigned<T, false, false> {

  static T convert(std::string const &str) {
    char *endptr = nullptr;

    unsigned int start;
    for (start = 0;
         start < str.size() && isspace(str[start]);
         ++start)
      ;
    if (start == str.size() || str[start] == '-')
      throw InvalidConversionTo<T>(str);
    unsigned long value = strtoul(str.c_str(), &endptr, 0);
    if (!endptr || endptr != str.c_str() + str.size())
      throw InvalidConversionTo<T>(str);
    if (value > std::numeric_limits<T>::max())
      throw InvalidConversionOverflowOf<T>(str);
    return value;
  }

};

template <typename T>
struct ToUnsigned<T, true, false> {

  static T convert(std::string const &str) {
    char *endptr = nullptr;

    unsigned int start;
    for (start = 0;
         start < str.size() && isspace(str[start]);
         ++start)
      ;
    if (start == str.size() || str[start] == '-')
      throw InvalidConversionTo<T>(str);
    unsigned long long value = strtoull(str.c_str(), &endptr, 0);
    if (!endptr || endptr != str.c_str() + str.size())
      throw InvalidConversionTo<T>(str);
    if (value > std::numeric_limits<T>::max())
      throw InvalidConversionOverflowOf<T>(str);
    return value;
  }

};

template <typename T>
struct ToUnsigned<T, true, true> {

  static T convert(std::string const &str) {
    char *endptr = nullptr;

    unsigned int start;
    for (start = 0;
         start < str.size() && isspace(str[start]);
         ++start)
      ;
    if (start == str.size() || str[start] == '-')
      throw InvalidConversionTo<T>(str);
    errno = 0;
    unsigned long long value = strtoull(str.c_str(), &endptr, 0);
    if (!endptr || endptr != str.c_str() + str.size())
      throw InvalidConversionTo<T>(str);
    if (value >  std::numeric_limits<T>::max() ||
        (value == std::numeric_limits<unsigned long long>::max() && errno == ERANGE))
      throw InvalidConversionOverflowOf<T>(str);
    return value;
  }

};

template <>
int8_t strAs<int8_t>(std::string const &str)
  { return ToSigned<int8_t>::convert(str); }

template <>
short strAs<short>(std::string const &str)
  { return ToSigned<short>::convert(str); }

template <>
int strAs<int>(std::string const &str)
  { return ToSigned<int>::convert(str); }

template <>
long strAs<long>(std::string const &str)
  { return ToSigned<long>::convert(str); }

template <>
long long strAs<long long>(std::string const &str)
  { return ToSigned<long long>::convert(str); }

template <>
uint8_t strAs<uint8_t>(std::string const &str)
  { return ToUnsigned<uint8_t>::convert(str); }

template <>
unsigned short strAs<unsigned short>(std::string const &str)
  { return ToUnsigned<unsigned short>::convert(str); }

template <>
unsigned int strAs<unsigned int>(std::string const &str)
  { return ToUnsigned<unsigned int>::convert(str); }

template <>
unsigned long strAs<unsigned long>(std::string const &str)
  { return ToUnsigned<unsigned long>::convert(str); }

template <>
unsigned long long strAs<unsigned long long>(std::string const &str)
  { return ToUnsigned<unsigned long long>::convert(str); }

} } // namespace CoSupport::String
